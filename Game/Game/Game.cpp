#include "Game.h"

float angle = 0;
int volume_sound = 100;
int volume_music = 100;

bool map_switch = false;

PlayerCollision *player_collision;

//digod nije gib me kad me ubije
//dont fall neprijatelji digod zapnu
//problem jer u animatoru sa f5 se sacuva i promjena scale
//raketa, granata - nastane eksplozija . sprite ne bi treba reagirat na i jedno i drugo, ali je
//AKO UNISTIM NESTO IZ SEKTORA TREBA TO I IZBRISAT OD TAMO TAKODJER;ZATO ZAPINJE IGRAC KAD UNISTIM S BREAK GLASS

void GibMe2(Sprite *a)
{
	vector <Sprite*> s;
	double angle = 0;

	s = a->skeleton;

	int sign=1;
	for (unsigned int i=0;i<s.size();i++)
	{
		if(s[i]->getName()!="$GUN1" && s[i]->getName()!="$GUN2") s[i]->setText("gib");
		s[i]->setClass(-999);
		s[i]->setType(GIBS);
		s[i]->setMovementState(FALL);
		s[i]->elastic = 0.25 + (float)(rand()%10)*0.01;//0.4	
		s[i]->setCollidable(true);		

		s[i]->setPosition(s[i]->getBoxCX(),s[i]->getBoxCY());
		s[i]->setRotationAxis(s[i]->getWidth()/2,s[i]->getHeight()/2);//moglo bi biti problema ovdje zbog scale		

		s[i]->coll_box.clear();
		g_engine->coll_manager->setColl_box(s[i]);
		s[i]->setCollisionMethod(BOX_RXYC);

		g_engine->coll_manager->collbox_update_RXYCbox(s[i]);
		g_engine->coll_manager->TestForCollisions_br(s[i],WORLD_FRONT_COLL);

		if (s[i]->getVelocityX()==0) s[i]->setVelocityX(a->getVelocityX()*1);
		if (s[i]->getVelocityY()==0) s[i]->setVelocityY(a->getVelocityY()*1);
	}	

	a->setAlive(false);
	g_engine->audio->Play("udeath");

}

void BreakGlass2(Sprite *a, int d_w, int d_h, float ex_x, float ex_y)
{
	//treva frame offst namistit
	int width = a->getImage()->tile_width;
	int height = a->getImage()->tile_height;

	bool rows_rest = false;
	bool columns_rest = false;

	int rows = (int)a->w()/width;
	if ((int)a->w() % width != 0)
	{
		rows++;
		rows_rest = true;
	}

	int columns = (int)a->h()/height;
	if ((int)a->h() % height != 0)
	{
		columns++;
		columns_rest = true;
	}

	int draw_width = 0;
	int draw_height = 0;

	//zasto igrac zapinje za njih i medjusobno se dogod sudare, digod se i srusi !!!

	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<columns;j++)
		{
			if (i==rows-1 && rows_rest)
				draw_width = (int)a->w() % width;
			else
				draw_width = a->getImage()->tile_width;

			if (j==columns-1 && columns_rest)
				draw_height = (int)a->h() % height;
			else
				draw_height = a->getImage()->tile_height;

			for(int m=0;m<draw_width/d_w;m++)
			{
				for(int n=0;n<draw_height/d_h;n++)
				{
					float rotation = (float)a->getRotation();			

					D3DXVECTOR2 trans;
					trans.x = (int)((float)(a->getX()+i*width+m*d_w));
					trans.y = (int)((float)(a->getY()+j*height+n*d_h));			

					Sprite *b = new Sprite();
					b->setImage(a->getImage());
					b->setPosition(trans.x+d_w/2,trans.y+d_h/2);
					b->setSize(d_w,d_h);
					b->setType(GIBS);
					b->setRotationAxis(d_w/2,d_h/2);
					g_engine->coll_manager->setColl_box(b);
					b->setCollisionMethod(BOX_RXYC);
					b->setMovementState(FALL);
					b->mass = 0;
					b->elastic = 0.4;
					b->setColor_Texture(a->getColor_Texture());
					g_engine->addSprite(b,PARTICLES_NON);					

					//special
					b->setFrameRelX(m*d_w);
					b->setFrameRelY(n*d_h);

					//koja razlika izmedju setFrameRelX i setFrameX !??

					float angle = g_engine->math->toDegrees(g_engine->math->AngleToTarget(ex_x,ex_y,b->getX(),b->getY()));
					b->setVelocity(
						g_engine->math->LinearVelocityX(angle)*8,
						g_engine->math->LinearVelocityY(angle)*8);
				}
			}			
		}
	}

	a->setAlive(false);
}

void RemoveFromSector(Sprite *a)
{
	//delete it
	for(unsigned int i=0;i<g_engine->game_maps[g_engine->map_current]->sectors.size();i++)
	{
		//can be in several sectors, so no skipping
		std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.begin();
		while (iter != g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.end())
		{
			if(*iter==a)
				iter = g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.erase(iter);
			else
				iter++;
		}		
	}	
}

void UpdateSector(Sprite *a)
{
	//delete it
	RemoveFromSector(a);

	//update
	for(int i=0;i<g_engine->game_maps[g_engine->map_current]->sectors.size();i++)
	{
		if(g_engine->coll_manager->dot_dotrect_nocount(
			a->getX(),a->getY(),
			g_engine->game_maps[g_engine->map_current]->sectors[i].x,g_engine->game_maps[g_engine->map_current]->sectors[i].y,
			g_engine->game_maps[g_engine->map_current]->sectors[i].w,g_engine->game_maps[g_engine->map_current]->sectors[i].h))
		{
			g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.push_back(a);
		}
		else if(g_engine->coll_manager->dot_dotrect_nocount(
			a->getX()+a->w(),a->getY(),
			g_engine->game_maps[g_engine->map_current]->sectors[i].x,g_engine->game_maps[g_engine->map_current]->sectors[i].y,
			g_engine->game_maps[g_engine->map_current]->sectors[i].w,g_engine->game_maps[g_engine->map_current]->sectors[i].h))
		{
			g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.push_back(a);
		}
		else if(g_engine->coll_manager->dot_dotrect_nocount(
			a->getX()+a->w(),a->getY()+a->h(),					
			g_engine->game_maps[g_engine->map_current]->sectors[i].x,g_engine->game_maps[g_engine->map_current]->sectors[i].y,
			g_engine->game_maps[g_engine->map_current]->sectors[i].w,g_engine->game_maps[g_engine->map_current]->sectors[i].h))
		{
			g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.push_back(a);
		}
		else if(g_engine->coll_manager->dot_dotrect_nocount(
			a->getX(),a->getY()+a->h(),
			g_engine->game_maps[g_engine->map_current]->sectors[i].x,g_engine->game_maps[g_engine->map_current]->sectors[i].y,
			g_engine->game_maps[g_engine->map_current]->sectors[i].w,g_engine->game_maps[g_engine->map_current]->sectors[i].h))
		{
			g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.push_back(a);
		}
	}
}

void bullethitskeleton_box(Sprite *b, Sprite *s)
{
	//zbrog mrtvih soldiera
	//return;

	float min_x = 100000;
	float min_y = 100000;
	float x = 100000;
	float y = 100000;
	float min_d = 100000;
	bool collision = false;

	if(b->getID()==RAILGUN || b->getID()==CHAINGUN || b->getName()=="tongue")
	{
		b->setWidth(b->getWidth()+100);
		g_engine->coll_manager->updateColl_box(b);
	}

	for(unsigned int i=0;i<s->skeleton.size();i++)
	{
		Sprite *a = s->skeleton[i];		

		//A-B
		if (g_engine->coll_manager->line_linelinexy(
			a->getX_A(),a->getY_A(),
			a->getX_B(),a->getY_B(),
			b->coll_box[0].getX()+10*b->getVelocityX(),b->coll_box[0].getY()+10*b->getVelocityY(),
			b->coll_box[2].getX()-10*b->getVelocityX(),b->coll_box[2].getY()-10*b->getVelocityY(),
			x,y))
		{
			if(g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY()) < min_d)
			{
				min_x = x;
				min_y = y;
				min_d = g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY());
			}
			collision = true;
		}

		//B-C
		if (g_engine->coll_manager->line_linelinexy(
			a->getX_B(),a->getY_B(),
			a->getX_C(),a->getY_C(),
			b->coll_box[0].getX()+10*b->getVelocityX(),b->coll_box[0].getY()+10*b->getVelocityY(),
			b->coll_box[2].getX()-10*b->getVelocityX(),b->coll_box[2].getY()-10*b->getVelocityY(),
			x,y))
		{
			if(g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY()) < min_d)
			{
				min_x = x;
				min_y = y;
				min_d = g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY());
			}
			collision = true;
		}

		//C-D
		if (g_engine->coll_manager->line_linelinexy(
			a->getX_C(),a->getY_C(),
			a->getX_D(),a->getY_D(),
			b->coll_box[0].getX()+10*b->getVelocityX(),b->coll_box[0].getY()+10*b->getVelocityY(),
			b->coll_box[2].getX()-10*b->getVelocityX(),b->coll_box[2].getY()-10*b->getVelocityY(),
			x,y))
		{
			if(g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY()) < min_d)
			{
				min_x = x;
				min_y = y;
				min_d = g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY());
			}
			collision = true;
		}

		//D-A
		if (g_engine->coll_manager->line_linelinexy(
			a->getX_D(),a->getY_D(),
			a->getX_A(),a->getY_A(),
			b->coll_box[0].getX()+10*b->getVelocityX(),b->coll_box[0].getY()+10*b->getVelocityY(),
			b->coll_box[2].getX()-10*b->getVelocityX(),b->coll_box[2].getY()-10*b->getVelocityY(),
			x,y))
		{
			if(g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY()) < min_d)
			{
				min_x = x;
				min_y = y;
				min_d = g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY());
			}
			collision = true;
		}
	}

	if(collision)
	{
		editor->Particle_manager->addBloodBig(min_x,min_y,17);		
	}
	else
	{
		Sprite *a = s;		

		//A-B
		if (g_engine->coll_manager->line_linelinexy(
			a->getX_A(),a->getY_A(),
			a->getX_B(),a->getY_B(),
			b->coll_box[0].getX()+10*b->getVelocityX(),b->coll_box[0].getY()+10*b->getVelocityY(),
			b->coll_box[2].getX()-10*b->getVelocityX(),b->coll_box[2].getY()-10*b->getVelocityY(),
			x,y))
		{
			if(g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY()) < min_d)
			{
				min_x = x;
				min_y = y;
				min_d = g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY());
			}
			collision = true;
		}

		//B-C
		if (g_engine->coll_manager->line_linelinexy(
			a->getX_B(),a->getY_B(),
			a->getX_C(),a->getY_C(),
			b->coll_box[0].getX()+10*b->getVelocityX(),b->coll_box[0].getY()+10*b->getVelocityY(),
			b->coll_box[2].getX()-10*b->getVelocityX(),b->coll_box[2].getY()-10*b->getVelocityY(),
			x,y))
		{
			if(g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY()) < min_d)
			{
				min_x = x;
				min_y = y;
				min_d = g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY());
			}
			collision = true;
		}

		//C-D
		if (g_engine->coll_manager->line_linelinexy(
			a->getX_C(),a->getY_C(),
			a->getX_D(),a->getY_D(),
			b->coll_box[0].getX()+10*b->getVelocityX(),b->coll_box[0].getY()+10*b->getVelocityY(),
			b->coll_box[2].getX()-10*b->getVelocityX(),b->coll_box[2].getY()-10*b->getVelocityY(),
			x,y))
		{
			if(g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY()) < min_d)
			{
				min_x = x;
				min_y = y;
				min_d = g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY());
			}
			collision = true;
		}

		//D-A
		if (g_engine->coll_manager->line_linelinexy(
			a->getX_D(),a->getY_D(),
			a->getX_A(),a->getY_A(),
			b->coll_box[0].getX()+10*b->getVelocityX(),b->coll_box[0].getY()+10*b->getVelocityY(),
			b->coll_box[2].getX()-10*b->getVelocityX(),b->coll_box[2].getY()-10*b->getVelocityY(),
			x,y))
		{
			if(g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY()) < min_d)
			{
				min_x = x;
				min_y = y;
				min_d = g_engine->math->Distance(x,y,b->getX()-100*b->getVelocityX(),b->getY()-100*b->getVelocityY());
			}
			collision = true;
		}

		if(collision)
		{
			editor->Particle_manager->addBloodBig(min_x,min_y,17);		
		}
	}
}

bool game_preload() 
{
	//load game settings
	Options *options = new Options();
	if (!options->loadSettings()) return false;
	g_engine->setFullscreen(options->s_fullscreen);	
	g_engine->v_sync = options->s_vsync;	

	SCREENW = options->s_screenw;
	SCREENH = options->s_screenh;
	g_engine->setScreenWidth(SCREENW);
	g_engine->setScreenHeight(SCREENH);
	g_engine->lock_framerate = options->s_lockframerate;
	g_engine->p_UpdateTime = 999/options->s_lockframerate;
	g_engine->locked = options->s_gamelocked;

	volume_sound = options->s_volumesound;
	volume_music = options->s_volumemusic;	

	delete options;

	g_engine->setAppTitle("Game");	
	g_engine->setColorDepth(32);

	return true;

}//game_preload

//adding entitiy to non existing list NE GASI PROGRAM !!!!!!!!! u game initu
bool game_init(HWND) 
{
	if(g_engine->ar_direct3d->create_pixelshader("Data/pixelshader.fx"))
		g_engine->p_effect = g_engine->ar_direct3d->getPixelShader();
	else
	{
		g_engine->log_text.push_back("ERROR#PIXEL SHADER");//ovdje treba oni detaljan ispis
		return false;
	}

	g_engine->audio->setVolumeSound((float)volume_sound/100);
	g_engine->audio->setVolumeStream((float)volume_music/100);

	//MAP
	for(unsigned int i=0;i<10;i++)
	{
		AR_Map *temp_map = new AR_Map();
		temp_map->addSpriteLayer(13);
		g_engine->game_maps.push_back(temp_map);
	}	

	//SPRITE AND TEXTURE MANAGEMENT
	g_engine->addSpriteLayer(13);
	TextureManager *texture_manager_temp = new TextureManager(8);
	g_engine->texture_manager = texture_manager_temp;
	g_engine->audio->max_channels = 30;	

	editor = new Editor();
	if (!editor->init()) return false;
	if (!editor->LoadTextures()) return false;

	animator = new Animator();
	if (!animator->LoadTextures()) return false;

	player_collision = new PlayerCollision();	
	editor->addMenuButtons();

	g_engine->font_manager.fonts[0]->scale = editor->options->s_scalehud;
	for(unsigned int i=0;i<editor->Player_managers.size();i++)
	{
		editor->Player_managers[i]->hud->gui_scale = editor->options->s_scalehud;
	}
	editor->gui_scale = editor->options->s_scalehud;

	return true;
}

void DeletePlayer(int id)
{
	if (editor->Player_managers[id]->p_player==NULL) return;
	else
	{
		for (unsigned int i=0;i<editor->Player_managers[id]->p_player->skeleton.size();i++)
		{
			editor->Player_managers[id]->p_player->skeleton[i]->setAlive(false);
		}
		for (unsigned int i=0;i<editor->Player_managers[id]->p_player->attachments.size();i++)
		{
			editor->Player_managers[id]->p_player->attachments[i]->setAlive(false);
		}
		editor->Player_managers[id]->p_player->setAlive(false);		
		editor->Player_managers[id]->p_player=NULL;		
	}
}

void KillPlayer(int id,Sprite *b,Sprite *boom, char death, double power)
{
	if (death=='B')
	{
		GibMe(b,'S',boom,power,'B');
		GibMe(b,'A',boom,power,'B');
	}
	else if (death=='P')
	{
		GibMe(b,'S',boom,power,'P');
		GibMe(b,'A',boom,power,'P');         
	}
	else if (death=='F')
	{
		GibMe(b,'S',boom,power,'F');
		GibMe(b,'A',boom,power,'F');         
	}

	int index = 0;
	switch(rand()%4)
	{
	case 0:index=1;break;
	case 1:index=2;break;
	case 2:index=3;break;
	case 3:index=4;break;
	}

	g_engine->audio->Play_path(editor->Player_managers[id]->player_models[editor->Player_managers[id]->model_id]->root_directory + "death" + g_engine->inttostring(index) + ".wav");

	editor->Player_managers[id]->p_player->setAlive(false);
	editor->Player_managers[id]->p_player = NULL;

	if(id==0 && !editor->Player_managers[1]->p_player) g_engine->setGameState(GAMEOVER);
	if(id==1 && !editor->Player_managers[0]->p_player) g_engine->setGameState(GAMEOVER);

}//KillPlayer()


bool Swim(Sprite *test_me)
{
	std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_NON]->sprites.begin();
	while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_NON]->sprites.end())
	{			
		Sprite *a = *iter;//temporary pointer			
		if ( a->getAlive() && a->getCollidable() && a->getID()==WATER)//is this entity in use?
		{
			if(g_engine->coll_manager->TestCollision_br(test_me,a))
			{
				if(test_me->getVelocityX()>2)
				{
					test_me->setVelocityX(2);
				}
				else if(test_me->getVelocityX()<-2)
				{
					test_me->setVelocityX(-2);
				}

				test_me->setVelocityY(test_me->getVelocityY()+0.06*g_engine->game_speed*g_engine->GetDeltaFix());

				//privremeno rjesenje - i dalje ne radi wtf !? ili je
				if(test_me->getVelocityY()>1.5)
				{
					test_me->setVelocityY(1.5);
				}
				else if(test_me->getVelocityY()<-1.5)
				{
					test_me->setVelocityY(-1.5);
				}

				return true;				
			}
		}
		iter++;
	}

	return false;
}

void UpdatePlayer(int id)
{
	Sprite *a = editor->Player_managers[id]->p_player;

	a->setMovementState(FALL);

	if(a->energy<=0)//digod se ne raspadne pa mozda ovo pomogne
	{
		GibMe2(a);
		editor->Player_managers[id]->p_player->setAlive(false);
		editor->Player_managers[id]->p_player = NULL;

		if(id==0 && !editor->Player_managers[1]->p_player) g_engine->setGameState(GAMEOVER);
		if(id==1 && !editor->Player_managers[0]->p_player) g_engine->setGameState(GAMEOVER);
		return;
	}

	if(!g_engine->noclip)
	{
		if(!Swim(a))
		{
			if(a->getVelocityY()<24)//0.5
				a->setVelocityY(a->getVelocityY()+0.4*g_engine->game_speed*g_engine->GetDeltaFix());
		}
	}

	editor->Player_managers[id]->UpdatePlayer();	
	g_engine->coll_manager->updateColl_box(a);

	if(!g_engine->noclip)
	{
		if(!editor->Player_managers[id]->in_lift)
		{
			player_collision->PlatformPlayerCollision(a,WORLD_FRONT_COLL);
			g_engine->coll_manager->TestForCollisions_br(a,g_engine->map_current,PARTICLES_COLL);//predmeti, treba i mapa sada			
		}
		else
		{
			player_collision->PlatformPlayerCollision(a,BACKGROUND_1);
			g_engine->coll_manager->TestForCollisions_br(a,g_engine->map_current,PARTICLES_COLL);
		}
	}

	if (editor->Player_managers[id]->p_player == NULL) return;
	if (editor->Player_managers[id]->p_player->energy <= 0) return;

	float down = 0;
	for(int i=0;i<g_engine->game_maps[g_engine->map_current]->border_down_vec.size()-1;i++)
	{
		if((a->cx()>g_engine->game_maps[g_engine->map_current]->border_x_vec[i]) && (a->cx()<g_engine->game_maps[g_engine->map_current]->border_x_vec[i+1]))
		{
			float alpha = g_engine->math->AngleToTarget(
				g_engine->game_maps[g_engine->map_current]->border_x_vec[i],g_engine->game_maps[g_engine->map_current]->border_down_vec[i],
				g_engine->game_maps[g_engine->map_current]->border_x_vec[i+1],g_engine->game_maps[g_engine->map_current]->border_down_vec[i+1]);
			float delta_y = tan(alpha)*fabs(a->cx()-g_engine->game_maps[g_engine->map_current]->border_x_vec[i]);

			g_engine->game_maps[g_engine->map_current]->border_down = g_engine->game_maps[g_engine->map_current]->border_down_vec[i] + delta_y;
			editor->Player_managers[id]->screen->border_down =  g_engine->game_maps[g_engine->map_current]->border_down_vec[i] + delta_y;
		}
	}

	if(editor->Player_managers[id]->player_control==KEYBOARD_MOUSE)
	{
		editor->Player_managers[id]->screen->setOffset(
			a->cx(),a->getY()+a->h(),
			(float)((editor->Player_managers[id]->screen->getWidth()/2-(g_engine->gui->cursor->cx()))*0.5),
			(float)((editor->Player_managers[id]->screen->getHeight()/2-(g_engine->gui->cursor->cy()))*0.5));		
	}
	else
	{
		editor->Player_managers[id]->screen->setOffset(a->cx(),a->getY()+a->h(),0,0);
	}

	g_engine->offset_X = editor->Player_managers[id]->screen->getOffsetX();
	g_engine->offset_Y = editor->Player_managers[id]->screen->getOffsetY();

	editor->Player_managers[id]->UpdateSkeleton();	

	////FIRE WEAPON
	if (editor->Player_managers[id]->player_fire == true) editor->Player_managers[id]->FireWeapon();
	editor->Player_managers[id]->player_fire = false;

	editor->Player_managers[id]->kneel = false;//zbog joysticka, nemogu provjerit jel sta pritisnuto (ili mogu ?)	

}//UpdatePlayer()

