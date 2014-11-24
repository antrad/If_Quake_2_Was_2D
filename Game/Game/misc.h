#define BACKGROUND_0 0
#define BACKGROUND_1 1 
#define WORLD_BACK_0 2
#define WORLD_BACK_1 3
#define ENEMIES 4
#define PLAYER 5
#define PARTICLES_COLL 6
#define PARTICLES_NON 7
#define WORLD_FRONT_COLL 8
#define WORLD_FRONT_NON 9
#define SHADERSPRITES 10

#define GIBS 511

double ExplosionAngle(Sprite *object, Sprite *boom)//explosion from bottom
{
	return g_engine->math->toDegrees( g_engine->math->AngleToTarget(
		boom->getX()+boom->hw(),
		boom->getY()+boom->hh(),
		object->getX()+object->hw(),
		object->getY()+object->hh()
		));
}

double ExplosionAngleCenter(Sprite *object, Sprite *boom)
{
	return g_engine->math->toDegrees(
		g_engine->math->AngleToTarget(
		boom->cx(),boom->cy(),
		object->cx(),object->cy())
		);
}

void setlayers()
{
	for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
	{		
		std::list<Sprite*>::iterator iter = g_engine->game_sprites[i]->sprites.begin();
		while (iter != g_engine->game_sprites[i]->sprites.end())
		{
			Sprite *a = (Sprite*)*iter;
			if (a->getLayer()==BACKGROUND_0) a->setLayer(WORLD_BACK_0);			
			iter++;
		}
	}
}

void GibMe(Sprite *a, char gib_what, Sprite *boom, double power, char type)
{
	vector <Sprite*> s;
	double angle = 0;

	if (gib_what=='S') s = a->skeleton; 
	else s = a->attachments; 


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
		/*s[i]->rotateX = s[i]->hw();
		s[i]->rotateY = s[i]->hh();*/

		s[i]->coll_box.clear();
		g_engine->coll_manager->setColl_box(s[i]);
		s[i]->setCollisionMethod(BOX_RXYC);

		g_engine->coll_manager->collbox_update_RXYCbox(s[i]);
		g_engine->coll_manager->TestForCollisions_br(s[i],WORLD_FRONT_COLL);

		switch(type)
		{			       
		case 'B'://explode
			angle = g_engine->math->toDegrees(
				g_engine->math->AngleToTarget(
				boom->cx(),boom->cy(),
				s[i]->getX(),s[i]->getY()//prije bilo cx i cy, treba to usavrsit
				));
			s[i]->setVelocity(
				g_engine->math->LinearVelocityX(angle)*power,
				g_engine->math->LinearVelocityY(angle)*power
				);
			break;

		case 'P'://fly in projectile direction
			s[i]->setVelocity(
				boom->getVelocityX()*power,
				boom->getVelocityY()*power
				);
			break;

		case 'F'://fall apart
			if (s[i]->getVelocityX()==0) s[i]->setVelocityX(a->getVelocityX()*power);
			if (s[i]->getVelocityY()==0) s[i]->setVelocityY(a->getVelocityY()*power);
			break;
		}
	}	

	a->setAlive(false);
	g_engine->audio->Play("udeath");

}

void bullethitskeleton(Sprite *b,Sprite *s)
{
	//zbrog mrtvih soldiera
	return;

	for(unsigned int i=0;i<s->skeleton.size();i++)
	{
		if (g_engine->coll_manager->line_lineline(
			s->skeleton[i]->getX(),s->skeleton[i]->getY(),
			s->skeleton[i]->getAtt_posX(0),s->skeleton[i]->getAtt_posY(0),
			b->coll_box[0].getX()+b->getVelocityX(),b->coll_box[0].getY()+b->getVelocityY(),
			b->coll_box[2].getX()-b->getVelocityX(),b->coll_box[2].getY()-b->getVelocityY()))
		{
			s->skeleton[i]->setVelocity(
				s->skeleton[i]->getVelocityX()+b->getVelocityX()/8,
				s->skeleton[i]->getVelocityY()+b->getVelocityY()/8);
		}
	}

	for(unsigned int i=0;i<s->attachments.size();i++)
	{
		if (g_engine->coll_manager->line_lineline(
			s->attachments[i]->getX(),s->attachments[i]->getY(),
			s->attachments[i]->getAtt_posX(0),s->attachments[i]->getAtt_posY(0),
			b->coll_box[0].getX()+b->getVelocityX(),b->coll_box[0].getY()+b->getVelocityY(),
			b->coll_box[2].getX()-b->getVelocityX(),b->coll_box[2].getY()-b->getVelocityY()))
		{
			s->attachments[i]->setVelocity(
				s->attachments[i]->getVelocityX()+b->getVelocityX()/8,
				s->attachments[i]->getVelocityY()+b->getVelocityY()/8);
		}
	}
}