void game_update() 
{
	if (g_engine->getEngineState()==ANIMATION){animator->Update();return;}	

	if (g_engine->getEngineState()!=GAME)
	{
		Sprite *c;
		std::list<Sprite*>::iterator iter;

		for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
		{
			iter = g_engine->game_sprites[i]->sprites.begin();
			while (iter != g_engine->game_sprites[i]->sprites.end())
			{
				c = *iter;
				g_engine->coll_manager->collbox_update(c);
				iter++;
			}
		}

		return;
	}

	if(g_engine->bullet_time)
		g_engine->game_speed = 0.4;
	else
		g_engine->game_speed = 1;

	static double freq;

	if (g_engine->bullet_time && freq>0.4)//0.6
	{
		freq-=0.01;
		if (freq < 0.4) freq = 0.4;
		g_engine->audio->setFrequencyGlobal(freq);
		g_engine->audio->UpdateFrequencyGlobal();
	}
	else if (!g_engine->bullet_time && freq<1)
	{
		freq+=0.01;
		if (freq>1) freq=1;
		g_engine->audio->setFrequencyGlobal(freq);
		g_engine->audio->UpdateFrequencyGlobal();
	}

	//UPDATE GAME ENTITIES
	if(!g_engine->getPaused() && g_engine->getEngineState()==GAME && (g_engine->getGameState()==PLAY || g_engine->getGameState()==GAMEOVER))
	{
		if(!g_engine->save_to_map)
		{
			g_engine->game_sprites[WORLD_FRONT_COLL]->UpdateSprites();//update sprites

			for(unsigned int i=0;i<editor->Player_managers.size();i++)//update players
			{
				if(editor->Player_managers[i]->p_player)
				{
					if(editor->Player_managers[i]->p_player->energy>0) UpdatePlayer(i);
				}
			}	

			g_engine->game_sprites[PLAYER]->UpdateSprites();
			g_engine->game_sprites[BACKGROUND_0]->UpdateSprites();
			g_engine->game_sprites[BACKGROUND_1]->UpdateSprites();
			g_engine->game_sprites[WORLD_BACK_0]->UpdateSprites();
			g_engine->game_sprites[WORLD_BACK_1]->UpdateSprites();
			g_engine->game_sprites[ENEMIES]->UpdateSprites();
			g_engine->game_sprites[PARTICLES_COLL]->UpdateSprites();
			g_engine->game_sprites[PARTICLES_NON]->UpdateSprites();
			g_engine->game_sprites[WORLD_FRONT_NON]->UpdateSprites();
			g_engine->game_sprites[SHADERSPRITES]->UpdateSprites();
			g_engine->game_sprites[LAYER_SOUND]->UpdateSprites();
		}
		else
		{
			g_engine->map_current = editor->Player_managers[0]->map_id;

			g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_COLL]->UpdateSprites();//update sprites

			if(editor->Player_managers[0]->p_player)
			{
				if(editor->Player_managers[0]->p_player->energy>0) UpdatePlayer(0);				
			}

			if(editor->Player_managers[1]->p_player)
			{
				if(editor->Player_managers[0]->map_id == editor->Player_managers[1]->map_id)
				{
					if(editor->Player_managers[1]->active) UpdatePlayer(1);
				}
			}

			g_engine->game_maps[g_engine->map_current]->sprite_layers[PLAYER]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[BACKGROUND_0]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[BACKGROUND_1]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_0]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_1]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[ENEMIES]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[PARTICLES_COLL]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[PARTICLES_NON]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_NON]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[SHADERSPRITES]->UpdateSprites();
			g_engine->game_maps[g_engine->map_current]->sprite_layers[LAYER_SOUND]->UpdateSprites();

			if(editor->Player_managers[0]->active && editor->Player_managers[1]->active)
			{
				if(editor->Player_managers[0]->map_id != editor->Player_managers[1]->map_id)
				{
					g_engine->map_current = editor->Player_managers[1]->map_id;

					g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_COLL]->UpdateSprites();//update sprites

					if(editor->Player_managers[1]->p_player) UpdatePlayer(1);

					g_engine->game_maps[g_engine->map_current]->sprite_layers[PLAYER]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[BACKGROUND_0]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[BACKGROUND_1]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_0]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_1]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[ENEMIES]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[PARTICLES_COLL]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[PARTICLES_NON]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_NON]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[SHADERSPRITES]->UpdateSprites();
					g_engine->game_maps[g_engine->map_current]->sprite_layers[LAYER_SOUND]->UpdateSprites();
				}
			}
		}		
	}

	g_engine->map_current = editor->Player_managers[0]->map_id;

	for(unsigned int i=0;i<editor->Player_managers.size();i++)//update players
	{
		editor->Player_managers[i]->control_action_prev = editor->Player_managers[i]->control_action;
	}

	map_switch = false;					

}//game_update()

void FadeAway(Sprite *a, float step)
{
	double temp_alpha = a->getAlpha() - step*(double)g_engine->game_speed*g_engine->GetDeltaFix();

	if (temp_alpha<=0)
	{
		a->setAlive(false); 
		return;
	}

	a->setColor_Texture(
		D3DCOLOR_ARGB(
		(int)temp_alpha%256,
		g_engine->ar_graphics->getRed(a->getColor_Texture()),
		g_engine->ar_graphics->getGreen(a->getColor_Texture()),
		g_engine->ar_graphics->getBlue(a->getColor_Texture())));

	a->setAlpha(temp_alpha);

}

void GoRed(Sprite *a, float step)
{
	double temp_alpha = a->getAlpha() - step*(double)g_engine->game_speed*g_engine->GetDeltaFix();

	if (temp_alpha<=0) temp_alpha = 0;

	a->setColor_Texture(
		D3DCOLOR_ARGB(
		g_engine->ar_graphics->getGreen(a->getColor_Texture()),
		g_engine->ar_graphics->getRed(a->getColor_Texture()),
		(int)temp_alpha%256,
		g_engine->ar_graphics->getBlue(a->getColor_Texture())));

	a->setAlpha(temp_alpha);
}

void game_spriteUpdate(Advanced2D::Sprite* b) 
{
	if (g_engine->getGameState()==PLAY  || g_engine->getGameState()==GAMEOVER)
	{
		switch(b->getClass())
		{
		case SKELETON:
			//b->animate();
			if(b->getState()!=1) b->setFrameCurrent(0);
			if(b->getArg(2)!="#") b->setAnimationLoop((int)g_engine->stringtoint(b->getArg(2)));
			if(g_engine->animation_manager.getAnimation(b->getArg(1))) g_engine->animation_manager.getAnimation(b->getArg(1))->setRotation(b);
			g_engine->skeleton_manager.ConnectBones(b);
			return;
			break;

		case FLY:
			for(unsigned int i=0;i<b->children.size();i++)
			{
				if(b->children[i]->getX()>b->getX()+b->w())
				{
					b->children[i]->setX(b->getX()+b->w());
					b->children[i]->setVelocityX(-b->children[i]->getVelocityX());
				}

				if(b->children[i]->getX()<b->getX())
				{
					b->children[i]->setX(b->getX());
					b->children[i]->setVelocityX(-b->children[i]->getVelocityX());
				}

				if(b->children[i]->getY()>b->getY()+b->h())
				{
					b->children[i]->setY(b->getY()+b->h());
					b->children[i]->setVelocityY(-b->children[i]->getVelocityY());
				}

				if(b->children[i]->getY()<b->getY())
				{
					b->children[i]->setY(b->getY());
					b->children[i]->setVelocityY(-b->children[i]->getVelocityY());
				}
			}
			return;
			break;

		case SOUND:
			{
				//treba doradit on eksplozije da uopce nije sound, nego obicni sprite
				if(b->getID()==SOUND_ONCE)
				{
					for(unsigned int i=0;i<editor->Player_managers.size();i++)
					{				
						if(editor->Player_managers[i]->p_player && b->getAlive() )
						{
							if(editor->Player_managers[i]->p_player->map_current == b->map_current)
							{
								if(g_engine->coll_manager->TestCollision_br(editor->Player_managers[i]->p_player,b))
								{
									g_engine->audio->Play(b->getArg(0));
									if(b->getText()=="ROCKLX1A")
									{									
										for(unsigned int i=0;i<b->children.size();i++)
										{
											editor->Particle_manager->addSmoke(b->children[i]->cx(),b->children[i]->cy(),21);
											editor->Particle_manager->addBloodBig(b->children[i]->cx(),b->children[i]->cy(),21);
											editor->Particle_manager->addSpark(b->children[i]->cx(),b->children[i]->cy(),21);
											editor->Particle_manager->addFire(b->children[i]->cx(),b->children[i]->cy(),21);
											b->children[i]->setAlive(false);
										}
									}
									b->setAlive(false);
								}
							}
						}
					}					
				}
				else if(b->getID()==SOUND_POINT)
				{
					if(b->getArg(b->arg_list.size()-1)[0]=='-')//global sound
					{
						/*if(!g_engine->audio->IsPlaying(b->getArg(0)))
						{
						b->sound = g_engine->audio->Play(b->getArg(0));									
						}*/

						bool play = false;
						for(unsigned int i=0;i<b->arg_list.size()-1;i++)
						{
							//is some sound playing
							if(g_engine->audio->IsPlaying(b->getArg(i))) play = true;
						}

						if(!play)//no sound is playing, play one random
						{
							int temp = rand()%(int)b->arg_list.size();
							b->sound = g_engine->audio->Play(b->getArg(rand()%(int)(b->arg_list.size()-1)));
						}
					}
					else
					{
						for(unsigned int i=0;i<editor->Player_managers.size();i++)
						{				
							if(editor->Player_managers[i]->p_player)
							{
								if(editor->Player_managers[i]->p_player->map_current == b->map_current)
								{
									if(!g_engine->audio->IsPlaying(b->getArg(0)) && g_engine->math->Distance(
										b->cx(),b->cy(),
										editor->Player_managers[i]->p_player->cx(),
										editor->Player_managers[i]->p_player->cy())<b->rel_posx)
									{
										b->sound = g_engine->audio->Play(b->getArg(0));
									}


									if(b->sound) b->sound->setVolume(
										((b->rel_posx-g_engine->math->Distance(
										b->cx(),b->cy(),
										editor->Player_managers[i]->p_player->cx(),
										editor->Player_managers[i]->p_player->cy()))/b->rel_posx
										)*g_engine->audio->getVolumeSound());
								}
							}
						}	
					}
				}
				else//bit ce problema prilikom prelaska mapa
				{
					bool play = false;
					for(unsigned int i=0;i<editor->Player_managers.size();i++)
					{				
						if(editor->Player_managers[i]->p_player)
						{
							if(editor->Player_managers[i]->p_player->map_current == b->map_current)
							{
								if(g_engine->coll_manager->TestCollision_br(editor->Player_managers[i]->p_player,b))
								{
									play = true;
									if(!g_engine->audio->IsPlaying(b->getArg(0)))
									{
										b->sound = g_engine->audio->Play(b->getArg(0));									
									}

									if(b->sound)
									{
										Sprite *a = editor->Player_managers[i]->p_player;
										float posx;
										float posy;									

										//horizontal
										if(b->w()>=b->h())
										{
											posy = b->getY()+b->hh();
											if(a->cx()<b->getX()+b->hh())
											{
												posx = b->getX()+b->hh();										
											}
											else if(a->cx()>b->getX()+b->w()-b->hh())
											{
												posx = b->getX()+b->w()-b->hh();										
											}
											else
											{
												posx = a->cx();										
											}
										}
										else//vertical
										{
											posx = b->getX()+b->hw();
											if(a->cy()<b->getY()+b->hw())
											{
												posy = b->getY()+b->hw();										
											}
											else if(a->cy()>b->getY()+b->h()-b->hw())
											{
												posy = b->getY()+b->h()-b->hw();										
											}
											else
											{
												posy = a->cy();										
											}
										}

										b->sound->setVolume(
											((b->hh()-g_engine->math->Distance(
											posx,posy,
											editor->Player_managers[i]->p_player->cx(),
											editor->Player_managers[i]->p_player->cy()))/b->hh()
											)*g_engine->audio->getVolumeSound());

										if(g_engine->display_lineofsight)
										{
											Line *temp_line0 = new Line(posx - 10, posy - 10, posx + 10, posy + 10, a->getLayer(), 0xFFFFFFFF);				
											g_engine->line_list.push_back(temp_line0);
											Line *temp_line1 = new Line(posx + 10, posy - 10, posx - 10, posy + 10, a->getLayer(), 0xFFFFFFFF);				
											g_engine->line_list.push_back(temp_line1);
										}
									}
								}
							}
						}
					}

					if(play==false)
					{
						if(b->sound)
						{
							b->sound->stop(); 
							b->sound = NULL;
						}
					}
				}
			}
			break;
		}

		switch(b->getType())
		{
		case 5001:
		case PLATFORMA:			
			if(b->getState()==DYNAMIC)// ovdje bi se mogla kolizija dvaputa provjeravat
			{				
				b->elastic = 0.25;
				if (abs(b->getVelocityY()<24))
				{
					b->setVelocityY(b->getVelocityY()+0.5*g_engine->game_speed*g_engine->GetDeltaFix());
					b->setY(b->getY()+0.5*g_engine->game_speed*g_engine->GetDeltaFix());					
				}				

				g_engine->coll_manager->updateColl_box(b);
				UpdateSector(b);
				player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);

				if(b->getText()=="explode")
				{
					b->mass = 50;
					//player_collision->DynamicCollision(b,PLAYER);//ne ce radit u spojenm mapama
				}
			}
			else
			{
				if(b->getVelocityX()!=0 || b->getVelocityY()!=0)//trebat ce ovo samo kad se pomice
				{
					b->elastic = 1;
					g_engine->coll_manager->updateColl_box(b);					
				}
			}

			if(b->getID()==452)//secret wall
			{
				if(b->getVisible())
				{
					for(unsigned int i=0;i<editor->Player_managers.size();i++)
					{
						if(editor->Player_managers[i]->p_player)
						{
							if(editor->Player_managers[i]->p_player->map_current == b->map_current)
							{
								if(g_engine->coll_manager->TestCollision_br(editor->Player_managers[i]->p_player,b))
								{
									b->setVisible(false);
									g_engine->audio->Play("secret");
									for(unsigned int i=0;i<b->children.size();i++)
									{
										if(b->children[i]->getLayer()==SHADERSPRITES)
											b->children[i]->setVisible(true);
										else
											b->children[i]->setVisible(false);
									}							
								}
							}
						}
					}

					if(b->getVisible())
					{
						for(unsigned int i=0;i<b->children.size();i++)
						{
							if(b->children[i]->getLayer()==SHADERSPRITES) b->children[i]->setVisible(false);
						}	
					}
				}
			}

			if(b->getID()==ITEM)
			{
				if(b->getVelocityY()>0 && b->getY()>b->rel_posy) b->setVelocityY(-0.3);
				if(b->getVelocityY()<0 && b->getY()<b->rel_posx) b->setVelocityY(0.3);
			}

			//trebat ce kombinirat oba tipa kocka kolizije,line i dot
			if(b->getClass()==SPAWNER)
			{
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					if(editor->Player_managers[i]->p_player)
					{
						if(editor->Player_managers[i]->p_player->map_current == b->map_current)
							if(g_engine->coll_manager->TestCollision_br(editor->Player_managers[i]->p_player,b))
							{
								editor->Map_LoadEntitiesFromFile(b->page,0);
								b->setAlive(false);
							}
					}
				}
			}

			//treba zaobic koliziju, ne provjeravat je za neke platforme
			if(b->getArg(0)=="glass" || b->getArg(0)=="glass")
			{
				g_engine->coll_manager->TestForCollisions_br(b,g_engine->map_current,PARTICLES_NON);//da se razbije uslijed eksplozije
			}
			else if(b->getArg(0)=="move_x")
			{
				if(b->getState()==1)
				{
					if(
						((b->getArg(1)[0]=='-') && (b->getX()<b->rel_posx))
						||
						((b->getArg(1)[0]!='-') && (b->getX()>b->rel_posx))
						)
					{
						b->setX(b->rel_posx);
						b->setVelocityX(0);
						b->setState(-999);
						b->setText(
							b->getArg(0)
							+ " " + g_engine->inttostring(-(int)g_engine->stringtoint(b->getArg(1)))
							+ " " + g_engine->inttostring(-g_engine->stringtoint(b->getArg(2)))
							);						

						g_engine->audio->Play("pt1_end");
					}

					if(b->getLayer()==WORLD_FRONT_COLL) UpdateSector(b);
					g_engine->coll_manager->updateColl_box(b);
				}

				for(unsigned int i=0;i<b->children.size();i++)
				{
					b->children[i]->setVelocity(b->getVelocityX(),b->getVelocityY());
					g_engine->coll_manager->collbox_update(b->children[i]);
					if(b->children[i]->getLayer()==WORLD_FRONT_COLL) UpdateSector(b->children[i]);
				}
			}
			else if(b->getArg(0)=="move_y")
			{
				if(b->getState()==1)
				{
					if(
						((b->getArg(1)[0]=='-') && (b->getY()<b->rel_posy))
						||
						((b->getArg(1)[0]!='-') && (b->getY()>b->rel_posy))
						)
					{
						b->setY(b->rel_posy);
						b->setVelocityY(0);
						b->setState(-999);
						b->setText(
							b->getArg(0)
							+ " " + g_engine->inttostring(-(int)g_engine->stringtoint(b->getArg(1)))
							+ " " + g_engine->inttostring(-g_engine->stringtoint(b->getArg(2)))
							);						

						g_engine->audio->Play("pt1_end");
					}

					if(b->getLayer()==WORLD_FRONT_COLL) UpdateSector(b);
					g_engine->coll_manager->updateColl_box(b);
				}

				for(unsigned int i=0;i<b->children.size();i++)
				{
					b->children[i]->setVelocity(b->getVelocityX(),b->getVelocityY());
					g_engine->coll_manager->collbox_update(b->children[i]);
					if(b->children[i]->getLayer()==WORLD_FRONT_COLL) UpdateSector(b->children[i]);
				}
			}
			else if(b->getArg(0)=="button")
			{
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					if(editor->Player_managers[i]->p_player)
					{
						if(editor->Player_managers[i]->p_player->map_current == b->map_current)
						{
							Sprite *p = editor->Player_managers[i]->p_player;
							if(g_engine->coll_manager->TestCollision_br(editor->Player_managers[i]->p_player,b)
								|| g_engine->coll_manager->line_rectrect(p->getX(),p->getY(),p->w(),p->h(),b->getX(),b->getY(),b->w(),b->h()))
							{
								if(editor->Player_managers[i]->control_action_prev && !editor->Player_managers[i]->control_action)
								{
									if(b->getArg(1)=="bluekey" || b->getArg(1)=="redkey")
									{
										if(editor->Player_managers[i]->getInventory(b->getArg(1)))
										{
											editor->Player_managers[i]->RemoveFromInventory(b->getArg(1));
											b->setText(b->getArg(0));
											g_engine->audio->Play("keyuse");
										}
										else
										{
											g_engine->audio->Play("keytry");
											return;
										}
									}
									else if(b->getArg(1)=="gameover")
									{
										g_engine->setGameState(GAMEOVER);
									}

									//trebat ce vise, kad treba uc i izac, da ne izadje usred platforme, treba povezat area s samim liftom
									for(unsigned int i=0;i<b->children.size();i++)
									{
										if(b->children[i]->getArg(0)=="move_x")
										{
											if(b->children[i]->getState()==-999)
											{
												g_engine->audio->Play("pt1_strt");
												b->children[i]->setState(1);
												b->children[i]->setVelocityX(g_engine->stringtoint(b->children[i]->getArg(2)));
												b->children[i]->rel_posx = b->children[i]->getX()+(int)g_engine->stringtoint(b->children[i]->getArg(1));
												if(b->getArg(1)!="bluekey" && b->getArg(1)!="redkey")
												{
													g_engine->audio->Play("butn2");
												}
											}
										}
										else if(b->children[i]->getArg(0)=="move_y")
										{
											if(b->children[i]->getState()==-999)
											{
												g_engine->audio->Play("pt1_strt");
												b->children[i]->setState(1);
												b->children[i]->setVelocityY(g_engine->stringtoint(b->children[i]->getArg(2)));
												b->children[i]->rel_posy = b->children[i]->getY()+(int)g_engine->stringtoint(b->children[i]->getArg(1));
												if(b->getArg(1)!="bluekey" && b->getArg(1)!="redkey")
												{
													g_engine->audio->Play("butn2");
												}
											}
										}

										if(b->children[i]->getClass()==SKELETON)
										{
											b->children[i]->setState(1);
										}
									}
								}

								Sprite *temp= new Sprite();
								temp->setID(217);
								temp->setName("use");
								temp->setImage(editor->t_action);
								temp->setPosition(
									editor->Player_managers[i]->p_player->cx()-temp->hw(),
									editor->Player_managers[i]->p_player->getY()-1.5*temp->h());
								temp->setCollidable(false);
								temp->oneframelife = true;
								g_engine->addSprite(temp,WORLD_FRONT_NON);
							}						
						}
					}
				}
			}			
			else if(b->getText()=="ship")
			{
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					if(editor->Player_managers[i]->p_player && b->getClass()==-999)
					{
						if(editor->Player_managers[i]->p_player->map_current == b->map_current)
						{
							if(fabs(b->getX()-editor->Player_managers[i]->p_player->getX()) < 1500)
							{
								g_engine->audio->Play("flyby1");
								b->setClass(-1);
							}
						}
					}
				}

				if(b->getX()>b->getState())
				{
					b->setX(b->getID());					
					b->setClass(-999);
				}
			}
			if(b->getText()=="instagib")//crveni laser
			{
				if(!b->children.empty())
				{
					//problem x kolizije
					if(g_engine->coll_manager->line_rectrect(b->children[0]->getX(),b->children[0]->getY(),b->children[0]->w(),b->children[0]->h(),b->getX(),b->getY(),b->w(),b->h()))
						b->setVisible(false);
					else
						b->setVisible(true);
				}

				if(b->getVisible())
				{
					for(unsigned int i=0;i<editor->Player_managers.size();i++)
					{
						if(editor->Player_managers[i]->p_player)
						{
							if(editor->Player_managers[i]->p_player->map_current == b->map_current)
							{
								Sprite *p = editor->Player_managers[i]->p_player;
								if(g_engine->coll_manager->line_rectrect(b->getX(),b->getY(),b->w(),b->h(),p->getX(),p->getY(),p->w(),p->h()))
								{
									p->energy -= 5*g_engine->game_speed*g_engine->GetDeltaFix();

									bullethitskeleton_box(b,p);

									if(p->energy<=0) KillPlayer(
										editor->Player_managers[i]->p_player->getID(),
										editor->Player_managers[i]->p_player,b,
										'B',5);
								}	
							}
						}
					}
				}
			}
			else if(b->getText()=="button-2")//sta je ovo
			{				
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					if(editor->Player_managers[i]->p_player)
					{
						if(editor->Player_managers[i]->p_player->map_current == b->map_current)
						{
							Sprite *p = editor->Player_managers[i]->p_player;
							if(g_engine->coll_manager->TestCollision_br(editor->Player_managers[i]->p_player,b)
								|| g_engine->coll_manager->line_rectrect(p->getX(),p->getY(),p->w(),p->h(),b->getX(),b->getY(),b->w(),b->h()))
							{
								if(editor->Player_managers[i]->control_action_prev && !editor->Player_managers[i]->control_action)
								{
									//trebat ce vise, kad treba uc i izac, da ne izadje usred platforme, treba povezat area s samim liftom
									if(b->children.size()==1)
									{
										//stopped
										if(b->children[0]->getVelocityY()==0 && b->children[0]->getVelocityX()==0)
											b->children[0]->setVelocityY(-2);
										else if(b->children[0]->getVelocityY()!=0)
											b->children[0]->setVelocityY(-b->children[0]->getVelocityY());
										else if(b->children[0]->getVelocityX()!=0)
											b->children[0]->setVelocityX(-b->children[0]->getVelocityX());

										g_engine->coll_manager->updateColl_box(b->children[0]);

										g_engine->audio->Play("butn2");
									}
								}

								Sprite *temp= new Sprite();
								temp->setID(217);
								temp->setName("call tram");
								temp->setImage(editor->t_action);
								temp->setPosition(
									editor->Player_managers[i]->p_player->cx()-temp->hw(),
									editor->Player_managers[i]->p_player->getY()-1.5*temp->h());
								temp->setCollidable(false);
								temp->oneframelife = true;
								g_engine->addSprite(temp,WORLD_FRONT_NON);
							}						
						}
					}
				}
			}
			else if(b->getText()=="redlift")//sta je ovo
			{				
				for(unsigned int i=0;i<b->children.size();i++)
				{
					if(b->children[i]->getLayer()==WORLD_FRONT_COLL) UpdateSector(b->children[i]);
					//mozda da stavim parent, pa da se ovo namisti kad se child updatea
					b->children[i]->setPosition(b->getX()+b->children[i]->getID(),b->getY()+b->children[i]->getState());
					b->children[i]->setVelocity(b->getVelocityX(),b->getVelocityY());
					g_engine->coll_manager->updateColl_box(b->children[i]);
					player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);
					//b->children[i]->setCollidable(false);
				}			
			}
			else if(b->getText()=="lift")
			{
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					if(editor->Player_managers[i]->p_player)
					{
						if(editor->Player_managers[i]->p_player->map_current == b->map_current)
						{							
							if(!g_engine->coll_manager->TestCollision_br(b,editor->Player_managers[i]->p_player))
							{
								//g_engine->audio->Play("jump1");
								if(b->getY()<b->getID()) b->setVelocityY(1);
								else
								{
									if(b->getVelocityY()>0) g_engine->audio->Play("pt1_strt");//treba i kad ide od gore i dolje, a ne samo prema gore i jos izmedju zvuk
									b->setVelocityY(0);
								}
							}
						}
					}
				}

				for(unsigned int i=0;i<b->children.size();i++)
				{
					//mozda da stavim parent, pa da se ovo namisti kad se child updatea
					b->children[i]->setPosition(b->getX()+b->children[i]->getID(),b->getY()+b->children[i]->getState());
					b->children[i]->setVelocity(b->getVelocityX(),b->getVelocityY());
					g_engine->coll_manager->updateColl_box(b->children[i]);
					//player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);
					//b->children[i]->setCollidable(false);
				}
			}
			else if(b->getText()=="blink")
			{
				if(b->fire_tick<50/14) b->fire_tick += g_engine->game_speed*g_engine->GetDeltaFix();
				else
				{
					int temp = rand()%256;
					b->setColor_Texture(D3DCOLOR_ARGB(temp,(int)b->getRed(),(int)b->getGreen(),(int)b->getBlue()));
					b->fire_tick = 0;
					for(unsigned int i=0;i<b->children.size();i++)
					{
						b->children[i]->setColor_Texture(D3DCOLOR_ARGB(temp,(int)b->children[i]->getRed(),(int)b->children[i]->getGreen(),(int)b->children[i]->getBlue()));						

					}
				}
			}
			else if(b->getText()=="spark")
			{
				if(b->fire_tick<4750/14) b->fire_tick += g_engine->game_speed*g_engine->GetDeltaFix();
				else
				{
					editor->Particle_manager->addBulletSpark(b->cx(),b->cy(),11);
					b->fire_tick = 0;
				}				
			}			
			else if(b->getText()=="door_r")
			{
				if(b->getState()==1 && b->getX()<b->rel_posx + 64)
				{
					b->setX(b->getX()+g_engine->game_speed*g_engine->GetDeltaFix());
					if(b->getX()>=b->rel_posx + 64)
					{
						b->setX(b->rel_posx + 64);
						b->setState(0);
						//b->setVelocityX(0);
					}
				}

				if(!b->children.empty())
				{
					if(b->getState()==0)
					{
						b->children[0]->setCollidable(true);
						g_engine->coll_manager->setColl_box(b->children[0]);
						b->children.clear();
					}
				}
			}
			else if(b->getText()=="door_l")
			{
				if(b->getState()==1 && b->getX()>b->rel_posx - 64)
				{
					b->setX(b->getX()-g_engine->game_speed*g_engine->GetDeltaFix());
					if(b->getX()<=b->rel_posx - 64)
					{
						b->setX(b->rel_posx - 64);
						b->setState(0);
						//b->setVelocityX(0);
					}
				}

				if(!b->children.empty())
				{
					if(b->getState()==0)
					{
						b->children[0]->setCollidable(true);
						g_engine->coll_manager->setColl_box(b->children[0]);
						b->children.clear();
					}
				}
			}
			else if(b->getText()=="elevator")
			{
				//treba resetirat lift area, ako poginem u liftu da kolizija ostane normalna
				//treba bit cili unutra a ne samo tocka
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					if(editor->Player_managers[i]->p_player && b->getCollidable())
					{
						if(editor->Player_managers[i]->p_player->map_current == b->map_current)
						{
							if(g_engine->coll_manager->TestCollision_br(editor->Player_managers[i]->p_player,b))
							{
								if(editor->Player_managers[i]->control_action_prev && !editor->Player_managers[i]->control_action)
								{
									g_engine->audio->Play("pt1_strt");
									b->setCollidable(false);
									if(!b->children.empty())
									{
										for(unsigned int j=0;j<b->children.size();j++)
										{
											if(b->children[j]->getState()==-999 && (b->children[j]->getText()=="door_r" || b->children[j]->getText()=="door_l"))
											{
												b->children[j]->setState(1);
												b->children[j]->rel_posx = b->children[j]->getX();
											}
										}
									}
								}

								Sprite *temp= new Sprite();
								temp->setID(217);
								temp->setName("use");
								temp->setImage(editor->t_action);
								temp->setPosition(
									editor->Player_managers[i]->p_player->cx()-temp->hw(),
									editor->Player_managers[i]->p_player->getY()-1.5*temp->h());
								temp->setCollidable(false);
								temp->oneframelife = true;
								if(editor->Player_managers[i]->in_lift) temp->setRotation(g_engine->math->toRadians(180));
								g_engine->addSprite(temp,WORLD_FRONT_NON);

							}						
						}
					}
				}
			}
			else if(b->getArg(0)[0]=='&' && map_switch == false)
			{
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					if(editor->Player_managers[i]->p_player && b->getCollidable())
					{
						if(editor->Player_managers[i]->p_player->map_current == b->map_current)
						{
							Sprite *p = editor->Player_managers[i]->p_player;
							if(g_engine->coll_manager->TestCollision_br(editor->Player_managers[i]->p_player,b)
								|| g_engine->coll_manager->line_rectrect(p->getX(),p->getY(),p->w(),p->h(),b->getX(),b->getY(),b->w(),b->h()))
							{
								Sprite *temp= new Sprite();
								temp->setID(217);
								if(b->getArg(1)=="bluekey" || b->getArg(1)=="redkey")
									temp->setName(b->getArg(1));
								else
									temp->setName("enter");
								temp->setImage(editor->t_action);
								temp->setPosition(
									editor->Player_managers[i]->p_player->cx()-temp->hw(),
									editor->Player_managers[i]->p_player->getY()-1.5*temp->h());
								temp->setCollidable(false);
								temp->oneframelife = true;
								g_engine->addSprite(temp,WORLD_FRONT_NON);							

								if(editor->Player_managers[i]->control_action_prev && !editor->Player_managers[i]->control_action)
								{
									if(b->getArg(1)=="bluekey" || b->getArg(1)=="redkey")
									{
										if(editor->Player_managers[i]->getInventory(b->getArg(1)))
										{
											editor->Player_managers[i]->RemoveFromInventory(b->getArg(1));
											b->setText(b->getArg(0));
											g_engine->audio->Play("keyuse");
										}
										else
										{
											g_engine->audio->Play("keytry");
											return;
										}
									}


									//nesretno rjesenje
									g_engine->audio->StopAll();

									g_engine->map_current = g_engine->getMap(b->getText().substr(1,b->getText().size()-1));

									//da me ne iscrta dva puta
									editor->Player_managers[i]->p_player->map_current = g_engine->map_current;
									for(unsigned int j=0;j<editor->Player_managers[i]->p_player->skeleton.size();j++)
									{
										editor->Player_managers[i]->p_player->skeleton[j]->map_current = g_engine->map_current;
									}

									editor->Player_managers[i]->map_id = g_engine->map_current;
									editor->Player_managers[i]->p_player->setX(g_engine->game_maps[g_engine->map_current]->player_spawn[b->getID()].getX());
									editor->Player_managers[i]->p_player->setY(g_engine->game_maps[g_engine->map_current]->player_spawn[b->getID()].getY());
									map_switch = true;


									float down = 0;
									Sprite *a = editor->Player_managers[i]->p_player;

									for(int j=0;j<g_engine->game_maps[g_engine->map_current]->border_down_vec.size()-1;j++)
									{
										if((a->cx()>g_engine->game_maps[g_engine->map_current]->border_x_vec[j]) && (a->cx()<g_engine->game_maps[g_engine->map_current]->border_x_vec[j+1]))
										{
											float alpha = g_engine->math->AngleToTarget(
												g_engine->game_maps[g_engine->map_current]->border_x_vec[j],g_engine->game_maps[g_engine->map_current]->border_down_vec[j],
												g_engine->game_maps[g_engine->map_current]->border_x_vec[j+1],g_engine->game_maps[g_engine->map_current]->border_down_vec[j+1]);
											float delta_y = tan(alpha)*fabs(a->cx()-g_engine->game_maps[g_engine->map_current]->border_x_vec[j]);

											g_engine->game_maps[g_engine->map_current]->border_down = g_engine->game_maps[g_engine->map_current]->border_down_vec[j] + delta_y;
											editor->Player_managers[i]->screen->border_down =  g_engine->game_maps[g_engine->map_current]->border_down_vec[j] + delta_y;
										}
									}

									if(editor->Player_managers[i]->player_control==KEYBOARD_MOUSE)
									{
										editor->Player_managers[i]->screen->setOffset(
											a->cx(),a->getY()+a->h(),
											(float)((g_engine->getScreenWidth()/2-(g_engine->gui->cursor->cx()))*0.6),
											(float)((g_engine->getScreenHeight()/2-(g_engine->gui->cursor->cy()))*0.6));		
									}
									else
									{
										editor->Player_managers[i]->screen->setOffset(a->cx(),a->getY()+a->h(),0,0);
									}
								}
							}
						}
					}
				}				
			}

			break;

		case CANNONBALL:
		case GRANATA:			
			g_engine->coll_manager->collbox_update(b);
			player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);
			g_engine->coll_manager->TestForCollisions_br(b,ENEMIES);
			if (b->getMovementState()==FALL) SpinInAir(b,0.05);//not on ground
			b->setMovementState(FALL);
			if (abs(b->getVelocityY()<20))
				b->setVelocityY(b->getVelocityY()+0.5*g_engine->game_speed*g_engine->GetDeltaFix());
			if(b->fire_tick<3000/14) b->fire_tick += g_engine->game_speed*g_engine->GetDeltaFix();
			else
			{
				g_engine->audio->Play("ROCKLX1A");
				editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
				editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
				editor->Particle_manager->addFire(b->cx(),b->cy(),21);					
				b->setAlive(false);
			}
			break;

		case GRENADE_E:			
			g_engine->coll_manager->collbox_update(b);
			player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);
			g_engine->coll_manager->TestForCollisions_br(b,PLAYER);
			if (b->getMovementState()==FALL) SpinInAir(b,0.05);//not on ground
			b->setMovementState(FALL);
			if (abs(b->getVelocityY()<20))
				b->setVelocityY(b->getVelocityY()+0.5*g_engine->game_speed*g_engine->GetDeltaFix());
			if(b->fire_tick<3000/14) b->fire_tick += g_engine->game_speed*g_engine->GetDeltaFix();
			else
			{
				g_engine->audio->Play("ROCKLX1A");
				editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
				editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
				editor->Particle_manager->addFire(b->cx(),b->cy(),21);					
				b->setAlive(false);
			}
			break;		

		case BULLET:
		case SACMA:
		case ROCKET:
			if(b->getID()==RAILGUN) FadeAway(b,10);
			else if(b->getID()==CHAINGUN) FadeAway(b,25);

			if (b->getID()==BLASTER)
			{
				if(g_engine->math->Distance(b->getX(),b->getY(),b->rel_posx,b->rel_posy)>10)
				{
					editor->Particle_manager->addBulletTrail(b->getX(),b->getY(),0,1);
					b->rel_posx = b->getX();
					b->rel_posy = b->getY();
				}
			}

			if (b->getID()==R_LAUNCHER)
			{
				b->setVelocity(
					b->getVelocityX()+b->getVelocityX()*0.03*g_engine->game_speed*g_engine->GetDeltaFix(),
					b->getVelocityY()+b->getVelocityY()*0.03*g_engine->game_speed*g_engine->GetDeltaFix());
				g_engine->coll_manager->updateColl_line3(b);
				editor->Particle_manager->addRocketFlame(b);
			}
			else
				g_engine->coll_manager->updateColl_line3(b);

			g_engine->coll_manager->TestForCollisions_dot(b,ENEMIES);
			g_engine->coll_manager->TestForCollisions_dot(b,WORLD_FRONT_COLL);

			if(b->child)
			{
				b->child->setPosition(b->getX()-b->child->hw(),b->getY()-b->child->hh());	
			}
			break;

		case FT_FLAME:
			if (b->getScaleX()<1.5) b->setScale_C(b->getScaleX()+0.02*g_engine->game_speed*g_engine->GetDeltaFix());
			g_engine->coll_manager->updateColl_center(b);
			g_engine->coll_manager->TestForCollisions_dot(b,ENEMIES);
			g_engine->coll_manager->TestForCollisions_dot(b,WORLD_FRONT_COLL);			
			break;		

		case BIOMECH_LASER:
		case BIOMECH_ROCKET:
			if(b->getType()==BIOMECH_ROCKET)
			{
				b->setVelocity(
					b->getVelocityX()+b->getVelocityX()*0.02*g_engine->game_speed*g_engine->GetDeltaFix(),
					b->getVelocityY()+b->getVelocityY()*0.02*g_engine->game_speed*g_engine->GetDeltaFix());
				g_engine->coll_manager->collbox_update(b);

				if(g_engine->math->Distance(b->getX(),b->getY(),b->rel_posx,b->rel_posy)>10)
				{
					editor->Particle_manager->addRocketFlame(b);
					b->rel_posx = b->getX();
					b->rel_posy = b->getY();
				}
			}

			if(b->getArg(0) == "orangetrail")
			{
				if(g_engine->math->Distance(b->getX(),b->getY(),b->rel_posx,b->rel_posy)>10)
				{
					editor->Particle_manager->addBulletTrail(b->getX(),b->getY(),0,1);
					b->rel_posx = b->getX();
					b->rel_posy = b->getY();
				}
			}

			g_engine->coll_manager->collbox_update(b);	
			//ne bi trebalo jer se provjerava kod update igraca !!!
			g_engine->coll_manager->TestForCollisions_dot(b,0,PLAYER);//kod igraca se provjerava br, dva puta ali sta ces !?
			g_engine->coll_manager->TestForCollisions_dot(b,WORLD_FRONT_COLL);
			break;

			///////////////
			//  ENEMIES  //
			///////////////

		case BESHTIYA://flipper mora biti povezan s vodom
			g_engine->coll_manager->updateColl_box(b);
			g_engine->coll_manager->TestForCollisions_br(b,0,PLAYER);
			player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);			
			editor->Beshtiya_manager->Update(b,editor->Player_managers[0]->p_player,editor->Player_managers[1]->p_player);
			if(!b->children.empty())
			{
				//l
				if(b->getX() < b->children[0]->getX()) b->setX(b->children[0]->getX()); 
				//r
				if(b->getX()+b->w() > b->children[0]->getX()+b->children[0]->w()) b->setX(b->children[0]->getX()+b->children[0]->w()-b->w());
				//u
				if(b->getY() < b->children[0]->getY()) b->setY(b->children[0]->getY());
				//d
				if(b->getY()+b->h() > b->children[0]->getY()+b->children[0]->h()) b->setY(b->children[0]->getY()+b->children[0]->h()-b->h());
			}

			if(b->energy<=0 && !b->children.empty())
			{
				for(int i=0;i<b->children.size();i++) b->children[i]->setAlive(false);
				b->children.clear();
			}			
			break;

			/*case GNAAR:			
			b->jump = 0;
			g_engine->coll_manager->updateColl_box(b);
			g_engine->coll_manager->TestForCollisions_br(b,0,PLAYER);
			player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);

			if(b->jump == 1 && b->getMovementState()==WALK) b->setVelocityY(b->getVelocityY()-5);
			if(b->jump == 2 && b->getMovementState()==WALK) b->setVelocityY(b->getVelocityY()-7);

			//update
			if(b->energy>0) b->setMovementState(FALL);
			if (abs(b->getVelocityY()<15))
			b->setVelocityY(b->getVelocityY()+0.3*g_engine->game_speed*(g_engine->GetDeltaFix()));
			editor->GnaarM_manager->Update(b,editor->Player_managers[0]->p_player,editor->Player_managers[1]->p_player);
			break;*/

		case BEHEADED:	
			b->jump = 0;

			g_engine->coll_manager->updateColl_box(b);
			g_engine->coll_manager->TestForCollisions_br(b,0,PLAYER);
			player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);
			//update
			if(b->getMovementState()!=FALL && b->getState()==0)
			{				
				editor->Beheaded_manager->DontFall(b);
			}
			g_engine->coll_manager->updateColl_box(b);
			editor->Beheaded_manager->Update(b,editor->Player_managers[0]->p_player,editor->Player_managers[1]->p_player);

			if(b->jump == 1 && b->getMovementState()==WALK) b->setVelocityY(b->getVelocityY()-5);
			if(b->jump == 2 && b->getMovementState()==WALK) b->setVelocityY(b->getVelocityY()-7);

			if(b->energy>0) b->setMovementState(FALL);
			else if(!b->children.empty())
			{
				for(int i=0;i<b->children.size();i++) b->children[i]->setAlive(false);
				b->children.clear();
			}

			if (abs(b->getVelocityY()<15))
				b->setVelocityY(b->getVelocityY()+0.3*g_engine->game_speed*(g_engine->GetDeltaFix()));			
			break;

		case BIOMECH:	
			g_engine->coll_manager->updateColl_box(b);
			player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);

			if(b->getMovementState()!=FALL && b->getState()==0)
			{				
				editor->Beheaded_manager->DontFall(b);
			}
			//update			
			editor->Biomech_manager->Update(b,editor->Player_managers[0]->p_player,editor->Player_managers[1]->p_player);
			if(b->energy>0) b->setMovementState(FALL);
			else if(!b->children.empty())
			{
				for(int i=0;i<b->children.size();i++) b->children[i]->setAlive(false);
				b->children.clear();
			}
			if (abs(b->getVelocityY()<15))
				b->setVelocityY(b->getVelocityY()+0.3*g_engine->game_speed*(g_engine->GetDeltaFix()));
			break;

		case GIBS:
		case SHELL:			
			g_engine->coll_manager->collbox_update(b);
			player_collision->PlatformPlayerCollision(b,WORLD_FRONT_COLL);
			if (b->getMovementState()==FALL)
			{
				SpinInAir(b,0.05);//not on ground
				//padne framerate, treba vidit u koji layer se stavi ima li nekih provjera kolizija
				if(b->getArg(0)=="gib")
				{
					if(g_engine->math->Distance(b->getX(),b->getY(),b->rel_posx,b->rel_posy)>15)
					{
						editor->Particle_manager->addBloodBig(b->getX(),b->getY(),11);
						b->rel_posx = b->getX();
						b->rel_posy = b->getY();
					}
				}				
			}
			b->setMovementState(FALL);
			if (abs(b->getVelocityY()<20))
				b->setVelocityY(b->getVelocityY()+0.5*g_engine->game_speed*g_engine->GetDeltaFix());
			FadeAway(b,0.75);
			break;

		case KRV:		
			if (b->getMovementState()==FALL)
			{
				SpinInAir(b,0.05);
				if (abs(b->getVelocityY()<24))
				{
					b->setVelocityY(b->getVelocityY()+0.5*0.35*g_engine->game_speed*g_engine->GetDeltaFix());
				}
				b->setScale_C(b->getScaleX()+0.05*(double)g_engine->game_speed*g_engine->GetDeltaFix());				
				g_engine->coll_manager->updateColl_center(b);
				g_engine->coll_manager->TestForCollisions_dot(b,WORLD_FRONT_COLL);
				FadeAway(b,10);
			}
			break;

		case DIM:
			b->setScale_C(b->getScaleX()+0.02*(double)g_engine->game_speed*g_engine->GetDeltaFix());
			FadeAway(b,10);
			break;

		case FIRE:
			b->setScale_C(b->getScaleX()+0.02*(double)g_engine->game_speed*g_engine->GetDeltaFix());
			FadeAway(b,10);
			GoRed(b,10);
			break;

		case SPARK:			
			if(b->getMovementState()==FALL)
			{
				if (abs(b->getVelocityY()<5)) b->setVelocityY(b->getVelocityY()+0.15*g_engine->game_speed*g_engine->GetDeltaFix());				
			}
			if(b->getArg(0)=="bullet") FadeAway(b,15);
			else FadeAway(b,10);
			break;

		case EXP:
			if(!b->just_created)
			{
				g_engine->coll_manager->TestForCollisions_br(b,WORLD_FRONT_COLL);
				g_engine->coll_manager->TestForCollisions_br(b,ENEMIES);
				g_engine->coll_manager->TestForCollisions_br(b,0,PLAYER);
				b->setAlive(false);
			}
			break;

		case BESHTIYA_LASER:FadeAway(b,25);break;

		}//switch
	}//PLAY,GAMEOVER

	//stvara probleme kod prijelaza nivoa, brisalo spriteove
	if(map_switch==false)
	{
		if(b->getY()>g_engine->game_maps[g_engine->map_current]->map_bottom+10000)
		{
			b->setAlive(false);
			if(b->child) b->child->setAlive(false);
			for(unsigned int i=0;i<b->skeleton.size();i++) b->skeleton[i]->setAlive(false);
		}
	}

}//game_spriteUpdate()