bool rayhitskeleton(Sprite *b, Sprite *s, float &x, float &y)
{
	for(unsigned int i=0;i<s->skeleton.size();i++)
	{
		if (g_engine->coll_manager->line_lineline(
			s->skeleton[i]->getX(),s->skeleton[i]->getY(),
			s->skeleton[i]->getAtt_posX(0),s->skeleton[i]->getAtt_posY(0),
			b->coll_box[0].getX()+b->getVelocityX(),b->coll_box[0].getY()+b->getVelocityY(),
			b->coll_box[2].getX()-b->getVelocityX(),b->coll_box[2].getY()-b->getVelocityY()))
		{
			g_engine->coll_manager->line_linelinexy(
				s->skeleton[i]->getX(),s->skeleton[i]->getY(),
				s->skeleton[i]->getAtt_posX(0),s->skeleton[i]->getAtt_posY(0),
				b->coll_box[0].getX()+b->getVelocityX(),b->coll_box[0].getY()+b->getVelocityY(),
				b->coll_box[2].getX()-b->getVelocityX(),b->coll_box[2].getY()-b->getVelocityY(),x,y);
			return true;
		}
	}

	return false;

}

void SpinInAir(Sprite *b, double angle)
{
	if (b->getVelocityX()>0)//move right, spin clockwise 
		b->setRotation(b->getRotation()+angle*g_engine->game_speed*g_engine->GetDeltaFix());
	else if (b->getVelocityX()<0)//move left, spin counter clockwise 
		b->setRotation(b->getRotation()-angle*g_engine->game_speed*g_engine->GetDeltaFix());	
}

void addGravity (Sprite *a)
{
	//prije 0.35/0.4
	if (abs(a->getVelocityY()<24))
		a->setVelocityY(a->getVelocityY()+0.5*g_engine->game_speed*g_engine->GetDeltaFix());	
}

void Print_JoystickState()
{
	std::vector <std::string> text;
	std::stringstream line;

	text.push_back("Joystick State :");

	for(unsigned int i=0;i<16;i++)
	{
		line << "B_" << i << " : " << bool(g_engine->p_input->js.rgbButtons[i] & 0x80);
		text.push_back(line.str());line.str("");
	}
	
	line << "Axis_X : " << g_engine->p_input->js.lX;
	text.push_back(line.str());line.str("");	

	line << "Axis_Y : " << g_engine->p_input->js.lY;
	text.push_back(line.str());line.str("");	

	line << "Axis_Z : " << g_engine->p_input->js.lZ;
	text.push_back(line.str());line.str("");	

	line << "Rot_X : " << g_engine->p_input->js.lRx;
	text.push_back(line.str());line.str("");	

	line << "Rot_Y : " << g_engine->p_input->js.lRy;
	text.push_back(line.str());line.str("");	

	line << "Rot_Z : " << g_engine->p_input->js.lRz;
	text.push_back(line.str());line.str("");

	g_engine->font_manager.fonts[0]->PrintText(text,g_engine->getScreenWidth()-250,250,1,1,0xFF00FFFF);

	/*g_engine->e_font->write_stringvector2(
			text,
			5,10,g_engine->getScreenWidth(),g_engine->getScreenHeight(),
			'L',0xFFFF00FF);*/

}