void game_spriteErase(Sprite* a)
{
	//treba stavit da se javi samo tokom igre kad pogodi nesto !!!
	//A NE KAD JE SVE GOOVO I BRISU SE SVI SPRITEOVI
	if(g_engine->getEngineState()==GAME && g_engine->getGameState()==PLAY)
	{
		if(a->getClass()==PLAYER_WEAPON || a->getClass()==ENEMY)
		{
			if (a->getType()==ROCKET || a->getType()==FT_FLAME || a->getType()==BULLET)
			{
				if (a->child!=NULL) a->child->setAlive(false);//ja mislim zbog svijetla			
			}

			if (a->getID()==G_LAUNCHER || a->getType()==GRANATA || a->getID()==GRENADE_E || a->getID()==R_LAUNCHER || a->getType()==ROCKET)
			{
				editor->Particle_manager->addExplosionCollision(a->cx(),a->cy(),150);
				return;
			}			

			if ((a->getType()==BEHEADED && a->getID()==KAMIKAZE) || (a->getType()==BESHTIYA && a->getID()==FLYER))
			{
				editor->Particle_manager->addExplosionCollision(a->coll_box[1].getX(),a->coll_box[1].getY(),100);
				return;
			}			
		}

		if (a->getType()==PLATFORMA && a->getText()=="explode")
		{
			//g_engine->audio->Play("jump1");
			editor->Particle_manager->addExplosionCollision(a->cx(),a->cy(),150);
			return;
		}

		if (a->getType()==KAMIKAZE)
		{
			if(a->sound!=NULL)
			{
				a->sound->stop();
				a->sound = NULL;
			}
		}
	}
}

bool game_CursorCollision(Sprite* cursor, Sprite* a)
{
	if(a->getClass()==-999 && a->getType()==-999 && a->getID()==-999) return true;

	if (g_engine->p_input->GetKeyState(DIK_DELETE)&0x80)
	{
		a->setAlive(false);
		if(a==g_engine->gui->getSelectedSprite()) g_engine->gui->setSelectedSprite(NULL);
		return true;
	}

	if (g_engine->p_input->GetKeyState(DIK_INSERT)&0x80)
	{
		if(g_engine->gui->getSelectedSprite())
		{
			g_engine->gui->getSelectedSprite()->children.push_back(a);	
			g_engine->gui->setSelectedSprite(NULL);//da ne prihvati vise izbora
		}
		return true;
	}	

	switch (a->getType())
	{
	default:
		if(g_engine->getEngineState()==ANIMATION)
		{
			if(animator->sprite!=NULL)
			{
				if(a==animator->sprite)
				{
					if (g_engine->gui->getSelectedSprite()==NULL)
					{
						g_engine->gui->setSelectedSprite(a);
						cursor->rel_posx = cursor->getX() - g_engine->getLayerOffsetX(g_engine->gui->getSelectedSprite()->getLayer()) - a->getX();
						cursor->rel_posy = cursor->getY() - g_engine->getLayerOffsetY(g_engine->gui->getSelectedSprite()->getLayer()) - a->getY();
						return true;
					}
				}
			}
		}
		else if (g_engine->gui->getSelectedSprite()==NULL)
		{
			g_engine->gui->setSelectedSprite(a);
			cursor->rel_posx = cursor->getX() - g_engine->getLayerOffsetX(g_engine->gui->getSelectedSprite()->getLayer()) - a->getX();
			cursor->rel_posy = cursor->getY() - g_engine->getLayerOffsetY(g_engine->gui->getSelectedSprite()->getLayer()) - a->getY();
			if (g_engine->getEngineState()==EDITOR)
				editor->edit_sprite->Update_form(a);
			return true;
		}		
		break;
	}

	return false;

}//game_cursorCollision()

bool IHitAPlatform(Advanced2D::Sprite* a, Advanced2D::Sprite* b,int side)
{
	switch(a->getClass())
	{
	case ENEMY:		
		{
			switch(b->getType())
			{
			case PLATFORMA:	
				switch (a->getType())
				{
				case GNAAR:editor->GnaarM_manager->HitPlatform(a,side,b);return true;break;
				case BIOMECH:editor->Biomech_manager->HitPlatform(a,side,b);return true;break;
				case BEHEADED:editor->Beheaded_manager->HitPlatform(a,side,b);return true;break;
				}
				break;
			}
		}
		break;
	}


	switch(a->getType())
	{
	case PLATFORMA:
		{
			switch(b->getType())
			{
			case IGRAC_1:
				if(side==300)
				{
					b->setMovementState(WALK);
					if(editor->Player_managers[b->getID()]->nocontrol) b->setVelocityX(0);
					editor->Player_managers[b->getID()]->nocontrol = false;
				}
				if(a->getText()=="lift")
				{
					//g_engine->audio->Play("jump1");
					if(a->getY()>a->getState()) a->setVelocityY(-1);
					else
					{
						if(a->getVelocityY()<0) g_engine->audio->Play("pt1_end");
						a->setVelocityY(0);
					}
				}
				return true;
				break;

			case CANNONBALL:
				if(a->getState()==-575) a->setAlive(false);				
				return true;
				break;

			case PLATFORMA:
				{
					if(b->getText()=="stop")
					{
						a->setVelocity(0,0);
						for(unsigned int i=0;i<a->children.size();i++)
						{
							a->children[i]->setVelocity(0,0);
						}
						g_engine->audio->Play("train2");
						return true;
					}
					else if(b->getText()=="u_r")
					{
						a->setVelocity(2,0);
						g_engine->audio->Play("train2");
						return true;
					}
					else if(b->getText()=="u_l")
					{
						a->setVelocity(-2,0);
						g_engine->audio->Play("train2");
						return true;
					}
					else if(b->getText()=="l_d")
					{
						a->setVelocity(0,2);
						g_engine->audio->Play("train2");
						return true;
					}
					else if(b->getText()=="r_d")
					{
						a->setVelocity(0,2);
						g_engine->audio->Play("train2");
						return true;
					}
				}
				break;
			}
		}
		break;
	}	

	return false;
}