void PrintStuff(Sprite *IGRAC1)
{
	ostringstream text;
	std::vector <std::string> out;

	if (g_engine->getGameState()==PLAY && IGRAC1!=NULL)
	{
		text << IGRAC1->getBone("$HEAD")->getRotation() ;
		out.push_back(text.str());text.str("");

		text << "PLAYER_1 " ;
		out.push_back(text.str());text.str("");

		text << "pos_x: " << IGRAC1->getX();
		out.push_back(text.str());text.str("");

		text << "pos_y: " << IGRAC1->getY();
		out.push_back(text.str());text.str("");

		text << "vel_x: " << IGRAC1->getVelocityX();
		out.push_back(text.str());text.str("");

		text << "vel_y: " << IGRAC1->getVelocityY();
		out.push_back(text.str());text.str("");		

		text << "fire tick: " << IGRAC1->fire_tick;
		out.push_back(text.str());text.str("");

		text << "frame: " << IGRAC1->getFrameCurrent();
		out.push_back(text.str());text.str("");
		
		g_engine->font_manager.fonts[0]->PrintText(out,g_engine->getScreenWidth()-250,20,1,1,0xFFFF00FF);

		/*g_engine->e_font->write_stringvector2(
			out,
			g_engine->getScreenWidth()-200,20,g_engine->getScreenWidth(),g_engine->getScreenHeight(),
			'L',0xFFFFFFFF);*/
	}

}//PrintPlayerInfo

void ShowConsole()
{
	ostringstream text;
	std::vector <std::string> out;

	text << "GAME TITLE  [" << "Game v0.1 " << "]";
	out.push_back(text.str());text.str("");

	text << "AUTHOR  [" << "Antonio Radojkovic" << "]";
	out.push_back(text.str());text.str("");

	text << "ENGINE [" << "MUGEe v0.1" << "]";
	out.push_back(text.str());text.str("");

	text << "SCREEN RESOLUTION  [" << g_engine->getScreenWidth() << "x" << g_engine->getScreenHeight() << "]";
	out.push_back(text.str());text.str("");

	text << "COLOR DEPTH  [" << g_engine->getColorDepth() << "]";
	out.push_back(text.str());text.str("");

	text << "FPS  [" << fixed << setprecision(2) << g_engine->getFrameRate_FPS() << "]";
	out.push_back(text.str());text.str("");

	text << "Delta  [" << (int)g_engine->getTime_Delta() << "]";
	out.push_back(text.str());text.str("");

	text << "Entities  [" << (int)g_engine->getSpriteCount() << "]";
	out.push_back(text.str());text.str("");

	text << "Collidable  [" <<  (int)g_engine->getCollidableCounter() << "]";
	out.push_back(text.str());text.str("");

	text << "Collision checks [" << (int)g_engine->collisioncheck_counter << "]";
	out.push_back(text.str());text.str("");

	text << "OnGround checks [" << (int)g_engine->collisioncheck_counter_other << "]";
	out.push_back(text.str());text.str("");

	text << "InSector checks [" << (int)g_engine->count_check_insector << "]";
	out.push_back(text.str());text.str("");

	text << "OnScreen checks [" << (int)g_engine->count_check_onscreen << "]";
	out.push_back(text.str());text.str("");

	text << "Total checks [" << (int)g_engine->count_check_onscreen+(int)g_engine->count_check_insector + (int)g_engine->collisioncheck_counter_other + (int)g_engine->collisioncheck_counter << "]";
	out.push_back(text.str());text.str("");

	text << "Collision_LINE checks [" << (int)g_engine->lineline_counter << "]";
	out.push_back(text.str());text.str("");

	text << "Render Type [" << g_engine->getRenderType() << "]";
	out.push_back(text.str());text.str("");

	text << "Screen offset [" << (int)g_engine->offset_X << ","<< (int)g_engine->offset_Y <<"]";
	out.push_back(text.str());text.str("");

	text << "Buttons  [" << g_engine->gui->p_buttons.size() << "]";
	out.push_back(text.str());text.str("");

	text << "-------------------------------------------------------";
	out.push_back(text.str());text.str("");

	for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
	{
		text.str("");
		text << "L" << i << " entities [" << g_engine->game_sprites[i]->sprites.size() << "]";
		out.push_back(text.str());text.str("");
	}

	text << "MAPS [" << g_engine->game_maps.size() << "]";
	out.push_back(text.str());text.str("");

	/*for(unsigned int i=0;i<g_engine->game_maps[0]->sprite_layers.size();i++)
	{
		g_engine->game_maps[g_engine->map_current]->sprite_layers[i]->draw(TEXTURE);
	}*/
	
	for(unsigned int i=0;i<g_engine->game_maps.size();i++)
	{
		text.str("");
		text << "MAP [" << i << "]";

		for(unsigned int j=0;j<g_engine->game_maps[i]->sprite_layers.size();j++)
		{
			text << "L" << j << "[" << g_engine->game_maps[i]->sprite_layers[j]->sprites.size() << "]";
		}
		
		out.push_back(text.str());text.str("");
	}

	text << "channels playing [" << g_engine->audio->channels_playing.size() << "]";
	out.push_back(text.str());text.str("");

	int channels_playing=0;
	g_engine->audio->getSystem()->getChannelsPlaying(&channels_playing);

	text << "channels really  playing [" << channels_playing << "]";
	out.push_back(text.str());text.str("");

	text << "freq [" << g_engine->audio->getFrequencyGlobal() << "]";
	out.push_back(text.str());text.str("");

	g_engine->font_manager.fonts[0]->PrintText(out,5,10,1,g_engine->font_manager.fonts[0]->char_dist,0xFFFFFF00);

	g_engine->gui->console_panel->setHeight(out.size()*g_engine->e_font->size);
	g_engine->collisioncheck_counter = 0;
	g_engine->collisioncheck_counter_other = 0;
	g_engine->count_check_insector = 0;
	g_engine->count_check_onscreen = 0;
	g_engine->collision_counter = 0;

}//PrikaziKonzolu