bool game_spriteCollision(Advanced2D::Sprite* a, Advanced2D::Sprite* b)
{
	g_engine->collision_counter++;	

	if (a->getAlive()==false || b->getAlive()==false) return true;

	Sprite *temp_sprite = NULL;

	if (g_engine->getGameState()==PLAY || g_engine->getGameState()==GAMEOVER)
	{
		switch (a->getType())
		{
		case BESHTIYA:
			switch (b->getClass())
			{
			case PLAYER_WEAPON:
				a->energy -= b->energy;
				//if(a->getState()==0) a->setState(10);
				if(b->getArg(0)!="railgun") b->setAlive(false);

				if (b->getID()==G_LAUNCHER || b->getID()==R_LAUNCHER || b->getType()==GRANATA)
				{					
					g_engine->audio->Play("ROCKLX1A");

					float x,y;
					if(b->getID()==R_LAUNCHER) {x = b->coll_box[1].getX(); y = b->coll_box[1].getY();}
					else {x = b->cx(); y = b->cy();}

					editor->Particle_manager->addSmoke(x,y,21);
					editor->Particle_manager->addBloodBig(x,y,21);
					editor->Particle_manager->addSpark(x,y,21);
					editor->Particle_manager->addFire(x,y,21);	
				}
				else
				{
					bullethitskeleton_box(a,b);
				}

				if (a->energy <=0 )
				{
					if(a->getID()==FLYER)
					{
						g_engine->audio->Play("ROCKLX1A");
						editor->Particle_manager->addSmoke(a->cx(),a->cy(),21);
						editor->Particle_manager->addBloodBig(a->cx(),a->cy(),21);
						editor->Particle_manager->addSpark(a->cx(),a->cy(),21);
						editor->Particle_manager->addFire(a->cx(),a->cy(),21);
						a->setAlive(false);
						for(unsigned int i=0;i<a->skeleton.size();i++) a->skeleton[i]->setAlive(false);
						if(rand()%2==0) g_engine->audio->Play("Flydeth1");
						return true;
					}
					else if(a->getID()==FLIPPER)
					{
						editor->Particle_manager->addBloodBig(a->cx(),a->cy(),15);
						GibMe(a,'S',b,2,'F');
						if(rand()%2==0) g_engine->audio->Play("Flpdeth1");
						return true;
					}

					a->setAlive(false);
					if(!a->children.empty())
					{
						for(int i=0;i<a->children.size();i++) a->children[i]->setAlive(false);
						a->children.clear();
					}	
				}
				break;
			}
			return true;
			break;

		case EXP:
			switch(b->getType())
			{
			case GIBS:
			case IGRAC_1:
				//if(b->energy <= 0) return true;
				{
					if(g_engine->math->Distance(a->cx(),a->cy(),b->cx(),b->cy())<75) b->energy -= 15;
					else if(g_engine->math->Distance(a->cx(),a->cy(),b->cx(),b->cy())<125) b->energy -= 10;
					else if(g_engine->math->Distance(a->cx(),a->cy(),b->cx(),b->cy())<175) b->energy -= 5;

					if(b->getType()==IGRAC_1 && b->energy<=0) KillPlayer(b->getID(),b,a,'B',5);

					float d = g_engine->math->Distance(a->cx(),a->cy(),b->cx(),b->cy());
					if(d<=150)
					{
						if(b->getType()==IGRAC_1) editor->Player_managers[b->getID()]->nocontrol = true;

						//za gibs ne treba cx nego getx !!!
						b->setVelocity(
							b->getVelocityX()+g_engine->math->LinearVelocityX(ExplosionAngleCenter(b,a))*8,
							b->getVelocityY()+g_engine->math->LinearVelocityY(ExplosionAngleCenter(b,a))*8
							);

						if(b->getVelocityX()>30) b->setVelocityX(30);
						if(b->getVelocityX()<-30) b->setVelocityX(-30);
						if(b->getVelocityY()>30) b->setVelocityY(30);
						if(b->getVelocityY()<-30) b->setVelocityY(-30);
					}
				}
				return true;
				break;

			case BESHTIYA:			
				{
					if(g_engine->math->Distance(a->cx(),a->cy(),b->cx(),b->cy())<50) b->energy -= 5;
					else if(g_engine->math->Distance(a->cx(),a->cy(),b->cx(),b->cy())<100) b->energy -= 2;
					else if(g_engine->math->Distance(a->cx(),a->cy(),b->cx(),b->cy())<150) b->energy -= 1;

					if (b->energy <=0 )
					{
						if(b->getID()==FLYER)
						{
							g_engine->audio->Play("ROCKLX1A");
							editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
							editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
							editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
							editor->Particle_manager->addFire(b->cx(),b->cy(),21);
							b->setAlive(false);
							for(unsigned int i=0;i<b->skeleton.size();i++) b->skeleton[i]->setAlive(false);
							if(rand()%2==0) g_engine->audio->Play("Flydeth1");
							return true;
						}
						else if(b->getID()==FLIPPER)
						{
							editor->Particle_manager->addBloodBig(b->cx(),b->cy(),15);
							GibMe(a,'S',b,5,'B');
							if(rand()%2==0) g_engine->audio->Play("Flpdeth1");
							return true;
						}

						b->setAlive(false);
						if(!b->children.empty())
						{
							for(int i=0;i<b->children.size();i++) b->children[i]->setAlive(false);
							b->children.clear();
						}	
					}
				}
				return true;
				break;

			case PLATFORMA:
				if(b->getText()=="explode")
				{
					g_engine->audio->Play("ROCKLX1A");
					editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
					editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
					editor->Particle_manager->addFire(b->cx(),b->cy(),21);					
					if(b->getLayer()==WORLD_FRONT_COLL) RemoveFromSector(b);
					b->setAlive(false);
				}
				else if(b->getText()=="glass" || b->getText()=="break")
				{
					BreakGlass2(b,24,24,a->cx(),a->cy());
					b->setAlive(false);
					if(b->getText()=="glass") g_engine->audio->Play("brkglas");
					if(b->getLayer()==WORLD_FRONT_COLL) RemoveFromSector(b);
				}
				return true;
				break;

			case BEHEADED:
			case GNAAR:
				if(b->energy <= 0) return true;
				{
					float d = g_engine->math->Distance(a->cx(),a->cy(),b->cx(),b->cy());

					if(d>=200)
					{
						b->energy -= 1;

						if(b->energy <= 0)
						{
							if (b->getID()==KAMIKAZE)
							{
								GibMe(b,'S',b,10,'B');
								b->setAlive(false);//parents, child ???
								b->energy = 0;
								g_engine->audio->Play("ROCKLX1A");
								editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
								editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
								editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
								editor->Particle_manager->addFire(b->cx(),b->cy(),21);						
								return true;
							}
							else
							{
								b->energy = 0;
								b->frame_tick[0]=0;
								b->setMovementState(FALL);
								b->setFrameCurrent(0);

								float alpha = g_engine->math->AngleToTarget(a->cx(),a->cy(),b->cx(),b->cy());								

								b->setVelocity(
									g_engine->math->LinearVelocityX(g_engine->math->toDegrees(alpha))*1,
									g_engine->math->LinearVelocityY(g_engine->math->toDegrees(alpha))*1);

								if((b->flip_h && (a->cx()<b->cx())) || (!b->flip_h && (a->cx()>b->cx()))) b->setName("front");
								else b->setName("back");

								b->animation_name = "fly";
								return true;
							}
						}
					}

					if(d<150)
					{
						if (b->getID()==KAMIKAZE)
						{
							GibMe(b,'S',b,10,'B');
							b->setAlive(false);//parents, child ???
							b->energy = 0;
							g_engine->audio->Play("ROCKLX1A");
							editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
							editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
							editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
							editor->Particle_manager->addFire(b->cx(),b->cy(),21);						
							return true;
						}
						else
						{
							GibMe(b,'S',a,10,'B');
							b->energy = 0;
						}
					}
					else if(d<200)
					{
						if (b->getID()==KAMIKAZE)
						{
							GibMe(b,'S',b,10,'B');
							b->setAlive(false);//parents, child ???
							b->energy = 0;
							g_engine->audio->Play("ROCKLX1A");
							editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
							editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
							editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
							editor->Particle_manager->addFire(b->cx(),b->cy(),21);						
							return true;
						}
						else
						{
							b->energy = 0;
							b->frame_tick[0]=0;
							b->setMovementState(FALL);
							b->setFrameCurrent(0);

							float alpha = g_engine->math->AngleToTarget(a->cx(),a->cy(),b->cx(),b->cy());
							float f = 1-(float)d/a->hw();

							b->setVelocity(
								g_engine->math->LinearVelocityX(g_engine->math->toDegrees(alpha))*10*f,
								g_engine->math->LinearVelocityY(g_engine->math->toDegrees(alpha))*10*f);
							if((b->flip_h && (a->cx()<b->cx())) || (!b->flip_h && (a->cx()>b->cx()))) b->setName("front");
							else b->setName("back");
							b->animation_name = "fly";
						}
					}
				}

				if(!b->getAlive() && !b->children.empty())
				{
					for(int i=0;i<b->children.size();i++) b->children[i]->setAlive(false);
					b->children.clear();
				}	
				return true;
				break;				
			}
			break;	

		case BEHEADED:			
		case GNAAR:
			switch (b->getClass())
			{
			case PLAYER_WEAPON:
				if (a->energy <=0 ) return true;
				else
				{
					//if(a->getState()==0) a->setState(10);
					a->energy -= b->energy;						
				}

				if (b->getID()==G_LAUNCHER || b->getID()==R_LAUNCHER || b->getType()==GRANATA)
				{
					GibMe(a,'S',b,10,'B');
					if(b->getArg(0)!="railgun") b->setAlive(false);					
					g_engine->audio->Play("ROCKLX1A");
					editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
					editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
					editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
					editor->Particle_manager->addFire(b->coll_box[1].getX(),b->coll_box[1].getY(),21);

					if(a->energy<=0)
					{
						a->setAlive(false);
						if(!a->children.empty())
						{
							for(int i=0;i<a->children.size();i++) a->children[i]->setAlive(false);
							a->children.clear();
						}

						if (a->getID()==KAMIKAZE)
						{
							GibMe(a,'S',a,10,'B');
							if(b->getArg(0)!="railgun") b->setAlive(false);
							a->energy -= 20;
							g_engine->audio->Play("ROCKLX1A");
							editor->Particle_manager->addSmoke(a->cx(),a->cy(),21);
							editor->Particle_manager->addBloodBig(a->cx(),a->cy(),21);
							editor->Particle_manager->addSpark(a->cx(),a->cy(),21);
							editor->Particle_manager->addFire(a->cx(),a->cy(),21);						
							return true;
						}

						return true;
					}					
				}

				bullethitskeleton_box(b,a);
				if(b->getArg(0)!="railgun") b->setAlive(false);

				if ( a->energy<=0 )
				{
					if (a->getID()==KAMIKAZE)
					{
						GibMe(a,'S',a,10,'B');
						if(b->getArg(0)!="railgun") b->setAlive(false);
						a->energy -= 20;
						g_engine->audio->Play("ROCKLX1A");
						editor->Particle_manager->addSmoke(a->cx(),a->cy(),21);
						editor->Particle_manager->addBloodBig(a->cx(),a->cy(),21);
						editor->Particle_manager->addSpark(a->cx(),a->cy(),21);
						editor->Particle_manager->addFire(a->cx(),a->cy(),21);						
						return true;
					}

					if(b->getArg(0)=="railgun")
					{
						GibMe(a,'S',a,10,'B');
						return true;
					}

					a->frame_tick[0]=0;
					a->setMovementState(FALL);
					a->setFrameCurrent(0);
					a->setVelocity((double)b->getVelocityX()/6,-3);

					if((a->flip_h && b->getVelocityX()<0) || (!a->flip_h && b->getVelocityX()>0))
						a->setName("back");
					else
						a->setName("front");

					a->animation_name = "fly";					

					if(a->getType()==BEHEADED)
					{
						if(a->getID()==ENFORCER)
						{
							//digod da ne bude nista
							switch(rand()%3)
							{
							case 0:g_engine->audio->Play("INFDETH1");break;
							case 1:g_engine->audio->Play("INFDETH2");break;
							}							
						}
						else if(a->getID()==PARASITE)
						{
							g_engine->audio->Play("Pardeth1");
						}
						else if(a->getID()==GUNNER)
						{
							g_engine->audio->Play("death1");
						}
						else
						{
							switch(rand()%4)
							{
							case 0:g_engine->audio->Play("SOLDETH1");break;
							case 1:g_engine->audio->Play("SOLDETH2");break;
							case 2:g_engine->audio->Play("SOLDETH3");break;
							}
						}

					}
					return true;					
				}		
				return true;
				break;
			}
			break;

		case BIOMECH:		
			switch (b->getClass())
			{
			case PLAYER_WEAPON:	
				if(a->energy<=0)
					return true;
				else
					a->energy -= b->energy;

				bullethitskeleton_box(b,a);

				if(b->getArg(0)!="railgun") b->setAlive(false);

				if (b->getID()==G_LAUNCHER || b->getID()==R_LAUNCHER || b->getType()==GRANATA)
				{
					if(b->getArg(0)!="railgun") b->setAlive(false);
					g_engine->audio->Play("ROCKLX1A");
					editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
					editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
					editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
					editor->Particle_manager->addFire(b->coll_box[1].getX(),b->coll_box[1].getY(),21);					
					return true;
				}

				if ( a->energy<=0 )
				{
					a->frame_tick[0]=0;
					a->setMovementState(STANDSTILL);
					a->setFrameCurrent(0);
					a->setVelocity(0,0);
					g_engine->audio->Play("SetacPlaviDeath");
					a->animation_name = "fly";
					return true;
				}

				break;
			}
			return true;
			break;//biomech_major

		}

		switch (a->getType())
		{
		case IGRAC_1:
			switch (b->getType())
			{
			case BEHEADED:				
				if (b->getID()==KAMIKAZE && b->energy>0)
				{
					GibMe(b,'S',b,10,'B');
					b->setAlive(false);//parents, child ???
					a->energy -= 20;
					g_engine->audio->Play("ROCKLX1A");
					editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
					editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
					editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
					editor->Particle_manager->addFire(b->coll_box[1].getX(),b->coll_box[1].getY(),21);					
					return true;					
				}
				return true;
				break;

			case BIOMECH_LASER:				
				a->energy -= b->energy;
				bullethitskeleton_box(b,a);
				b->setAlive(false);				
				if (a->energy<=0) KillPlayer(a->getID(),a,b,'F',1);
				return true;
				break;

			case BIOMECH_ROCKET:
				//blow away the player
				a->setVelocityX(g_engine->math->LinearVelocityX(ExplosionAngleCenter(a,b))*6);
				a->setVelocityY(g_engine->math->LinearVelocityY(ExplosionAngleCenter(a,b))*6);
				a->setMovementState(OUTOFCONTROL);
				if(a->getType()==IGRAC_1) editor->Player_managers[a->getID()]->nocontrol = true;
				//
				a->energy -= b->energy;
				b->setAlive(false);
				g_engine->audio->Play("ROCKLX1A");
				editor->Particle_manager->addSmoke(b->coll_box[1].getX(),b->coll_box[1].getY(),21);
				editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
				editor->Particle_manager->addSpark(b->coll_box[1].getX(),b->coll_box[1].getY(),21);
				editor->Particle_manager->addFire(b->coll_box[1].getX(),b->coll_box[1].getY(),21);
				if (a->energy<=0) KillPlayer(a->getID(),a,b,'B',8);
				return true;
				break;

			case GRENADE_E:
				//blow away the player
				a->setVelocityX(g_engine->math->LinearVelocityX(ExplosionAngleCenter(a,b))*3);
				a->setVelocityY(g_engine->math->LinearVelocityY(ExplosionAngleCenter(a,b))*3);
				a->setMovementState(OUTOFCONTROL);
				if(a->getType()==IGRAC_1) editor->Player_managers[a->getID()]->nocontrol = true;
				//
				a->energy -= b->energy;
				b->setAlive(false);
				g_engine->audio->Play("ROCKLX1A");
				editor->Particle_manager->addSmoke(b->cx(),b->cy(),21);
				editor->Particle_manager->addBloodBig(b->cx(),b->cy(),21);
				editor->Particle_manager->addSpark(b->cx(),b->cy(),21);
				editor->Particle_manager->addFire(b->cx(),b->cy(),21);
				if (a->energy<=0) KillPlayer(a->getID(),a,b,'B',8);
				return true;
				break;		
			}
			break;//case IGRAC_1

		case PLATFORMA:
			switch (b->getType())
			{
			case IGRAC_1:
				{
					std::string temp;
					std::stringstream stream(a->getText());
					while(stream >> temp)
					{
						if(temp=="health2")
						{
							b->energy += 2;
							a->setAlive(false);
							g_engine->audio->Play("s_health");
						}
						else if(temp=="health10")
						{
							if(b->energy>=100) return true;
							else
							{
								b->energy += 10;
								a->setAlive(false);
								if(b->energy>=100) b->energy = 100;
								g_engine->audio->Play("m_health");
							}
						}
						else if(temp=="health25")
						{
							if(b->energy>=100) return true;
							else
							{
								b->energy += 25;
								a->setAlive(false);
								if(b->energy>=100) b->energy = 100;
								g_engine->audio->Play("m_health");
							}
						}
						else if(temp=="armor2")
						{
							editor->Player_managers[b->getID()]->armor += 2;
							a->setAlive(false);
							g_engine->audio->Play("ar2_pkup");
						}
						else if(temp=="armor25")
						{
							if(editor->Player_managers[b->getID()]->armor>=100) return true;
							else
							{
								editor->Player_managers[b->getID()]->armor += 25;
								a->setAlive(false);
								if(editor->Player_managers[b->getID()]->armor>=100) editor->Player_managers[b->getID()]->armor = 100;
								g_engine->audio->Play("ar1_pkup");
							}
						}
						else if(temp=="shotgun")
						{
							editor->Player_managers[b->getID()]->player_weapon = SHOTGUN;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[1] += 10;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[1] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[1] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[1])
								editor->Player_managers[b->getID()]->hud->ammo_amount[1] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[1];
						}
						else if(temp=="s_shotgun")
						{
							editor->Player_managers[b->getID()]->player_weapon = S_SHOTGUN;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[1] += 10;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[2] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[1] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[1])
								editor->Player_managers[b->getID()]->hud->ammo_amount[1] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[1];
						}
						else if(temp=="machinegun")
						{
							editor->Player_managers[b->getID()]->player_weapon = MACHINEGUN;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[2] += 50;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[3] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[2] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[2])
								editor->Player_managers[b->getID()]->hud->ammo_amount[2] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[2];
						}
						else if(temp=="chaingun")
						{
							editor->Player_managers[b->getID()]->player_weapon = CHAINGUN;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[2] += 50;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[4] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[2] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[2])
								editor->Player_managers[b->getID()]->hud->ammo_amount[2] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[2];
						}
						else if(temp=="g_launcher")
						{
							editor->Player_managers[b->getID()]->player_weapon = G_LAUNCHER;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[3] += 10;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[5] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[3] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[3])
								editor->Player_managers[b->getID()]->hud->ammo_amount[3] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[3];
						}
						else if(temp=="r_launcher")
						{
							editor->Player_managers[b->getID()]->player_weapon = R_LAUNCHER;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[4] += 10;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[6] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[4] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[4])
								editor->Player_managers[b->getID()]->hud->ammo_amount[4] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[4];
						}
						else if(temp=="hyperblaster")
						{
							editor->Player_managers[b->getID()]->player_weapon = HYPERBLASTER;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[5] += 50;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[7] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[5] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[5])
								editor->Player_managers[b->getID()]->hud->ammo_amount[5] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[5];
						}
						else if(temp=="railgun")
						{
							editor->Player_managers[b->getID()]->player_weapon = RAILGUN;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[6] += 10;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[8] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[6] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[6])
								editor->Player_managers[b->getID()]->hud->ammo_amount[6] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[6];
						}
						else if(temp=="bfg")
						{
							editor->Player_managers[b->getID()]->player_weapon = BFG;
							editor->Player_managers[b->getID()]->setWeaponTexture();
							editor->Player_managers[b->getID()]->hud->ammo_amount[7] += 100;
							editor->Player_managers[b->getID()]->hud->weapon_unlocked[9] = true;
							a->setAlive(false);
							g_engine->audio->Play("w_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[7] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[7])
								editor->Player_managers[b->getID()]->hud->ammo_amount[7] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[7];
						}
						else if(temp=="shells")
						{
							editor->Player_managers[b->getID()]->hud->ammo_amount[1] += 10;
							a->setAlive(false);
							g_engine->audio->Play("am_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[1] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[1])
								editor->Player_managers[b->getID()]->hud->ammo_amount[1] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[1];
						}
						else if(temp=="bullets")
						{
							editor->Player_managers[b->getID()]->hud->ammo_amount[2] += 50;
							a->setAlive(false);
							g_engine->audio->Play("am_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[2] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[2])
								editor->Player_managers[b->getID()]->hud->ammo_amount[2] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[2];
						}
						else if(temp=="grenades")
						{
							editor->Player_managers[b->getID()]->hud->ammo_amount[3] += 10;
							a->setAlive(false);
							g_engine->audio->Play("am_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[3] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[3])
								editor->Player_managers[b->getID()]->hud->ammo_amount[3] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[3];
						}
						else if(temp=="rockets")
						{
							editor->Player_managers[b->getID()]->hud->ammo_amount[4] += 10;
							a->setAlive(false);
							g_engine->audio->Play("am_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[4] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[4])
								editor->Player_managers[b->getID()]->hud->ammo_amount[4] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[4];
						}
						else if(temp=="cells")
						{
							editor->Player_managers[b->getID()]->hud->ammo_amount[5] += 25;
							a->setAlive(false);
							g_engine->audio->Play("am_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[5] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[5])
								editor->Player_managers[b->getID()]->hud->ammo_amount[5] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[5];
						}
						else if(temp=="slugs")
						{
							editor->Player_managers[b->getID()]->hud->ammo_amount[6] += 10;
							a->setAlive(false);
							g_engine->audio->Play("am_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[6] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[6])
								editor->Player_managers[b->getID()]->hud->ammo_amount[6] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[6];
						}
						else if(temp=="bfgs")
						{
							editor->Player_managers[b->getID()]->hud->ammo_amount[7] += 100;
							a->setAlive(false);
							g_engine->audio->Play("am_pkup");
							if(editor->Player_managers[b->getID()]->hud->ammo_amount[7] > editor->Player_managers[b->getID()]->hud->ammo_amount_max[7])
								editor->Player_managers[b->getID()]->hud->ammo_amount[7] = editor->Player_managers[b->getID()]->hud->ammo_amount_max[7];
						}
						else if(temp=="redkey")
						{
							editor->Player_managers[b->getID()]->inventory.push_back("redkey");
							a->setAlive(false);
							g_engine->audio->Play("pkup");
						}
						else if(temp=="bluekey")
						{
							editor->Player_managers[b->getID()]->inventory.push_back("bluekey");
							a->setAlive(false);
							g_engine->audio->Play("pkup");
						}
						else if(temp=="bullettime")
						{
							editor->Player_managers[b->getID()]->inventory.push_back("bullettime");
							a->setAlive(false);
							g_engine->audio->Play("pkup");
						}
						else if(temp=="fly")
						{
							editor->Player_managers[b->getID()]->inventory.push_back("fly");
							a->setAlive(false);
							g_engine->audio->Play("pkup");
						}
						temp = "";
					}					
				}
				return true;
				break;

			case BULLET:
			case SACMA:
			case ROCKET:
			case BIOMECH_ROCKET:
			case BIOMECH_LASER:	
				//da se ne rusi i collidable spriteove ne pravi
				b->setAlive(false);
				if(b->child) b->child->setAlive(false);
				for(unsigned int i=0;i<b->children.size();i++)
				{
					b->children[i]->setAlive(false);
				}

				if(a->getText()=="buttonshoot")
				{
					//trebat ce vise, kad treba uc i izac, da ne izadje usred platforme, treba povezat area s samim liftom
					for(unsigned int i=0;i<a->children.size();i++)
					{
						if(a->children[i]->getArg(0)=="move_x")
						{
							if(a->children[i]->getState()==-999)
							{
								g_engine->audio->Play("pt1_strt");
								a->children[i]->setState(1);
								a->children[i]->setVelocityX(g_engine->stringtoint(a->children[i]->getArg(2)));
								a->children[i]->rel_posx = a->children[i]->getX()+(int)g_engine->stringtoint(a->children[i]->getArg(1));
								g_engine->audio->Play("butn2");
							}
						}

						if(a->children[i]->getArg(0)=="move_y")
						{
							if(a->children[i]->getState()==-999)
							{
								g_engine->audio->Play("pt1_strt");
								a->children[i]->setState(1);
								a->children[i]->setVelocityY(g_engine->stringtoint(a->children[i]->getArg(2)));
								a->children[i]->rel_posy = a->children[i]->getY()+(int)g_engine->stringtoint(a->children[i]->getArg(1));
								g_engine->audio->Play("butn2");
							}
						}
					}
				}

				if(a->getText()=="explode" && (b->getType()==BULLET || b->getType()==SACMA || b->getType()==ROCKET))
				{
					g_engine->audio->Play("ROCKLX1A");
					editor->Particle_manager->addSmoke(a->cx(),a->cy(),21);
					editor->Particle_manager->addSpark(a->cx(),a->cy(),21);
					editor->Particle_manager->addFire(a->cx(),a->cy(),21);
					if(a->getLayer()==WORLD_FRONT_COLL) RemoveFromSector(a);
					a->setAlive(false);
				}

				if(b->getID()==BLASTER || b->getID()==HYPERBLASTER || b->getID()==RAILGUN)
				{
					if(b->getName()!="tongue") bullethitskeleton_box(b,a);editor->Particle_manager->addBulletSpark(b,a,21);
				}
				else if (b->getID()==R_LAUNCHER || b->getType()==BIOMECH_ROCKET)
				{
					b->setAlive(false);//parents, child ???
					g_engine->audio->Play("ROCKLX1A");
					editor->Particle_manager->addSmoke(b->coll_box[1].getX(),b->coll_box[1].getY(),21);
					editor->Particle_manager->addSpark(b->coll_box[1].getX(),b->coll_box[1].getY(),21);
					editor->Particle_manager->addFire(b->coll_box[1].getX(),b->coll_box[1].getY(),21);
					return true;
				}
				else
				{
					if(b->getName()!="tongue") editor->Particle_manager->addBulletSpark_Grey(b,a,21);
				}

				if(a->getText()=="glass" || a->getText()=="break")
				{
					BreakGlass2(a,24,24,b->cx(),b->cy());
					a->setAlive(false);
					if(a->getText()=="glass") g_engine->audio->Play("brkglas");
					if(a->getLayer()==WORLD_FRONT_COLL) RemoveFromSector(a);					
				}

				return true;
				break;
			}//switch(b)

			break;//PLATFORMA

		case GIBS:
			switch(b->getClass())
			{
			case PLAYER_WEAPON:
				if(a->getName()!="$GUN" && a->getName()!="$GUN1" && a->getName()!="$GUN2")
				{
					editor->Particle_manager->addBloodBig(a->getBoxCX(),a->getBoxCY(),21);
					a->setAlive(false);
				}
				return true;
				break;
			}
			break;		

		}//switch(a)
	}//PLAY

	return false;
}

void RenderToPlayerScreen(int id)
{
	g_engine->offset_X = editor->Player_managers[id]->screen->getOffsetX();
	g_engine->offset_Y = editor->Player_managers[id]->screen->getOffsetY();

	g_engine->p_device->SetRenderTarget(0,editor->Player_managers[id]->screen->getRenderSurface());
	g_engine->ar_graphics->ClearScene(D3DCOLOR_XRGB(editor->map_red,editor->map_green,editor->map_blue));
	g_engine->p_device->BeginScene();
	g_engine->ar_graphics->Render2D_Start();

	if (g_engine->getRenderType()==TEXTURE || g_engine->getRenderType()==ALL)
	{
		if(editor->Player_managers[id]->in_lift)
		{
			g_engine->game_maps[g_engine->map_current]->sprite_layers[0]->draw(TEXTURE);
			g_engine->game_maps[g_engine->map_current]->sprite_layers[1]->draw(TEXTURE);
			g_engine->game_maps[0]->sprite_layers[PLAYER]->draw_checkmap(TEXTURE);//player
			g_engine->game_maps[g_engine->map_current]->sprite_layers[2]->draw(TEXTURE);
		}
		else
		{
			for(unsigned int i=0;i<WORLD_BACK_1;i++)//treba prominit redosljed kad je u liftu
			{
				g_engine->game_maps[g_engine->map_current]->sprite_layers[i]->draw(TEXTURE);
			}
		}

	}//TEXTURE

	//render back darkness over this
	if(g_engine->game_maps[g_engine->map_current]->dark != 0)
	{
		g_engine->ar_graphics->Render2D_Stop();
		g_engine->p_device->EndScene();

		g_engine->p_device->SetRenderTarget(0,editor->Player_managers[id]->screen->getDarkSurfaceBack());
		g_engine->ar_graphics->ClearScene(D3DCOLOR_XRGB(0,255,0));
		g_engine->p_device->BeginScene();
		g_engine->ar_graphics->Render2D_Start();

		if (g_engine->getRenderType()==TEXTURE || g_engine->getRenderType()==ALL)
		{
			g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_1]->draw(TEXTURE);
			g_engine->game_maps[g_engine->map_current]->sprite_layers[SHADERSPRITES]->draw(TEXTURE);
		}

		g_engine->ar_graphics->Render2D_Stop();
		g_engine->p_device->EndScene();

		g_engine->p_device->SetRenderTarget(0,editor->Player_managers[id]->screen->getRenderSurface());
		g_engine->p_device->BeginScene();		

		//
		g_engine->p_effect->SetTechnique("nogreenback");
		g_engine->ar_graphics->Render2D_Start();

		UINT passes = 0;
		g_engine->p_effect->Begin(&passes, 0);
		g_engine->p_effect->BeginPass(0);

		editor->Player_managers[id]->screen->DrawDarkBack();				

		g_engine->ar_graphics->Render2D_Stop();
		g_engine->p_effect->EndPass();
		g_engine->p_effect->End();

		g_engine->ar_graphics->Render2D_Start();

	}

	if (g_engine->getRenderType()==TEXTURE || g_engine->getRenderType()==ALL)
	{
		//iscrtavanje preko mape
		for(unsigned int i=ENEMIES;i<SHADERSPRITES;i++)
		{
			//oba su u istom sloju :/
			if(i==PLAYER)
			{
				if(!editor->Player_managers[id]->in_lift) g_engine->game_maps[0]->sprite_layers[i]->draw_checkmap(TEXTURE);//player sloj iz prve mape, ostalo iz trenutne				
			}
			else
			{
				g_engine->game_maps[g_engine->map_current]->sprite_layers[i]->draw(TEXTURE);				
			}
		}
	}	

	if (g_engine->getRenderType()==VECTOR || g_engine->getRenderType()==ALL)
	{
		//iscrtavanje preko mape
		for(unsigned int i=0;i<SHADERSPRITES;i++)
		{
			if(i==PLAYER)
				g_engine->game_maps[0]->sprite_layers[i]->draw(VECTOR);//player sloj iz prve mape, ostalo iz trenutne
			else
				g_engine->game_maps[g_engine->map_current]->sprite_layers[i]->draw(VECTOR);
		}		
	}

	g_engine->ar_graphics->Render2D_Stop();
	g_engine->p_device->EndScene();

	//RENDER TO DARKNESS TEXTURE
	if(g_engine->game_maps[g_engine->map_current]->dark != 0)
	{
		g_engine->p_device->SetRenderTarget(0,editor->Player_managers[id]->screen->getDarkSurface());//0,0,0,0 skroz tamno
		g_engine->p_device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(0,0,255,0),1.0f,0); 
		g_engine->p_device->BeginScene();

		//draw lights to texture
		g_engine->ar_graphics->Render2D_Start();

		g_engine->game_maps[g_engine->map_current]->sprite_layers[BACKGROUND_1]->DrawShadow();
		g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_0]->DrawShadow();
		g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_1]->DrawShadow();
		g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_COLL]->DrawShadow();
		g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_NON]->DrawShadow();

		g_engine->game_maps[g_engine->map_current]->sprite_layers[SHADERSPRITES]->draw(TEXTURE);		
		g_engine->ar_graphics->Render2D_Stop();

		g_engine->p_device->EndScene();		
	}

}//RenderToPlayerScreen()

void game_render()
{
	///////
	/*tick = (timeGetTime()-lasttick);
	lasttick = timeGetTime();
	if (tick>50) tick=50;
	TimerA = TimerA + tick;*/	
	///////

	////in game rendering
	if (g_engine->getEngineState()==GAME && (g_engine->getGameState()==PLAY || g_engine->getGameState()==GAMEOVER))
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)//ne crta shadersprites layer 
		{
			if (editor->Player_managers[i]->active)
			{
				g_engine->map_current = editor->Player_managers[i]->map_id;
				RenderToPlayerScreen(i);
			}
		}		
	}	

	//start rendering to backbuffer
	g_engine->p_device->SetRenderTarget(0,g_engine->pBackBuffer);
	g_engine->ar_graphics->ClearScene(D3DCOLOR_XRGB(editor->map_red,editor->map_green,editor->map_blue));
	g_engine->p_device->BeginScene();	

	//render player screens on backbuffer
	if (g_engine->getEngineState()==GAME && (g_engine->getGameState()==PLAY || g_engine->getGameState()==GAMEOVER))
	{
		g_engine->ar_graphics->Render2D_ShaderStart(stuffEffect(PixelShader_ID));
		g_engine->ar_graphics->Render2D_Start();

		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->active) editor->Player_managers[i]->screen->Draw();			
		}

		g_engine->ar_graphics->Render2D_Stop();	
		g_engine->ar_graphics->Render2D_ShaderStop();
	}	

	//render darkness textures on backbuffer
	if(g_engine->game_maps[g_engine->map_current]->dark != 0)
	{
		g_engine->p_effect->SetTechnique("nogreen");
		g_engine->p_effect->SetFloat("darkness",(float)(100-g_engine->game_maps[g_engine->map_current]->dark)/100);

		g_engine->ar_graphics->Render2D_Start();

		UINT passes = 0;
		g_engine->p_effect->Begin(&passes, 0);
		g_engine->p_effect->BeginPass(0);

		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->active) editor->Player_managers[i]->screen->DrawDark();			
		}		

		g_engine->ar_graphics->Render2D_Stop();
		g_engine->p_effect->EndPass();
		g_engine->p_effect->End();
	}	

	g_engine->ar_graphics->Render2D_Start();

	//DRAW PLAYER HUD
	for(unsigned int i=0;i<editor->Player_managers.size();i++)
	{
		if(editor->Player_managers[i]->p_player)
		{
			editor->Player_managers[i]->hud->draw(
				editor->Player_managers[i]->p_player->energy,
				editor->Player_managers[i]->armor,
				editor->Player_managers[i]->player_weapon);

			if(editor->Player_managers[i]->draw_inventory) editor->DrawInventory(i);
		}
	}

	//SPLIT-SCREEN LINE
	if (g_engine->getEngineState()==GAME && (g_engine->getGameState()==PLAY || g_engine->getGameState()==GAMEOVER))
	{
		if(editor->player_number==2)
		{
			if (!editor->options->s_splitscreen)
				g_engine->ar_graphics->draw_line(
				editor->Player_managers[0]->screen->getWidth(),0,
				editor->Player_managers[0]->screen->getWidth(),editor->Player_managers[0]->screen->getHeight(),
				0,0,0xFF000000);
			else
				g_engine->ar_graphics->draw_line(
				0,editor->Player_managers[0]->screen->getHeight(),
				editor->Player_managers[0]->screen->getWidth(),editor->Player_managers[0]->screen->getHeight(),
				0,0,0xFF000000);
		}
	}	

	//treba odvojit player controle od ovih ostalih da ne ide preko controlrelease i press
	if(editor->display_f1 && g_engine->getGameState()==PLAY) editor->DrawF1();
	if(g_engine->getGameState()==PLAY)
	{
		if(editor->f1_tick < 5000/14) editor->f1_tick += g_engine->game_speed*g_engine->GetDeltaFix();
		if(editor->f1_tick >= 5000/14 && editor->Player_managers[0]->hud->new_f1)
		{
			g_engine->audio->Play("pc_up");
			editor->f1_tick = 0;
		}
	}

	//PAUSE
	if(g_engine->p_pauseMode)
	{
		g_engine->ar_graphics->draw_texture(
			editor->t_menu_paused,
			g_engine->getScreenWidth()/2-editor->t_menu_paused->getWidth()/2,
			g_engine->getScreenHeight()/2-editor->t_menu_paused->getHeight()/2,
			(int)editor->t_menu_paused->getWidth(),(int)editor->t_menu_paused->getHeight(),
			0,0,			
			(int)editor->t_menu_paused->getWidth(),(int)editor->t_menu_paused->getHeight(),
			1,1,
			0xFFFFFFFF);	
	}	

	if(g_engine->getGameState()==OPTIONS && editor->options->options_visible)
	{
		g_engine->ar_graphics->draw_texture(
			editor->t_options_banner,
			g_engine->getScreenWidth()/2-editor->t_options_banner->getWidth()/2,
			g_engine->getScreenHeight()/2 - 175*editor->gui_scale,
			(int)editor->t_options_banner->getWidth(),(int)editor->t_options_banner->getHeight(),
			0,0,			
			(int)editor->t_options_banner->getWidth(),(int)editor->t_options_banner->getHeight(),
			1,1,
			0xFFFFFFFF);

		g_engine->ar_graphics->draw_texture(
			editor->t_video_banner,
			g_engine->getScreenWidth()/2-editor->t_video_banner->getWidth()/2,
			g_engine->getScreenHeight()/2 + 25*editor->gui_scale,
			(int)editor->t_video_banner->getWidth(),(int)editor->t_video_banner->getHeight(),
			0,0,			
			(int)editor->t_video_banner->getWidth(),(int)editor->t_video_banner->getHeight(),
			1,1,
			0xFFFFFFFF);

	}	

	if (g_engine->getEngineState()==EDITOR) editor->Draw();
	if (g_engine->getEngineState()==ANIMATION) animator->Draw();

	g_engine->gui->DrawGUI_Buttons();

	//zboh player modela, treba to smislit
	if (g_engine->getEngineState()==GAME && g_engine->getGameState()==MENU)
	{
		for(unsigned int i=0;i<editor->pl_models.size();i++)
		{
			editor->pl_models[i]->animate();
			editor->Player_managers[i]->player_models[editor->Player_managers[i]->model_id]->animation_run.setRotation(editor->pl_models[i]);
			g_engine->skeleton_manager.Walk_Bipedal(editor->pl_models[i]);
		}

		for(int i=0;i<g_engine->game_sprites.size();i++) g_engine->game_sprites[i]->draw(g_engine->getRenderType());
	}

	if(g_engine->getGameState()==GAMEOVER)
	{
		g_engine->font_manager.fonts[0]->PrintText(
			"GAME OVER",
			g_engine->getScreenWidth()/2-g_engine->font_manager.fonts[0]->getLength("GAME OVER",g_engine->font_manager.fonts[0]->char_dist,1)/2,
			g_engine->getScreenHeight()/2-g_engine->font_manager.fonts[0]->getHeight()/2,
			1,1,0xFFFFFFFF);	
	}

	if(g_engine->gui->cursor->getImage()->getTilemap()) g_engine->gui->cursor->animate();
	g_engine->gui->DrawGUI_Cursor();

	if(g_engine->getGameState()==PLAY && g_engine->display_lineofsight)
	{
		g_engine->font_manager.fonts[0]->PrintText(g_engine->inttostring(g_engine->game_maps[g_engine->map_current]->sectors.size()),0,0,1,1,0xFFFFFFFF);	
		for(unsigned int i=0;i<g_engine->game_maps[g_engine->map_current]->sectors.size();i++)
		{
			g_engine->game_maps[g_engine->map_current]->sectors[i].Draw();
		}
	}


	//Draw lines
	std::list<Line*>::iterator iter = g_engine->line_list.begin();
	while (iter != g_engine->line_list.end())
	{
		Line *a = *iter;//treba ih brisat da oslobodi memoriju !!!
		g_engine->ar_graphics->draw_line(a->x1,a->y1,a->x2,a->y2,g_engine->getLayerOffsetX(a->layer),g_engine->getLayerOffsetY(a->layer),a->color);
		delete (*iter);
		iter = g_engine->line_list.erase(iter);
	}

	if (console_state)
	{
		//konzola seta okolo
		g_engine->ar_graphics->draw_sprite(g_engine->gui->console_panel,0,0);
		ShowConsole();
		PrintStuff(editor->Player_managers[0]->p_player);
		Print_JoystickState();
	}	

	if (cmd && g_engine->p_pauseMode)
	{
		stringstream cmd_text;
		int posy = 5;

		g_engine->gui->console_panel->setPosition(g_engine->getScreenWidth()-400,0);
		g_engine->ar_graphics->draw_sprite(g_engine->gui->console_panel,0,0);		

		for (unsigned int i=0;i<g_engine->gui->console_text.size();i++)
		{
			if (i==g_engine->gui->console_text.size()-1)
				cmd_text << ":>" << g_engine->gui->console_text[i] << "_";
			else
				cmd_text << g_engine->gui->console_text[i];

			g_engine->font_manager.fonts[0]->PrintText(cmd_text.str(),g_engine->getScreenWidth()-400,posy,1,1,0xFF00FF00);

			cmd_text.str("");     
			posy+=20;
		}

		g_engine->ar_graphics->draw_box(
			g_engine->gui->console_panel->getX(),g_engine->gui->console_panel->getY(),
			g_engine->gui->console_panel->w(),g_engine->gui->console_panel->h(),
			0,0,g_engine->gui->console_panel->getColor_Box());

		g_engine->gui->console_panel->setPosition(0,0);		
	}

	//display FPS
	if(g_engine->display_fps) g_engine->font_manager.fonts[0]->PrintText(g_engine->inttostring((int)g_engine->getFrameRate_FPS()) + " FPS",g_engine->getScreenWidth()-150,10,1,1,0xFFFFFFFF);	

	g_engine->ar_graphics->Render2D_Stop();
	g_engine->p_device->EndScene();
	g_engine->p_device->Present(NULL, NULL, NULL, NULL);

}//game_render

//////////////////
//  PARSE TEXT  //
//////////////////

void textbox_parse(std::string line)
{
	//treba ovdje poslat i botun
	if (g_engine->gui->getSelectedButton()==NULL) return;

	//cursor attachement
	Button *c_button = g_engine->gui->getSelectedButton();

	switch(c_button->getGUI())//trebalo bi biti obrnuto
	{
	case EDITSPRITE_CLASS:
		if (g_engine->gui->getSelectedSprite()!=NULL)
		{
			editor->edit_sprite->Update_sprite(g_engine->gui->getSelectedSprite(),c_button);
		}
		break;

	case EDITLAYER_CLASS:
		editor->edit_layer->LayerData_update(c_button);
		break;
	}

	if(c_button->getType()==BUTTON_ANIMATOR_SAVETOFILE)
	{
		if(animator->Animation_ListOn(c_button->getText())) 
		{
			Button *temp_button = g_engine->gui->form_add_yesno(
				"SAVE ANIMATION","Overwrite existing animation ?",250,200);
			temp_button->form[3]->setType(BUTTON_ANIMATOR_SAVETOFILE);
			temp_button->form[3]->setText(c_button->getText());
			temp_button->form[3]->setWidth(temp_button->form[3]->getWidth()+16);
			temp_button->form[3]->setX(temp_button->form[3]->getX()-16);
			temp_button->form[3]->setState(1);
			temp_button->parent = c_button;
		}
		else
		{
			animator->Animation_save(c_button->getText());
			g_engine->gui->form_delete(c_button->parent);
		}
	}

	if(c_button->getType()==MAP_SAVEAS)
	{
		if(editor->Map_ListOn(c_button->getText())) 
		{
			Button *temp_button = g_engine->gui->form_add_yesno(
				"SAVE MAP","Overwrite existing map ?",250,200);
			temp_button->form[3]->setType(MAP_SAVE);
			temp_button->form[3]->setText(c_button->getText());
			temp_button->form[3]->setWidth(temp_button->form[3]->getWidth()+16);
			temp_button->form[3]->setX(temp_button->form[3]->getX()-16);
			temp_button->form[3]->setID(1);
			temp_button->parent = c_button;			
		}
		else
		{
			editor->Map_SaveToFile(c_button->getText());
			g_engine->gui->form_delete(c_button->parent);
		}
	}

	//release the button
	g_engine->gui->setSelectedButton(NULL);

}//textbox_parse()