std::string stuffEffect(int PixelShader_ID)
{
	switch (PixelShader_ID)
	{
	case -1:return "null";break;
	case 0:return "postprocess";break;
	case 1:return "nogreen";break;
	case 2:return "diagonalblur";break;
	case 3:return "verticalwaves";break;
	case 4:return "horizontalwaves";break;
	case 5:return "sharpen";break;
	case 6:return "embossed";break;
	case 7:return "colortubesv";break;
	case 8:return "colortubesh";break;
	case 9:return "simplifycolors";break;
	case 10:return "greyscale";break;
	case 11:return "blackwhite";break;
	case 12:return "swivel";break;
	case 13:return "flipx";break;
	case 14:return "flipy";break;
	case 15:return "light";break;
	default:return "postprocess";break;
	}
}

/*
//RENDER TO DARKNESS TEXTURE
	if(darkness)
	{
		g_engine->p_device->SetRenderTarget(0,editor->Player_managers[id]->screen->getDarkSurface());//0,0,0,0 skroz tamno
		g_engine->p_device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(0,0,255,0),1.0f,0); 
		
		g_engine->p_device->BeginScene();		
		g_engine->ar_graphics->Render2D_Start();

		//create dark
		g_engine->game_maps[g_engine->map_current]->sprite_layers[BACKGROUND_1]->DrawShadow();
		g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_0]->DrawShadow();
		g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_BACK_1]->DrawShadow();
		g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_COLL]->DrawShadow();
		g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_NON]->DrawShadow();
		//draw lights to texture
		g_engine->game_maps[g_engine->map_current]->sprite_layers[SHADERSPRITES]->draw(TEXTURE);

		g_engine->ar_graphics->Render2D_Stop();
		g_engine->p_device->EndScene();		
	}*/

/*
void Screen::DrawDark()
{
	D3DXMATRIX mat;
	D3DXVECTOR2 trans(this->screen_x,this->screen_y);
	D3DXMatrixTransformation2D(&mat,NULL,0,NULL,0,0,&trans);
	RECT srcRect = {0,0,this->screen_w,this->screen_h};	
	
	g_engine->getSpriteHandler()->SetTransform(&mat);
	g_engine->getSpriteHandler()->Draw(this->DarkTexture,&srcRect,NULL,NULL,0xFFFFFFFF);
}
*/

/*//darkness
	g_engine->p_device->CreateTexture(this->screen_w,this->screen_h,
		1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&this->DarkTexture,NULL);	
	this->DarkTexture->GetSurfaceLevel(0,&this->DarkSurface);*/