void console_parse(std::string line)
{
	if (line.size()==0) return;
	std::stringstream temp_stream(line);

	g_engine->gui->console_text[0] = ":>"+line;

	std::string arg0;
	std::string arg1;

	if(g_engine->locked)
	{
		if(temp_stream >> arg0)
		{
			if(arg0 == "Wien")
			{
				std::vector <std::string> temp;
				temp.push_back("lock_this_game 0");

				std::ifstream filein("System/config.txt");
				std::string line;
				while(getline(filein,line)) temp.push_back(line);
				filein.close();

				std::ofstream fileout("System/config.txt");
				for(unsigned int i=0;i<temp.size();i++)
				{
					fileout << temp[i] << std::endl;
				}
				fileout.close();

				g_engine->locked = false;
				editor->options->s_gamelocked = false;
				g_engine->gui->console_text[1] = "GAME UNLOCKED";
			}
			else
			{
				g_engine->gui->console_text[1] = "ENTER CORRECT UNLOCKING CODE !";
			}
		}
	}
	else
	{
		if(temp_stream >> arg0)
		{
			if(arg0 == "give")
			{
				if(temp_stream >> arg1)
				{
					if(arg1 == "health")
					{
						for(unsigned int i=0;i<editor->Player_managers.size();i++)
						{
							if(editor->Player_managers[i]->p_player)
								editor->Player_managers[i]->p_player->energy = 100;
						}
						g_engine->gui->console_text[1] = "CHEAT -> full health";

					}
					else if(arg1 == "all")
					{
						for(unsigned int i=0;i<editor->Player_managers.size();i++)
						{
							for(int j=0;j<15;j++) editor->Player_managers[i]->hud->weapon_unlocked[j] = true;
							for(int k=0;k<9;k++) editor->Player_managers[i]->hud->ammo_amount[k] = editor->Player_managers[0]->hud->ammo_amount_max[k];
							if (editor->Player_managers[i]->p_player)
								editor->Player_managers[i]->p_player->energy = 100;
							editor->Player_managers[i]->inventory.push_back("redkey");
							editor->Player_managers[i]->inventory.push_back("bluekey");
							editor->Player_managers[i]->inventory.push_back("fly");
							editor->Player_managers[i]->inventory.push_back("bullettime");
							g_engine->gui->console_text[1] = "CHEAT -> give all";	
						}
					}
					else
						g_engine->gui->console_text[1] = "error# invalid command";
				}
				else
				{
					g_engine->gui->console_text[1] = "error# invalid command";
				}
			}
			else if(arg0 == "god")
			{
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					if(editor->Player_managers[i]->p_player)
						editor->Player_managers[i]->god = !editor->Player_managers[i]->god;						
				}
				if(editor->Player_managers[0]->god) g_engine->gui->console_text[1] = "CHEAT -> godmode ON";
				else g_engine->gui->console_text[1] = "CHEAT -> godmode OFF";
			}
			else if(arg0 == "notarget")
			{
				g_engine->coll_manager->notarget = !g_engine->coll_manager->notarget;
				if(g_engine->coll_manager->notarget)
					g_engine->gui->console_text[1] = "CHEAT -> notarget ON";
				else
					g_engine->gui->console_text[1] = "CHEAT -> notarget OFF";
			}
			else if(arg0 == "noclip")
			{
				g_engine->noclip = !g_engine->noclip;
				if(g_engine->noclip)
					g_engine->gui->console_text[1] = "CHEAT -> noclip ON";
				else
					g_engine->gui->console_text[1] = "CHEAT -> noclip OFF";
			}
			else
			{
				g_engine->gui->console_text[1] = "error# invalid command";
			}
		}
	}

}//console_parse

////////////////
//  KEYBOARD  //
////////////////

void game_keyPress(int key)
{
	if (g_engine->getEngineState()==EDITOR)
	{
		editor->keyPress(key);
		return;
	}
	else if (g_engine->getEngineState()==ANIMATION)
	{
		animator->keyPress(key);
		return;
	}
	else if (g_engine->getEngineState()==GAME && g_engine->getGameState()==PLAY && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player) editor->Player_managers[i]->control_press(key);
		}		
		return;
	}

}//game_keyPress

void game_keyRelease(int key) 
{
	///////////////
	//  OPTIONS  //
	///////////////	

	if (g_engine->getGameState()==OPTIONS && g_engine->gui->getSelectedButton()!=NULL)
	{
		if(g_engine->gui->getSelectedButton()->getType()!=O_LOCKFRAMERATE)
		{
			//player controls
			g_engine->gui->getSelectedButton()->setText(g_engine->gui->key_to_string(key));
			g_engine->gui->setSelectedButton(NULL);//allow only one key
			return;
		}
	}

	////////////////
	// TEXT INPUT //
	////////////////

	//write to selected text box
	if (g_engine->gui->getSelectedButton()!=NULL)//treba stavit da preskoci (li sta vec) enter i ostalo
	{
		if (g_engine->gui->getSelectedButton()->getClass()==TEXTBOX)
		{
			if (key==DIK_RETURN || key==DIK_NUMPADENTER)
				textbox_parse(g_engine->gui->getSelectedButton()->getText());
			else
				g_engine->gui->getSelectedButton()->setText(
				g_engine->gui->text_input(key,g_engine->gui->getSelectedButton()->getText())
				);			
		}
		return;
	}

	//console input
	if (cmd && g_engine->p_pauseMode)
	{
		if (key==DIK_RETURN || key==DIK_NUMPADENTER)
		{
			console_parse(g_engine->gui->console_text[g_engine->gui->console_text.size()-1]);
			g_engine->gui->console_text[g_engine->gui->console_text.size()-1]=("");
		}
		else if (key==DIK_F12)//to close te console
		{
			cmd = false;
			g_engine->p_pauseMode=false;
		}
		else
		{
			g_engine->gui->console_text[g_engine->gui->console_text.size()-1]=
				g_engine->gui->text_input(key,g_engine->gui->console_text[g_engine->gui->console_text.size()-1]);
		}
		return;
	}

	/////////////
	// DEFAULT //
	/////////////

	static int render_type = 0;	

	switch (key)
	{
	case DIK_INSERT:
		if(editor->Player_managers[0]->active && !editor->Player_managers[0]->p_player && editor->Player_managers[1]->p_player && editor->Player_managers[1]->p_player->energy > 2)
		{
			editor->Player_managers[0]->p_player = editor->Player_managers[0]->addPlayer(editor->Player_managers[1]->p_player->getX(),editor->Player_managers[1]->p_player->getY() - 25);
			editor->Player_managers[0]->UpdateSkeleton();
			editor->Player_managers[0]->setWeaponTexture();
			editor->Player_managers[0]->p_player->energy = editor->Player_managers[1]->p_player->energy/2;
			editor->Player_managers[1]->p_player->energy = editor->Player_managers[1]->p_player->energy/2;
		}
		else if(editor->Player_managers[1]->active && !editor->Player_managers[1]->p_player && editor->Player_managers[0]->p_player && editor->Player_managers[0]->p_player->energy > 2)
		{
			editor->Player_managers[1]->p_player = editor->Player_managers[1]->addPlayer(editor->Player_managers[0]->p_player->getX(),editor->Player_managers[0]->p_player->getY() - 25);
			editor->Player_managers[1]->UpdateSkeleton();
			editor->Player_managers[1]->setWeaponTexture();
			editor->Player_managers[0]->p_player->energy = editor->Player_managers[0]->p_player->energy/2;
			editor->Player_managers[1]->p_player->energy = editor->Player_managers[0]->p_player->energy/2;			
		}
		break;

	case DIK_P:if(g_engine->getGameState()==PLAY) g_engine->p_pauseMode = !g_engine->p_pauseMode;break;//botun "Pause" cini se ima mali prioritet
	case DIK_GRAVE:console_state =! console_state;break;

	case DIK_F6:
		//g_engine->display_sectors = !g_engine->display_sectors;
		if(!g_engine->locked)
		{
			g_engine->display_collisiondots = !g_engine->display_collisiondots;
			g_engine->display_rotationaxis = !g_engine->display_rotationaxis;
		}
		break;

	case DIK_F7:if(!g_engine->locked) {g_engine->display_lineofsight = !g_engine->display_lineofsight;}break;

	case DIK_F8:
		if(!g_engine->locked)
		{
			render_type++;
			if(render_type>2) render_type = 0;
			switch(render_type)
			{
			case 0:g_engine->setRenderType(TEXTURE);break;
			case 1:g_engine->setRenderType(ALL);break;
			case 2:g_engine->setRenderType(VECTOR);break;
			}			
		}
		break;

	case DIK_F9:
		if (editor->player_number==2)
		{
			editor->options->s_splitscreen = !editor->options->s_splitscreen;			

			if (!editor->options->s_splitscreen)
			{
				editor->Player_managers[0]->screen_create(0,0,
					g_engine->getScreenWidth()/2,g_engine->getScreenHeight());
				editor->Player_managers[1]->screen_create(g_engine->getScreenWidth()/2,0,
					g_engine->getScreenWidth()/2,g_engine->getScreenHeight());
			}
			else
			{
				editor->Player_managers[0]->screen_create(0,0,
					g_engine->getScreenWidth(),g_engine->getScreenHeight()/2);
				editor->Player_managers[1]->screen_create(0,g_engine->getScreenHeight()/2,
					g_engine->getScreenWidth(),g_engine->getScreenHeight()/2);
			}
		}
		break;

	case DIK_F4:g_engine->display_fps = !g_engine->display_fps;break;

	case DIK_F11:
		if(!g_engine->locked)
		{
			if (PixelShader_ID < 15)
				PixelShader_ID++;
			else
				PixelShader_ID=0;
		}
		break;

	case DIK_F12:
		if(cmd){cmd = false;g_engine->p_pauseMode = false;}
		else{cmd = true;g_engine->p_pauseMode = true;}
		break;
	}

	//////////////
	// ANIMATOR //
	//////////////

	if (g_engine->getEngineState()==ANIMATION)
	{
		animator->keyRelease(key);
		return;
	}	

	//////////
	// GAME //
	//////////

	if (g_engine->getGameState()==PLAY && g_engine->getEngineState()==GAME && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player) editor->Player_managers[i]->control_release(key);
		}

		switch(key)
		{
		case DIK_F1:
			editor->display_f1 = !editor->display_f1;
			for(unsigned int i=0;i<editor->Player_managers.size();i++)
			{
				editor->Player_managers[i]->hud->new_f1 = false;
			}
			if(g_engine->audio->IsPlaying("pc_up")) g_engine->audio->Stop("pc_up");
			break;

		case DIK_ESCAPE:
			g_engine->p_pauseMode = false;
			g_engine->audio->StopAll();
			g_engine->save_to_map = false;
			editor->display_f1 = false;
			g_engine->bullet_time = false;
			for(unsigned int i=0;i<editor->Player_managers.size();i++)
			{
				if (editor->Player_managers[i]->p_player)
					DeletePlayer(editor->Player_managers[i]->p_player->getID());
				editor->Player_managers[i]->active = false;
			}			
			editor->addMenuButtons();
			g_engine->audio->StopAll();			
			break;
		}
	}

	////////////
	// EDITOR //
	////////////

	if (g_engine->getEngineState()==EDITOR)
	{
		editor->keyRelease(key);
		return;
	}

	///////////////
	// GAME OVER //
	///////////////

	if (g_engine->getGameState()==GAMEOVER)
	{
		switch(key)
		{
		case DIK_ESCAPE:
			g_engine->p_pauseMode = false;
			g_engine->audio->StopAll();
			g_engine->save_to_map = false;
			editor->display_f1 = false;
			g_engine->bullet_time = false;
			for(unsigned int i=0;i<editor->Player_managers.size();i++)
			{
				if (editor->Player_managers[i]->p_player)
					DeletePlayer(editor->Player_managers[i]->p_player->getID());
				editor->Player_managers[i]->active = false;
			}	
			editor->addMenuButtons();
			break;
		}  
		return;
	}	

}//game_keyRelease()

/////////////
//  MOUSE  //
/////////////

void game_mousePress(int button)
{
	if (g_engine->getEngineState()==EDITOR)
	{
		editor->mousePress(button);
		return;
	}
	else if (g_engine->getEngineState()==ANIMATION)
	{
		switch(button)
		{
		case 0:
			g_engine->gui->CursorCollision();
			if (g_engine->gui->getSelectedSprite()!=NULL) editor->clipboard->movesprite(g_engine->gui->getSelectedSprite());			
			break;

		case 2:
			g_engine->gui->setSelectedButton(NULL);
			g_engine->gui->setSelectedSprite(NULL);
			break;
		}
	}
	else if (g_engine->getEngineState()==GAME && g_engine->getGameState()==PLAY && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player) editor->Player_managers[i]->control_press(-button);
		}		
	}

}//game_mousePress

void game_mouseRelease(int button)
{
	if (g_engine->getGameState()==OPTIONS && g_engine->gui->getSelectedButton()!=NULL)
	{
		g_engine->gui->getSelectedButton()->setText(g_engine->gui->key_to_string(-button));
		g_engine->gui->setSelectedButton(NULL);//allow only one key
		return;
	}

	if (g_engine->getEngineState()==ANIMATION)
	{
		animator->mouseRelease(button);
		return;
	}

	if (g_engine->getEngineState()==EDITOR)
	{
		editor->mouseRelease(button);
		return;
	}

	if (g_engine->getGameState()==PLAY && g_engine->getEngineState()==GAME && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player) editor->Player_managers[i]->control_release(-button);
		}
		return;
	}

}//game_mouseRelease

void game_mouseMove(int x,int y)//ovo se poziva samo kad nastane promjene nije prilikom svake iteracije petlje
{
	// drukcije za nisan i oni u meni-u ??
	if (g_engine->getGameState()==PLAY && editor->player_number>1)
	{
		if (x>editor->Player_managers[0]->screen->getWidth()-g_engine->gui->cursor->w())
			x = editor->Player_managers[0]->screen->getWidth()-g_engine->gui->cursor->w();	
		if (y>editor->Player_managers[0]->screen->getHeight()-g_engine->gui->cursor->h())
			y = editor->Player_managers[0]->screen->getHeight()-g_engine->gui->cursor->h();		
	}	

	g_engine->gui->cursor->setPosition(x,y);	

}//game_mouseMove()

void game_mouseWheel(int wheel)
{
	if (g_engine->getEngineState()==EDITOR)
	{
		editor->mouseWheel(wheel);
		return;
	}
	if (g_engine->getEngineState()==ANIMATION)
	{
		animator->mouseWheel(wheel);
		return;
	}
	else if (g_engine->getGameState()==OPTIONS && g_engine->gui->getSelectedButton()!=NULL)
	{
		g_engine->gui->getSelectedButton()->setText(g_engine->gui->key_to_string(wheel*10));
		g_engine->gui->setSelectedButton(NULL);//allow only one key
		return;
	}
	else if (g_engine->getGameState()==PLAY && g_engine->getEngineState()==GAME && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player)
				editor->Player_managers[i]->control_release(wheel*10);
		}				
	}

}//game_mouseWheel()

////////////////
//  JOYSTICK  //
////////////////

void game_joystickPress(int button)
{
	if (g_engine->getGameState()==PLAY && g_engine->getEngineState()==GAME && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player) editor->Player_managers[i]->control_press(button+1000);
		}		
	}

}//game_joystickPress()

void game_joystickRelease(int button)
{
	if (g_engine->getGameState()==OPTIONS && g_engine->gui->getSelectedButton()!=NULL)
	{
		g_engine->gui->getSelectedButton()->setText(g_engine->gui->key_to_string(button+1000));
		g_engine->gui->setSelectedButton(NULL);//allow only one key
		return;
	}

	if (g_engine->getGameState()==PLAY && g_engine->getEngineState()==GAME && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player) editor->Player_managers[i]->control_release(button+1000);
		}
	}

}//game_joystickRelease()

void game_joystickXYAxisPress(int axis)
{
	if (g_engine->getGameState()==PLAY && g_engine->getEngineState()==GAME && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player) editor->Player_managers[i]->control_press(axis);
		}
		return;
	}
}

void game_joystickXYAxisRelease(int axis)
{
	if (g_engine->getGameState()==OPTIONS && g_engine->gui->getSelectedButton()!=NULL)
	{
		g_engine->gui->getSelectedButton()->setText(g_engine->gui->key_to_string(axis));
		g_engine->gui->setSelectedButton(NULL);
		return;
	}

	if (g_engine->getGameState()==PLAY && g_engine->getEngineState()==GAME && g_engine->p_pauseMode==false)
	{
		for(unsigned int i=0;i<editor->Player_managers.size();i++)
		{
			if (editor->Player_managers[i]->p_player) editor->Player_managers[i]->control_release(axis);
		}
		return;
	}
}

void game_joystickAxis(LONG x, LONG y, LONG z, LONG z_rot)
{
	for(int i=0;i<editor->Player_managers.size();i++)
	{
		if (editor->Player_managers[i]->p_player && editor->Player_managers[i]->player_control==JOYSTICK)
		{
			//asin i slicno prima vrijednosti od -1,do 1 cpluplus
			editor->Player_managers[i]->fire_angle = g_engine->math->toDegrees(atan2((float)z_rot,(float)z));

			if (editor->Player_managers[i]->fire_angle>-90 && editor->Player_managers[i]->fire_angle<90)
			{
				editor->Player_managers[i]->p_player->flip_h=false;
			}
			else
			{
				editor->Player_managers[i]->p_player->flip_h=true;
			}

		}
	}
}

///////////////////
//  GUI BUTTONS  //
///////////////////

void GUI_ButtonPress( Button *button, int mousebutton )
{
	if(g_engine->getEngineState()==EDITOR)
	{
		editor->buttonPress(button,mousebutton);		
	}

	switch (mousebutton)
	{
	case 0:
		switch (button->getType())
		{
		case O_VOLUMESOUND:
			g_engine->audio->setVolumeSound(
				(float)g_engine->stringtoint(button->getText())/100);
			g_engine->audio->UpdateVolumeSound();
			editor->options->s_volumesound = (int)g_engine->stringtoint(button->getText());
			break;

		case O_VOLUMEMUSIC:
			g_engine->audio->setVolumeStream(
				(float)g_engine->stringtoint(button->getText())/100);
			g_engine->audio->UpdateVolumeStream();
			editor->options->s_volumemusic = (int)g_engine->stringtoint(button->getText());
			break;
		}
		break;
	}
}

void GUI_ButtonRelease( Button *button, int mousebutton )
{
	switch(button->getType())
	{
	case BUTTON_DIFFICULTY:
	case BUTTON_GAME:
	case BUTTON_1PLAYER:
	case BUTTON_2PLAYER:
	case BUTTON_OPTIONS:
	case BUTTON_EDITOR:
	case BUTTON_ANIMEDITOR:
	case BUTTON_QUIT:
	case O_CONTROLS:
	case BUTTON_GAMECUSTOM:
		g_engine->audio->Play("menu1");
		break;

	case BUTTON_ANIMATOR_EXIT:
	case BUTTON_EXIT_EDITOR:
	case OPTIONS_EXIT:
	case BUTTON_TOMAINMENU:	
	case OPTIONS_EXITCONTROLS:	
		g_engine->audio->Play("menu3");
		break;
	}

	if (g_engine->getEngineState()==ANIMATION)
	{
		animator->buttonRelease(button,mousebutton);

		switch (mousebutton)
		{
		case 0:
			switch (button->getType())
			{
			case BUTTON_ANIMATOR_EXIT:	
				animator->Reset();
				editor->addMenuButtons();
				break;
			}
			return;
		}
	}	

	Button *temp_button = NULL;

	double value = 0;
	Sprite *a = NULL;	

	if (g_engine->getGameState()==OPTIONS)
	{
		editor->options->ButtonRelease(button,mousebutton);

		switch (mousebutton)
		{
		case 0:
			switch (button->getType())
			{
			case O_FULLSCREEN:
			case O_VSYNC:
			case O_SPLITSCREEN:
			case O_LOCKFRAMERATE:
				button->setColor_Texture(0x00000000);
				break;		

			case OPTIONS_SAVE:
				editor->options->options_savetofile();
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					editor->options->player_controls[i]->getPlayerControls(
						editor->Player_managers[i]->LEFT,editor->Player_managers[i]->RIGHT,
						editor->Player_managers[i]->UP,editor->Player_managers[i]->DOWN,
						editor->Player_managers[i]->JUMP,
						editor->Player_managers[i]->FIRE1,editor->Player_managers[i]->FIRE2,
						editor->Player_managers[i]->ACTION,editor->Player_managers[i]->PDA,
						editor->Player_managers[i]->C_FLY,editor->Player_managers[i]->BULLETTIME,
						editor->Player_managers[i]->NEXT_WEAPON,editor->Player_managers[i]->PREV_WEAPON,
						editor->Player_managers[i]->WEAPON0,editor->Player_managers[i]->WEAPON1,
						editor->Player_managers[i]->WEAPON2,editor->Player_managers[i]->WEAPON3,
						editor->Player_managers[i]->WEAPON4,editor->Player_managers[i]->WEAPON5,
						editor->Player_managers[i]->WEAPON6,editor->Player_managers[i]->WEAPON7,
						editor->Player_managers[i]->WEAPON8,editor->Player_managers[i]->WEAPON9);
				}
				break;

			case OPTIONS_EXIT:
				editor->addMenuButtons();
				g_engine->font_manager.fonts[0]->scale = editor->options->s_scalehud;
				for(unsigned int i=0;i<editor->Player_managers.size();i++)
				{
					editor->Player_managers[i]->hud->gui_scale = editor->options->s_scalehud;
				}
				editor->gui_scale = editor->options->s_scalehud;
				break;
			}
			break;
		}

		return;

	}//OPTIONS

	if (g_engine->getEngineState()==EDITOR) editor->buttonRelease(button, mousebutton);

	switch (mousebutton)
	{
	case 0:
		switch (button->getType())
		{		
		case PLAYERMODEL:
			if(button->getText()=="<")
				editor->Player_managers[button->getID()]->model_id--;
			else
				editor->Player_managers[button->getID()]->model_id++;

			//0-max
			if(editor->Player_managers[button->getID()]->model_id <= 0)
				editor->Player_managers[button->getID()]->model_id = 0;
			else if(editor->Player_managers[button->getID()]->model_id >= editor->Player_managers[button->getID()]->player_models.size()-1)
				editor->Player_managers[button->getID()]->model_id = editor->Player_managers[button->getID()]->player_models.size()-1;

			//delete skeleton and attachments
			for(unsigned int i=0;i<editor->pl_models[button->getID()]->skeleton.size();i++)
				editor->pl_models[button->getID()]->skeleton[i]->setAlive(false);
			for(unsigned int i=0;i<editor->pl_models[button->getID()]->attachments.size();i++)
				editor->pl_models[button->getID()]->attachments[i]->setAlive(false);
			editor->pl_models[button->getID()]->skeleton.clear();
			editor->pl_models[button->getID()]->attachments.clear();

			//update skeleton
			editor->Player_managers[button->getID()]->addSkeleton(editor->pl_models[button->getID()]);
			editor->pl_models[button->getID()]->setFramesTotal(
				editor->Player_managers[button->getID()]->player_models[editor->Player_managers[button->getID()]->model_id]->animation_run.totalframes
				);
			editor->pl_models[button->getID()]->setFrameTimer(
				editor->Player_managers[button->getID()]->player_models[editor->Player_managers[button->getID()]->model_id]->animation_run.timer
				);	
			editor->pl_models[button->getID()]->animate();
			editor->Player_managers[button->getID()]->player_models[editor->Player_managers[button->getID()]->model_id]->animation_run.setRotation(editor->pl_models[button->getID()]);
			g_engine->skeleton_manager.Walk_Bipedal(editor->pl_models[button->getID()]);

			//update position
			editor->pl_models[button->getID()]->setY(button->getY()+button->getHeight()/2-editor->pl_models[button->getID()]->h()/2);

			if(editor->pl_models[button->getID()]->getBone("$GUN1")) editor->pl_models[button->getID()]->getBone("$GUN1")->setVisible(false);
			if(editor->pl_models[button->getID()]->getBone("$GUN2")) editor->pl_models[button->getID()]->getBone("$GUN2")->setVisible(false);
			break;

		case BUTTON_GAME:editor->addMenu2Buttons();break;
		case BUTTON_TOMAINMENU:editor->addMenuButtons();break;

		case BUTTON_DIFFICULTY:
			g_engine->difficulty = button->getState();
			for(unsigned int i=0;i<button->children.size();i++)
			{
				button->children[i]->setColor_Text(0xFFFFFFFF);
			}
			button->setColor_Text(0xFF00FF00);
			break;

		case BUTTON_1PLAYER:
			{
				g_engine->p_device->SetRenderTarget(0,g_engine->pBackBuffer);
				g_engine->ar_graphics->ClearScene(D3DCOLOR_XRGB(0,0,0));
				g_engine->p_device->BeginScene();	
				g_engine->ar_graphics->Render2D_Start();
				//
				g_engine->ar_graphics->draw_texture(
					editor->t_menu_loading,
					g_engine->getScreenWidth()/2-editor->t_menu_loading->getWidth()/2,g_engine->getScreenHeight()/2-editor->t_menu_loading->getHeight()/2,
					editor->t_menu_loading->getWidth(),editor->t_menu_loading->getHeight(),
					0,0,editor->t_menu_loading->getWidth(),editor->t_menu_loading->getHeight(),
					1,1,
					0xFFFFFFFF);
				//
				g_engine->ar_graphics->Render2D_Stop();
				g_engine->p_device->EndScene();
				g_engine->p_device->Present(NULL, NULL, NULL, NULL);
			}			
			g_engine->save_to_map = false;
			g_engine->setEngineState(GAME);
			g_engine->setGameState(PLAY);			
			g_engine->gui->delete_GuiButtons();
			g_engine->delete_GameSprites();
			editor->pl_models.clear();
			g_engine->save_to_map = true;
			g_engine->map_current = 0;			
			if (!editor->Map_LoadSubMaps("campaign",editor->player_number)) break;			

			g_engine->gui->cursor->setImage(editor->Cursor_ingame);
			g_engine->gui->cursor->setFramesTotal(0);
			g_engine->gui->cursor->setFrameColumns(1);
			g_engine->gui->cursor->setFrameCurrent(0);
			g_engine->gui->cursor->setFrameTimer(0);
			g_engine->gui->cursor->setRotation(0);
			g_engine->gui->cursor->setFrameX(0);
			g_engine->gui->cursor->setFrameY(0);
			break;

		case BUTTON_2PLAYER:
			if(editor->player_number == 1)
			{
				editor->player_number = 2;
				button->setText("2 player");
			}
			else
			{
				editor->player_number = 1;
				button->setText("1 player");
			}
			break;

		case BUTTON_GAMECUSTOM:if(!g_engine->locked) editor->Map_LoadButtons(button,LOAD_LEVEL);break;

		case BUTTON_ANIMEDITOR:
			if(!g_engine->locked)
			{
				g_engine->gui->delete_GuiButtons();
				g_engine->delete_GameSprites();
				editor->pl_models.clear();
				g_engine->gui->cursor->setImage(editor->Cursor_Menu);
				g_engine->gui->cursor->setFramesTotal(0);
				g_engine->gui->cursor->setFrameColumns(1);
				g_engine->gui->cursor->setFrameCurrent(0);
				g_engine->gui->cursor->setFrameTimer(0);
				g_engine->gui->cursor->setRotation(0);
				g_engine->gui->cursor->setFrameX(0);
				g_engine->gui->cursor->setFrameY(0);
				g_engine->setEngineState(ANIMATION);
				animator->setAnimator();
			}
			break;

		case BUTTON_OPTIONS:
			g_engine->delete_GameSprites();
			editor->pl_models.clear();//Hmmmm !?
			editor->options->setOptions();
			editor->map_red = 0;
			editor->map_green = 0;
			editor->map_blue = 0;			
			break;		

		case LOAD_LEVEL:
			{
				g_engine->p_device->SetRenderTarget(0,g_engine->pBackBuffer);
				g_engine->ar_graphics->ClearScene(D3DCOLOR_XRGB(0,0,0));
				g_engine->p_device->BeginScene();	
				g_engine->ar_graphics->Render2D_Start();
				//
				g_engine->ar_graphics->draw_texture(
					editor->t_menu_loading,
					g_engine->getScreenWidth()/2-editor->t_menu_loading->getWidth()/2,g_engine->getScreenHeight()/2-editor->t_menu_loading->getHeight()/2,
					editor->t_menu_loading->getWidth(),editor->t_menu_loading->getHeight(),
					0,0,editor->t_menu_loading->getWidth(),editor->t_menu_loading->getHeight(),
					1,1,
					0xFFFFFFFF);
				//
				g_engine->ar_graphics->Render2D_Stop();
				g_engine->p_device->EndScene();
				g_engine->p_device->Present(NULL, NULL, NULL, NULL);
			}
			g_engine->setEngineState(GAME);
			g_engine->setGameState(PLAY);
			editor->pl_models.clear();
			g_engine->gui->delete_GuiButtons();
			g_engine->delete_GameSprites();			
			g_engine->save_to_map = true;
			g_engine->map_current = 0;			
			if (!editor->Map_LoadFromFile(button->getText(),editor->player_number)) break;
			if (!editor->Map_LoadSubMaps(button->getText(),editor->player_number)) break;			

			//if (!editor->Map_LoadFromFile(button->getText(),player_number)) break;
			g_engine->gui->cursor->setImage(editor->Cursor_ingame);
			g_engine->gui->cursor->setFramesTotal(0);
			g_engine->gui->cursor->setFrameColumns(1);
			g_engine->gui->cursor->setFrameCurrent(0);
			g_engine->gui->cursor->setFrameTimer(0);
			g_engine->gui->cursor->setRotation(0);
			g_engine->gui->cursor->setFrameX(0);
			g_engine->gui->cursor->setFrameY(0);
			break;		

		case BUTTON_EDITOR:if(!g_engine->locked) editor->setEditor(true);break;

		case BUTTON_QUIT:editor->addQuitButtons();break;
		case EXIT_PROGRAM:g_engine->Close();break;

		}
		break;
	}//switch


}//GUI_ButtonRelease

void GUI_ButtonUpdate(Button *button)
{
	switch(button->getType())
	{
	case FORM_AUTOHIDE:
		if(button->child)
		{
			//CHECK COLLISION
			bool visible =
				(g_engine->coll_manager->dot_dotrect_eq(
				g_engine->gui->cursor->getX(), g_engine->gui->cursor->getY(),
				button->getX(),button->getY(),button->getWidth(),button->getHeight())
				||
				(g_engine->coll_manager->dot_dotrect_eq(
				g_engine->gui->cursor->getX(), g_engine->gui->cursor->getY(),
				button->child->getX(),button->child->getY(),
				button->child->getWidth(),button->child->getHeight()) && button->child->getVisible())
				);

			//UPDATE VISIBILITY
			button->child->setVisible(visible);
			for(unsigned int i=0;i<button->child->form.size();i++)
			{
				button->child->form[i]->setVisible(visible);
			}
			for(unsigned int i=0;i<button->child->children.size();i++)
			{
				button->child->children[i]->setVisible(visible);
			}
		}
		break;
	}
}

/////////////////////
//  GUI RENDERING  //
/////////////////////

void GUI_RenderCursorButtonPress(Button *button)
{
	switch(button->getType())
	{
	case BUTTON_TOMAINMENU:
	case BUTTON_GAME:
	case BUTTON_1PLAYER:
	case BUTTON_2PLAYER:
	case BUTTON_OPTIONS:
	case BUTTON_EDITOR:
	case BUTTON_ANIMEDITOR:
	case BUTTON_QUIT:
		if(button->texture_vector.size()==2)
		{
			g_engine->ar_graphics->draw_texture(
				button->texture_vector[1],
				button->getX(),button->getY(),
				button->getWidth(),button->getHeight(),
				0,0,button->getWidth(),button->getHeight(),
				1,1,
				0xFFFFFFFF);		
		}
		break;

	default:
		g_engine->ar_graphics->draw_box(
			button->getX(),button->getY(),
			button->getWidth(),button->getHeight(),
			0,0,0xFF666666);
		break;
	}

}//GUI_RenderCursorButtonPress()

void GUI_RenderCursorOverButton(Button *button)
{
	button->cursor_inside = true;

	if(g_engine->locked)
	{
		switch(button->getType())
		{
		case BUTTON_EDITOR:
		case BUTTON_ANIMEDITOR:
		case BUTTON_GAMECUSTOM:
			g_engine->font_manager.fonts[0]->PrintText(
				"locked",
				button->getX()+button->getWidth(),
				button->getY(),
				1,1,0xFFFF0000);
			break;
		}
	}

	switch(button->getType())
	{
	case O_FULLSCREEN:
	case O_VSYNC:
	case O_RESOLUTION:
		g_engine->font_manager.fonts[0]->PrintText(
			"requires restart",
			button->getX()+button->getWidth(),
			button->getY(),
			1,1,0xFF00FF00);
		break;

	case BUTTON_GAME:
	case BUTTON_OPTIONS:
	case BUTTON_EDITOR:
	case BUTTON_ANIMEDITOR:
	case BUTTON_QUIT:
		if(button->texture_vector.size()==2)
		{
			g_engine->ar_graphics->draw_texture(
				button->texture_vector[1],
				button->getX(),button->getY(),
				button->getWidth(),button->getHeight(),
				0,0,button->getWidth(),button->getHeight(),
				1,1,
				0xFFFFFFFF);		
		}
		//ide na zivce kad ih vide brzo
		//if(button->cursor_inside && !button->cursor_insidepast) g_engine->audio->Play("menu2");
		break;

	case BUTTON_TEXTURE:
		if(!button->getImage()->getTilemap())
		{
			g_engine->ar_graphics->draw_box(
				button->getX(),button->getY(),
				button->getWidth(),button->getHeight(),
				0,0,0xFF0000AA);

			g_engine->ar_graphics->draw_texture(
				button->getImage(),
				button->getX(),button->getY(),
				button->getImage()->getWidth(),button->getImage()->getHeight(),
				0,0,button->getImage()->getWidth(),button->getImage()->getHeight(),
				1,1,
				0xFFFFFFFF);			
		}
		else
		{
			g_engine->ar_graphics->draw_box(
				editor->clipboard->getFramePositionX(
				button->getX(),
				button->getImage()->tile_width,
				button->getImage()->tile_offset_x),
				editor->clipboard->getFramePositionY(
				button->getY(),
				button->getImage()->tile_height,
				button->getImage()->tile_offset_y),
				button->getImage()->tile_width,
				button->getImage()->tile_height,
				0,0,
				0xFFAAAAAA);
		}
		break;

	default:
		g_engine->ar_graphics->draw_box(
			button->getX(),button->getY(),
			button->getWidth(),button->getHeight(),
			0,0,0xFFAAAAAA);
		break;
	}	

}//GUI_RenderCursorOverButton()

////////////
//  MISC  //
////////////

void game_end()
{
	std::string temp;
	time_t rawtime;
	time ( &rawtime );
	temp = ctime (&rawtime);

	g_engine->log_text.push_back("");	
	g_engine->log_text.push_back("MUG engine shutting down at: " + temp );	
	g_engine->save_log();

}//game_end()

void game_mouseMotion(int x,int y){}

void game_spritePostRender(Sprite *a)
{
	//treba ovo uopce ne pozivat u release bildu

	if(a->getID()==216)
	{
		float temp = g_engine->font_manager.fonts[0]->scale;
		g_engine->font_manager.fonts[0]->scale = 1;

		g_engine->font_manager.fonts[0]->PrintText(
			"Hello !",
			a->getX()-8+g_engine->getLayerOffsetX(a->getLayer()),
			a->getY()+a->h()+g_engine->getLayerOffsetY(a->getLayer()),
			1,1,0xFFFFFFFF);

		g_engine->font_manager.fonts[0]->PrintText(
			"HIB 3 STEAM KEY: XMMD9-YLGM9-5MB6H",
			a->getX()-8+g_engine->getLayerOffsetX(a->getLayer()),
			a->getY()+a->h()+2*(g_engine->font_manager.fonts[0]->getHeight()+1)+g_engine->getLayerOffsetY(a->getLayer()),
			1,1,0xFFFFFFFF);

		g_engine->font_manager.fonts[0]->PrintText(
			"UNLOCKING CODE: Wien",
			a->getX()-8+g_engine->getLayerOffsetX(a->getLayer()),
			a->getY()+a->h()+3*(g_engine->font_manager.fonts[0]->getHeight()+1)*editor->gui_scale+g_engine->getLayerOffsetY(a->getLayer()),
			1,1,0xFFFFFFFF);

		g_engine->font_manager.fonts[0]->PrintText(
			"Please contact me, so I can ",
			a->getX()-8+g_engine->getLayerOffsetX(a->getLayer()),
			a->getY()+a->h()+5*(g_engine->font_manager.fonts[0]->getHeight()+1)*editor->gui_scale+g_engine->getLayerOffsetY(a->getLayer()),
			1,1,0xFFFFFFFF);

		g_engine->font_manager.fonts[0]->PrintText(
			"tell the unlocking code to others.",
			a->getX()-8+g_engine->getLayerOffsetX(a->getLayer()),
			a->getY()+a->h()+6*(g_engine->font_manager.fonts[0]->getHeight()+1)*editor->gui_scale+g_engine->getLayerOffsetY(a->getLayer()),
			1,1,0xFFFFFFFF);

		g_engine->font_manager.fonts[0]->PrintText(
			"Tnx, AntonioR",
			a->getX()-8+g_engine->getLayerOffsetX(a->getLayer()),
			a->getY()+a->h()+7*(g_engine->font_manager.fonts[0]->getHeight()+1)*editor->gui_scale+g_engine->getLayerOffsetY(a->getLayer()),
			1,1,0xFFFFFFFF);

		g_engine->font_manager.fonts[0]->scale = temp;
	}
	else if(a->getID()==217)
	{
		g_engine->font_manager.fonts[0]->PrintText(
			a->getName(),
			a->cx()-g_engine->font_manager.fonts[0]->getLength(a->getName(),g_engine->font_manager.fonts[0]->char_dist,1)/2+g_engine->getLayerOffsetX(a->getLayer()),
			a->getY()-(g_engine->font_manager.fonts[0]->getHeight()+2)*editor->gui_scale+g_engine->getLayerOffsetY(a->getLayer()),
			1,1,a->getColor_Texture());
	}

	if(g_engine->display_lineofsight)
	{
		for(unsigned int i=0;i<a->children.size();i++)
		{
			/*g_engine->font_manager.fonts[0]->PrintText(
			g_engine->inttostring(a->children[i]->getUIGID()),
			a->children[i]->getX()+g_engine->getLayerOffsetX(a->children[i]->getLayer()),
			a->children[i]->getY()+g_engine->getLayerOffsetY(a->children[i]->getLayer()),
			1,1,0xFFFFFFFF);*/

			Line *temp_line0 = new Line(a->cx(), a->cy(), a->children[i]->cx(), a->children[i]->cy(), a->getLayer(), 0xFFFFFFFF);				
			g_engine->line_list.push_back(temp_line0);
		}

		if(a->getClass()==SPAWNER)
		{
			for(unsigned int i=0;i<a->page.size();i++)
			{
				g_engine->font_manager.fonts[0]->PrintText(
					a->page[i],
					a->getX()+g_engine->getLayerOffsetX(a->getLayer()),
					a->getY()+i*g_engine->font_manager.fonts[0]->getHeight()+g_engine->getLayerOffsetY(a->getLayer()),
					1,1,0xFFFF0000);
			}
		}
	}

	/*g_engine->font_manager.fonts[0]->PrintText(
	g_engine->inttostring(a->getState()),
	a->getX()+g_engine->getLayerOffsetX(a->getLayer()),
	a->getY()+g_engine->getLayerOffsetY(a->getLayer()),
	1,1,0xFFFF0000);*/


	/*for(unsigned int i=0;i<a->arg_list.size();i++)
	{
	g_engine->font_manager.fonts[0]->PrintText(
	a->getArg(i),
	a->getX()+g_engine->getLayerOffsetX(a->getLayer()),
	a->getY()+i*20+g_engine->getLayerOffsetY(a->getLayer()),
	1,1,0xFFFF0000);
	}*/

	/*g_engine->font_manager.fonts[0]->PrintText(
	g_engine->inttostring(a->getType()),
	a->getX()+g_engine->getLayerOffsetX(a->getLayer()),
	a->getY()+g_engine->getLayerOffsetY(a->getLayer())+10,
	1,1,0xFF00FF00);

	g_engine->font_manager.fonts[0]->PrintText(
	g_engine->inttostring(a->getID()),
	a->getX()+g_engine->getLayerOffsetX(a->getLayer()),
	a->getY()+g_engine->getLayerOffsetY(a->getLayer())+20,
	1,1,0xFF0088FF);*/
}