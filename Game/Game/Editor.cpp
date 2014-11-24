#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Editor.h"

namespace Advanced2D
{
	Editor::Editor()
	{
		this->player_number = 1;
		this->t_sound = NULL;
		this->t_spam = NULL;

		this->t_save = NULL;
		this->t_back = NULL;

		this->f1_tick = 0;
		this->display_f1 = false;

		this->gui_scale = 1;
		this->t_options_banner = NULL;
		this->t_video_banner = NULL;
		this->t_cursor_menu = NULL;

		this->t_action = NULL;

		this->t_menu_paused = NULL;
		this->t_menu_game0 = NULL;
		this->t_menu_game1 = NULL;
		this->t_menu_options0 = NULL;
		this->t_menu_options1 = NULL;
		this->t_menu_quit0 = NULL;
		this->t_menu_quit1 = NULL;
		this->t_menu_loading = NULL;
		this->t_menu_editor0 = NULL;
		this->t_menu_animator0 = NULL;
		this->t_menu_editor1 = NULL;
		this->t_menu_animator1 = NULL;
		this->t_banner_game = NULL;
		this->t_inventory = NULL;
		this->t_save = NULL;
		this->t_back = NULL;

		this->t_1player = NULL;

		this->t_areyousure = NULL;
		this->t_yes = NULL;
		this->t_no = NULL;

		this->t_consoleback = NULL;
		this->t_ingame_helpconsole = NULL;
		this->t_modelbox = NULL;

		this->Reset();
		this->sector_size = 2000;

		//edit stuff
		this->edit_sprite = NULL;
		this->edit_layer = NULL;
		this->options = NULL;		

		//textures
		this->player_start = NULL;		
		this->default_t = NULL;
		this->grid_t=NULL;
		this->tiled_t=NULL;	

		//BORDER STUFF
		this->map_border_t = NULL;		
		this->border_left = 0;
		this->border_right = 99999;
		this->border_up = 99999;
		this->border_down = -99999;

		//game managers
		this->World_manager = NULL;
		this->Particle_manager = NULL;
		this->GnaarM_manager = NULL;
		this->Biomech_manager = NULL;
		this->Beheaded_manager = NULL;
		this->Beshtiya_manager = NULL;

	}//Editor

	void Editor::Reset()
	{
		this->tiled_sprite = false;
		this->map_red = 0;
		this->map_green = 0;
		this->map_blue = 0;		

	}	

	bool Editor::LoadTextures()
	{
		this->t_inventory = g_engine->texture_manager->findTexture("Data/Textures/menu/inventory.png");
		if (this->t_inventory==NULL){g_engine->message("\"Data/Textures/menu/inventory.png\" not preloaded.");return false;}

		this->t_ingame_helpconsole = g_engine->texture_manager->findTexture("Data/Textures/menu/help.png");
		if (this->t_ingame_helpconsole==NULL){g_engine->message("\"Data/Textures/menu/help.png\" not preloaded.");return false;}

		this->t_options_banner = g_engine->texture_manager->findTexture("Data/Textures/menu/m_banner_options.png");
		if (this->t_options_banner==NULL){g_engine->message("\"Data/Textures/menu/m_banner_options.png\" not preloaded.");return false;}

		this->t_video_banner = g_engine->texture_manager->findTexture("Data/Textures/menu/m_banner_video.png");
		if (this->t_video_banner==NULL){g_engine->message("\"Data/Textures/menu/m_banner_video.png\" not preloaded.");return false;}

		this->t_menu_paused = g_engine->texture_manager->findTexture("Data/Textures/menu/pause.png");
		if (this->t_menu_paused == NULL){g_engine->message("\"Data/Textures/menu/pause.png\" not preloaded.");return false;}

		this->t_save = g_engine->texture_manager->findTexture("Data/Textures/menu/m_save.png");
		if (this->t_save == NULL){g_engine->message("\"Data/Textures/menu/m_save.png\" not preloaded.");return false;}

		this->t_back = g_engine->texture_manager->findTexture("Data/Textures/menu/m_back.png");
		if (this->t_back == NULL){g_engine->message("\"Data/Textures/menu/m_back.png\" not preloaded.");return false;}

		this->t_menu_game0 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_game.png");
		if (this->t_menu_game0 == NULL){g_engine->message("\"Data/Textures/menu/m_main_game.png\" not preloaded.");return false;}

		this->t_menu_game1 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_game_sel.png");
		if (this->t_menu_game1 == NULL){g_engine->message("\"Data/Textures/menu/m_main_game_sel.png\" not preloaded.");return false;}

		this->t_menu_options0 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_options.png");
		if (this->t_menu_options0 == NULL){g_engine->message("\"Data/Textures/menu/m_main_options.png\" not preloaded.");return false;}

		this->t_menu_options1 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_options_sel.png");
		if (this->t_menu_options1 == NULL){g_engine->message("\"Data/Textures/menu/m_main_options_sel.png\" not preloaded.");return false;}

		this->t_menu_quit0 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_quit.png");
		if (this->t_menu_quit0 == NULL){g_engine->message("\"Data/Textures/menu/m_main_quit.png\" not preloaded.");return false;}

		this->t_menu_quit1 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_quit_sel.png");
		if (this->t_menu_quit1 == NULL){g_engine->message("\"Data/Textures/menu/m_main_quit_sel.png\" not preloaded.");return false;}

		this->t_menu_loading = g_engine->texture_manager->findTexture("Data/Textures/menu/loading.png");
		if (this->t_menu_loading == NULL){g_engine->message("\"Data/Textures/menu/loading.png\" not preloaded.");return false;}

		this->t_menu_editor0 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_editor.png");
		if (this->t_menu_editor0 == NULL){g_engine->message("\"Data/Textures/menu/m_main_editor.png\" not preloaded.");return false;}

		this->t_menu_animator0 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_animator.png");
		if (this->t_menu_animator0 == NULL){g_engine->message("\"Data/Textures/menu/m_main_animator.png\" not preloaded.");return false;}

		this->t_menu_editor1 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_editor_sel.png");
		if (this->t_menu_editor1 == NULL){g_engine->message("\"Data/Textures/menu/m_main_editor_sel.png\" not preloaded.");return false;}

		this->t_menu_animator1 = g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_animator_sel.png");
		if (this->t_menu_animator1 == NULL){g_engine->message("\"Data/Textures/menu/m_main_animator_sel.png\" not preloaded.");return false;}

		this->t_cursor_menu = g_engine->texture_manager->findTexture("Data/Textures/menu/cursor.png");
		if (this->t_cursor_menu == NULL){g_engine->message("\"Data/Textures/menu/cursor.png\" not preloaded.");return false;}

		this->t_areyousure = g_engine->texture_manager->findTexture("Data/Textures/menu/areyousure.png");
		if (this->t_areyousure == NULL){g_engine->message("\"Data/Textures/menu/areyousure.png\" not preloaded.");return false;}

		this->t_yes = g_engine->texture_manager->findTexture("Data/Textures/menu/y.png");
		if (this->t_yes == NULL){g_engine->message("\"Data/Textures/menu/y.png\" not preloaded.");return false;}

		this->t_no = g_engine->texture_manager->findTexture("Data/Textures/menu/n.png");
		if (this->t_no == NULL){g_engine->message("\"Data/Textures/menu/n.png\" not preloaded.");return false;}

		this->t_1player = g_engine->texture_manager->findTexture("Data/Textures/menu/m_menu_1player.png");
		if (this->t_1player == NULL){g_engine->message("\"Data/Textures/menu/m_menu_1player.png\" not preloaded.");return false;}

		this->t_action = g_engine->texture_manager->findTexture("Data/Textures/action.png");
		if (this->t_action == NULL){g_engine->message("\"Data/Textures/action.png\" not preloaded.");return false;}

		//EDITOR STUFF
		this->t_sound = g_engine->texture_manager->findTexture("System/Editor/t_sound.png");
		if (this->t_sound == NULL){g_engine->message("\"System/Editor/t_sound.png\" not preloaded.");return false;}

		this->t_spam = g_engine->texture_manager->findTexture("System/Editor/t_spam.png");
		if (this->t_spam == NULL){g_engine->message("\"System/Editor/t_spam.png\" not preloaded.");return false;}

		grid_t = new Texture();
		if (!grid_t->Load("System/Editor/grid_t.png"))
		{
			g_engine->message("Error loading System/Editor/grid_t.png");
			return false;
		}

		tiled_t = new Texture();
		if (!tiled_t->Load("System/Editor/tiled_t.png"))
		{
			g_engine->message("Error loading System/Editor/tiled_t.png");
			return false;
		}

		player_start = new Texture();
		if (!player_start->Load("System/Editor/player_start.tga"))
		{
			g_engine->message("Error loading System/Editor/player_start.tga");
			return false;
		}

		//LEVEL BORDERS
		this->map_border_t = new Texture();
		if (!this->map_border_t->Load("System/Editor/map_border_t.png"))
		{
			g_engine->message("Error loading System/Editor/map_border_t.png");
			return false;
		}		
		//

		default_t = g_engine->gui->default_t;

		Cursor_Menu = new Texture();
		if (!Cursor_Menu->Load("System/GUI/Cursor_Menu.tga"))
		{
			g_engine->message("Error loading System/GUI/Cursor_Menu.tga");
			return false;
		}

		Cursor_ingame = new Texture();
		if (!Cursor_ingame->Load("System/GUI/Cursor_ingame.tga"))
		{
			g_engine->message("Error loading System/GUI/Cursor_ingame.tga");
			return false;
		}

		this->t_banner_game = g_engine->texture_manager->findTexture("Data/Textures/menu/m_banner_game.png");
		if (this->t_banner_game==NULL){g_engine->message("\"Data/Textures/menu/m_banner_game.png\" not preloaded.");return false;}

		this->t_consoleback = g_engine->texture_manager->findTexture("Data/Textures/menu/conback.png");
		if (this->t_consoleback==NULL){g_engine->message("\"Data/Textures/menu/conback.png\" not preloaded.");return false;}

		this->t_modelbox = g_engine->texture_manager->findTexture("Data/Textures/menu/modelbox.png");
		if (this->t_modelbox==NULL){g_engine->message("\"Data/Textures/menu/modelbox.png\" not preloaded.");return false;}

		if (g_engine->gui->console_panel==NULL)
		{
			g_engine->gui->console_panel = new Sprite();
			g_engine->gui->console_panel->setImage(t_consoleback);
			g_engine->gui->console_panel->setColor_Texture(0xAAFFFFFF);
			g_engine->gui->console_panel->setScale(
				(float)g_engine->getScreenWidth()/t_consoleback->getWidth(),
				(float)g_engine->getScreenHeight()/t_consoleback->getHeight());
		}

		//load cursor
		Sprite *cursor = new Sprite();		
		cursor->setImage(this->t_cursor_menu);
		cursor->setSize(22,29);		
		cursor->setFrameColumns(5);
		cursor->setFramesTotal(15);
		cursor->setFrameTimer_ms(100);
		g_engine->gui->cursor = cursor;

		if(!this->Map_ListLoad()) return false;
		if(!this->Map_ListSave()) return false;		

		return true;

	}//LoadTextures


	bool Editor::init()
	{
		this->edit_sprite = new EditSprite();
		this->edit_layer = new EditLayer();		
		this->clipboard = new Clipboard();
		this->World_manager = new World();

		if(!this->LoadGameAssets()) return false;

		g_engine->log_text.push_back("Loading player settings and stuff...");

		this->options = new Options();
		if (!this->options->LoadTextures()) return false;
		if (!this->options->loadSettings()) return false;
		if (!this->options->player_controls[0]->Load("System/config.txt")) return false;
		if (!this->options->player_controls[1]->Load("System/config.txt")) return false;		

		//PLAYER 1
		Player *player1 = new Player("Data/Textures/Players/Player1/",this->Player_managers.size());
		if (!player1->LoadTextures()) return false;
		this->Player_managers.push_back(player1);
		//PLAYER 2
		Player *player2 = new Player("Data/Textures/Players/Player2/",this->Player_managers.size());
		if (!player2->LoadTextures()) return false;
		player2->player_control = JOYSTICK;
		this->Player_managers.push_back(player2);

		for(unsigned int i=0;i<this->Player_managers.size();i++)
		{
			this->options->player_controls[i]->getPlayerControls(
				this->Player_managers[i]->LEFT,this->Player_managers[i]->RIGHT,
				this->Player_managers[i]->UP,this->Player_managers[i]->DOWN,
				this->Player_managers[i]->JUMP,
				this->Player_managers[i]->FIRE1,this->Player_managers[i]->FIRE2,
				this->Player_managers[i]->ACTION,this->Player_managers[i]->PDA,
				this->Player_managers[i]->C_FLY,this->Player_managers[i]->BULLETTIME,
				this->Player_managers[i]->NEXT_WEAPON,this->Player_managers[i]->PREV_WEAPON,
				this->Player_managers[i]->WEAPON0,this->Player_managers[i]->WEAPON1,
				this->Player_managers[i]->WEAPON2,this->Player_managers[i]->WEAPON3,
				this->Player_managers[i]->WEAPON4,this->Player_managers[i]->WEAPON5,
				this->Player_managers[i]->WEAPON6,this->Player_managers[i]->WEAPON7,
				this->Player_managers[i]->WEAPON8,this->Player_managers[i]->WEAPON9);
		}

		Particle_manager = new Particles();
		if (!Particle_manager->LoadTextures()) return false;

		g_engine->log_text.push_back("Loading enemy settings and stuff...");

		GnaarM_manager = new Gnaar();
		if (!GnaarM_manager->LoadTextures()) return false;		

		Beheaded_manager = new Beheaded();
		if (!Beheaded_manager->LoadTextures()) return false;

		Biomech_manager = new Biomech();
		if (!Biomech_manager->LoadTextures()) return false;

		Beshtiya_manager = new Beshtiya();
		if (!Beshtiya_manager->LoadTextures()) return false;

		return true;
	}


	void Editor::addPlayerStart(double x, double y)
	{
		Sprite *start = new Sprite();
		start->setType(PLAYER_START);
		start->setImage(player_start);
		start->setPosition(x,y);
		g_engine->addSprite(start,PLAYER);
	}

	void Editor::addSound(double x, double y)
	{
		Sprite *sound = new Sprite();
		sound->setClass(SOUND);
		sound->setType(PLATFORMA);
		sound->setImage(this->t_sound);
		sound->setRenderType(TILE);
		sound->setPosition(x,y);
		g_engine->addSprite(sound,LAYER_SOUND);
	}

	void Editor::addSpammer(double x, double y)
	{
		Sprite *sound = new Sprite();
		sound->setClass(SPAWNER);
		sound->setType(PLATFORMA);
		sound->setImage(this->t_spam);
		sound->setRenderType(TILE);
		sound->setPosition(x,y);
		g_engine->addSprite(sound,WORLD_FRONT_NON);
	}

	void Editor::addLevelBorder(double x, double y, std::string text)
	{
		Sprite *border_sprite = new Sprite();
		border_sprite->setType(LEVEL_BORDER);
		border_sprite->setImage(this->map_border_t);
		border_sprite->setPosition(x,y);
		border_sprite->setCollidable(false);
		border_sprite->setText(text);
		if(g_engine->getEngineState()==GAME) border_sprite->setVisible(false);

		if(text=="l")
		{	
			this->border_left = x;
		}
		else if(text=="r")
		{
			this->border_right = x;
		}
		else if(text=="u")
		{
			this->border_up = y;
		}
		else if(text=="d")
		{
			for(int i=0;i<this->Player_managers.size();i++)
			{
				this->Player_managers[i]->screen->border_down_vec.push_back(y);
				this->Player_managers[i]->screen->border_x_vec.push_back(x);				
			}

			g_engine->game_maps[g_engine->map_current]->border_down_vec.push_back(y);
			g_engine->game_maps[g_engine->map_current]->border_x_vec.push_back(x);

			if(y>g_engine->game_maps[g_engine->map_current]->map_bottom)
			{
				g_engine->game_maps[g_engine->map_current]->map_bottom = y;
				this->border_down = y;
			}
		}		

		g_engine->addSprite(border_sprite,WORLD_FRONT_NON);
		g_engine->game_maps[g_engine->map_current]->SetBorders(this->border_left,this->border_right,this->border_up,this->border_down);

	}//addLevelBorder()

	void Editor_UpdateSprites()
	{
		for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
		{			
			std::list<Sprite*>::iterator iter = g_engine->game_sprites[i]->sprites.begin();

			while (iter != g_engine->game_sprites[i]->sprites.end())
			{
				Sprite *a = *iter;
				if ( a->getAlive() ) 
				{
					a->animate();				
					if (a->getCollidable()) g_engine->collidable_counter++;				
				}
				++iter;
			} 
		}
	}

	void Editor::Draw()
	{
		//render sprites
		if (g_engine->getEngineState()==EDITOR)
		{
			Editor_UpdateSprites();

			g_engine->p_device->SetRenderTarget(0,g_engine->pBackBuffer);
			g_engine->ar_graphics->ClearScene(D3DCOLOR_XRGB(this->map_red,this->map_green,this->map_blue));

			if (g_engine->getRenderType()==TEXTURE || g_engine->getRenderType()==ALL)
			{
				for(int i=0;i<g_engine->game_sprites.size();i++)
				{
					if(g_engine->game_sprites[i]->getShader()!="#") g_engine->ar_graphics->Render2D_ShaderStart(g_engine->game_sprites[i]->getShader());
					g_engine->game_sprites[i]->draw(TEXTURE);
					if(g_engine->game_sprites[i]->getShader()!="#") g_engine->ar_graphics->Render2D_ShaderStop();
				}				
			}

			if (g_engine->getRenderType()==VECTOR || g_engine->getRenderType()==ALL)
			{
				for(int i=0;i<g_engine->game_sprites.size();i++)
					g_engine->game_sprites[i]->draw(VECTOR);
			}
		}		

		this->clipboard->Draw();

	}//draw

	void Editor::addMenuButtons()
	{
		g_engine->setEngineState(GAME);
		g_engine->setGameState(MENU);

		//reset gui, sprites, layers
		g_engine->delete_GameSprites();
		g_engine->gui->delete_GuiButtons();
		this->pl_models.clear();

		for(int i=0;i<g_engine->game_sprites.size();i++)
		{
			g_engine->game_sprites[i]->setVisible(true);
			g_engine->game_sprites[i]->setCollidable(true);
			g_engine->game_sprites[i]->setShader("");
		}		

		g_engine->gui->cursor->setRotation(g_engine->math->toRadians(-45));
		g_engine->gui->cursor->setImage(this->t_cursor_menu);
		g_engine->gui->cursor->setSize(22,29);
		g_engine->gui->cursor->setFrameColumns(5);
		g_engine->gui->cursor->setFramesTotal(15);
		g_engine->gui->cursor->setFrameTimer_ms(100);

		g_engine->gui->setSelectedButton(NULL);
		g_engine->gui->setSelectedSprite(NULL);

		this->map_red = 0;
		this->map_green = 0;
		this->map_blue = 0;

		g_engine->offset_X = 0;
		g_engine->offset_Y = 0;

		//MENU BUTTONS

		Button *b_game = new Button();
		b_game->setType(BUTTON_GAME);		
		b_game->texture_vector.push_back(this->t_menu_game0);
		b_game->texture_vector.push_back(this->t_menu_game1);
		b_game->setImage(this->t_menu_game0);
		b_game->setPosition(g_engine->getScreenWidth()/2 - b_game->getWidth()/2, g_engine->getScreenHeight()/2 - 100);
		g_engine->gui->addButton(b_game);

		Button *b_options = new Button();
		b_options->setType(BUTTON_OPTIONS);
		b_options->texture_vector.push_back(this->t_menu_options0);
		b_options->texture_vector.push_back(this->t_menu_options1);
		b_options->setImage(this->t_menu_options0);		
		b_options->setPosition(b_game->getX(),b_game->getY()+b_game->getHeight()+8);
		g_engine->gui->addButton(b_options);

		Button *b_menu_editor = new Button();
		b_menu_editor->setType(BUTTON_EDITOR);
		b_menu_editor->texture_vector.push_back(this->t_menu_editor0);
		b_menu_editor->texture_vector.push_back(this->t_menu_editor1);
		b_menu_editor->setImage(this->t_menu_editor0);		
		b_menu_editor->setPosition(b_game->getX(),b_options->getY()+b_options->getHeight()+8);		
		g_engine->gui->addButton(b_menu_editor);

		Button *b_menu_animator = new Button();
		b_menu_animator->setType(BUTTON_ANIMEDITOR);
		b_menu_animator->texture_vector.push_back(this->t_menu_animator0);
		b_menu_animator->texture_vector.push_back(this->t_menu_animator1);
		b_menu_animator->setImage(this->t_menu_animator0);
		b_menu_animator->setPosition(b_game->getX(),b_menu_editor->getY()+b_menu_editor->getHeight()+8);
		g_engine->gui->addButton(b_menu_animator);
		
		Button *b_menu_quit = new Button();
		b_menu_quit->setType(BUTTON_QUIT);
		b_menu_quit->texture_vector.push_back(this->t_menu_quit0);
		b_menu_quit->texture_vector.push_back(this->t_menu_quit1);
		b_menu_quit->setImage(this->t_menu_quit0);		
		b_menu_quit->setPosition(b_game->getX(),b_menu_animator->getY()+b_menu_animator->getHeight()+8);
		g_engine->gui->addButton(b_menu_quit);

		//MENU LOGOS
		Button *b_menu_plaque = new Button();
		b_menu_plaque->setImage(g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_plaque.png"));			
		b_menu_plaque->setPosition(b_game->getX()-b_menu_plaque->getWidth()-16, b_game->getY()-10);
		b_menu_plaque->clickable = false;
		g_engine->gui->addButton(b_menu_plaque);

		Button *b_menu_id = new Button();
		b_menu_id->setImage(g_engine->texture_manager->findTexture("Data/Textures/menu/m_main_logo.png"));			
		b_menu_id->setPosition(b_menu_plaque->getX()+b_menu_plaque->getWidth()/2-b_menu_id->getWidth()/2, b_menu_plaque->getY() + b_menu_plaque->getHeight() + 4);
		b_menu_id->clickable = false;
		g_engine->gui->addButton(b_menu_id);

		Button *b_menu_antonior64 = new Button();
		b_menu_antonior64->setImage(g_engine->texture_manager->findTexture("Data/Textures/menu/antonior64.png"));			
		b_menu_antonior64->setPosition(g_engine->getScreenWidth()/2-b_menu_antonior64->getWidth()/2,g_engine->getScreenHeight()-b_menu_antonior64->getHeight()-4);
		b_menu_antonior64->clickable = false;
		g_engine->gui->addButton(b_menu_antonior64);

	}

	void Editor::addMenu2Buttons()
	{
		//reset gui, sprites, layers
		g_engine->delete_GameSprites();
		g_engine->gui->delete_GuiButtons();		

		for(int i=0;i<g_engine->game_sprites.size();i++)
		{
			g_engine->game_sprites[i]->setVisible(true);
			g_engine->game_sprites[i]->setCollidable(true);
			g_engine->game_sprites[i]->setShader("");
		}

		g_engine->setEngineState(GAME);
		g_engine->setGameState(MENU);

		g_engine->gui->cursor->setImage(this->t_cursor_menu);
		g_engine->gui->cursor->setSize(22,29);		
		g_engine->gui->cursor->setFrameColumns(5);
		g_engine->gui->cursor->setFramesTotal(15);
		g_engine->gui->cursor->setFrameTimer_ms(100);

		g_engine->gui->setSelectedButton(NULL);
		g_engine->gui->setSelectedSprite(NULL);

		this->map_red = 0;
		this->map_green = 0;
		this->map_blue = 0;

		g_engine->offset_X = 0;
		g_engine->offset_Y = 0;

		//MENU BUTTONS
		Button *b_newgame = new Button();
		b_newgame->setType(BUTTON_1PLAYER);
		b_newgame->setImage(this->t_1player);
		b_newgame->setPosition(g_engine->getScreenWidth()/2 - b_newgame->getWidth()/2, g_engine->getScreenHeight()/2 - 50);
		g_engine->gui->addButton(b_newgame);

		Button *b_newgame2 = new Button();
		b_newgame2->setType(BUTTON_2PLAYER);
		b_newgame2->setImage(this->default_t);	
		b_newgame2->setText("1 player");
		this->player_number = 1;
		b_newgame2->font_png = true;
		b_newgame2->setSize(128*g_engine->font_manager.fonts[0]->getScale(),8+g_engine->font_manager.fonts[0]->getHeight()*g_engine->font_manager.fonts[0]->getScale());		
		b_newgame2->setColor_Texture(0x00000000);
		b_newgame2->setPosition(g_engine->getScreenWidth()/2 - b_newgame2->getWidth()/2,b_newgame->getY() + b_newgame->getHeight() + 24 );
		g_engine->gui->addButton(b_newgame2);

		Button *b_gamecustom = new Button();
		b_gamecustom->setType(BUTTON_GAMECUSTOM);
		b_gamecustom->setImage(this->default_t);	
		b_gamecustom->setText(".custom game.");
		b_gamecustom->font_png = true;
		b_gamecustom->setSize(128*g_engine->font_manager.fonts[0]->getScale(),8+g_engine->font_manager.fonts[0]->getHeight()*g_engine->font_manager.fonts[0]->getScale());		
		b_gamecustom->setColor_Texture(0x00000000);
		b_gamecustom->setPosition(g_engine->getScreenWidth()/2 - b_gamecustom->getWidth()/2,b_newgame2->getY() + b_newgame2->getHeight() + 24 );
		g_engine->gui->addButton(b_gamecustom);	

		Button *b_dif0 = new Button();
		b_dif0->setType(BUTTON_DIFFICULTY);
		b_dif0->setImage(this->default_t);	
		b_dif0->setText("easy");
		b_dif0->setState(0);
		b_dif0->font_png = true;
		b_dif0->setSize(128*g_engine->font_manager.fonts[0]->getScale(),8+g_engine->font_manager.fonts[0]->getHeight()*g_engine->font_manager.fonts[0]->getScale());		
		b_dif0->setColor_Texture(0x00000000);
		b_dif0->setPosition(g_engine->getScreenWidth()/2 - b_dif0->getWidth()/2,b_gamecustom->getY() + b_gamecustom->getHeight() + 24 );
		g_engine->gui->addButton(b_dif0);
		if(g_engine->difficulty == 0) b_dif0->setColor_Text(0xFF00FF00);

		Button *b_dif1 = new Button();
		b_dif1->setType(BUTTON_DIFFICULTY);
		b_dif1->setImage(this->default_t);	
		b_dif1->setText("normal");
		b_dif1->setState(1);
		b_dif1->font_png = true;
		b_dif1->setSize(128*g_engine->font_manager.fonts[0]->getScale(),8+g_engine->font_manager.fonts[0]->getHeight()*g_engine->font_manager.fonts[0]->getScale());
		b_dif1->setColor_Texture(0x00000000);
		b_dif1->setPosition(g_engine->getScreenWidth()/2 - b_dif1->getWidth()/2,b_dif0->getY() + b_dif0->getHeight() + 8 );
		g_engine->gui->addButton(b_dif1);
		if(g_engine->difficulty == 1) b_dif1->setColor_Text(0xFF00FF00);

		Button *b_dif2 = new Button();
		b_dif2->setType(BUTTON_DIFFICULTY);
		b_dif2->setImage(this->default_t);	
		b_dif2->setText("hard");
		b_dif2->setState(2);
		b_dif2->font_png = true;
		b_dif2->setSize(128*g_engine->font_manager.fonts[0]->getScale(),8+g_engine->font_manager.fonts[0]->getHeight()*g_engine->font_manager.fonts[0]->getScale());
		b_dif2->setColor_Texture(0x00000000);
		b_dif2->setPosition(g_engine->getScreenWidth()/2 - b_dif2->getWidth()/2,b_dif1->getY() + b_dif1->getHeight() + 8 );
		g_engine->gui->addButton(b_dif2);
		if(g_engine->difficulty == 2) b_dif2->setColor_Text(0xFF00FF00);

		b_dif0->children.push_back(b_dif1);
		b_dif0->children.push_back(b_dif2);

		b_dif1->children.push_back(b_dif0);
		b_dif1->children.push_back(b_dif2);

		b_dif2->children.push_back(b_dif1);
		b_dif2->children.push_back(b_dif0);

		//MENU BUTTONS
		Button *b_banner = new Button();
		b_banner->setImage(this->t_banner_game);
		b_banner->clickable = false;
		b_banner->setPosition(g_engine->getScreenWidth()/2 - b_banner->getWidth()/2, g_engine->getScreenHeight()/2 - 200);
		g_engine->gui->addButton(b_banner);

		Button *b_menu_quit = new Button();
		b_menu_quit->setType(BUTTON_TOMAINMENU);
		b_menu_quit->setImage(this->t_back);		
		b_menu_quit->setPosition(g_engine->getScreenWidth()/2 - b_menu_quit->getWidth()/2, g_engine->getScreenHeight() - 2*b_menu_quit->getHeight());
		g_engine->gui->addButton(b_menu_quit);	

		//PLAYER MODEL SELECTION	
		Button *sk_back0 = new Button();
		sk_back0->setImage(this->t_modelbox);
		sk_back0->clickable = false;
		sk_back0->font_png = true;
		sk_back0->setText("player 1");
		sk_back0->text_positioning = 'C';
		sk_back0->setColor_Text(0xFFFFFFFF);
		sk_back0->setPosition(g_engine->getScreenWidth()/2 - 300, g_engine->getScreenHeight()/2 - 100);
		g_engine->gui->addButton(sk_back0);

		Button *sk_back00 = new Button();
		sk_back00->setImage(this->t_modelbox);
		sk_back00->clickable = false;
		sk_back00->font_png = true;
		sk_back00->setText("player 2");
		sk_back00->text_positioning = 'C';
		sk_back00->setColor_Text(0xFFFFFFFF);
		sk_back00->setPosition(g_engine->getScreenWidth()/2 + (300-sk_back00->getWidth()), g_engine->getScreenHeight()/2 - 100);
		g_engine->gui->addButton(sk_back00);

		//add player models		
		this->pl_models.clear();
		this->pl_models.push_back(this->Player_managers[0]->addPlayer(b_newgame->getX()-100,b_newgame->getY()));
		this->pl_models.push_back(this->Player_managers[1]->addPlayer(b_newgame2->getX()+b_newgame2->getWidth()+100,b_newgame2->getY()));
		this->pl_models[1]->setX(this->pl_models[1]->getX()-this->pl_models[1]->w());
		//add player models
		for(unsigned int i=0;i<this->pl_models.size();i++)
		{
			this->pl_models[i]->setFramesTotal(this->Player_managers[i]->player_models[this->Player_managers[i]->model_id]->animation_run.totalframes);
			this->pl_models[i]->setFrameTimer(this->Player_managers[i]->player_models[this->Player_managers[i]->model_id]->animation_run.timer);	
			this->pl_models[i]->animate();
			if(this->pl_models[i]->getBone("$GUN1")) this->pl_models[i]->getBone("$GUN1")->setVisible(false);
			if(this->pl_models[i]->getBone("$GUN2")) this->pl_models[i]->getBone("$GUN2")->setVisible(false);
			this->Player_managers[i]->player_models[this->Player_managers[i]->model_id]->animation_run.setRotation(this->pl_models[i]);
			g_engine->skeleton_manager.Walk_Bipedal(this->pl_models[i]);
		}

		//p1
		{
			Button *temp1 = new Button();
			temp1->setImage(default_t);
			temp1->setType(PLAYERMODEL);
			temp1->setID(0);
			temp1->setText("<");
			temp1->setSize(25,20);
			temp1->setColor_Texture(0x00000000);
			temp1->setColor_Text(0xFF666666);
			temp1->setPosition(sk_back0->getX() + 16,sk_back0->getY()+sk_back0->getHeight()/2-temp1->getHeight()/2);
			g_engine->gui->addButton(temp1);		

			Button *temp2 = new Button();
			temp2->setImage(default_t);
			temp2->setType(PLAYERMODEL);
			temp2->setID(0);
			temp2->setText(">");
			temp2->setSize(25,20);
			temp2->setColor_Texture(0x00000000);
			temp2->setColor_Text(0xFF666666);
			temp2->setPosition(sk_back0->getX()+sk_back0->getWidth() - 16 - temp2->getWidth(), sk_back0->getY()+sk_back0->getHeight()/2-temp2->getHeight()/2);
			g_engine->gui->addButton(temp2);

			this->pl_models[0]->setX(sk_back0->getX()+sk_back0->getWidth()/2-this->pl_models[0]->hw());
			this->pl_models[0]->setY(sk_back0->getY()+sk_back0->getHeight()/2-this->pl_models[0]->hh());
		}

		//p2
		{
			Button *temp1 = new Button();
			temp1->setImage(default_t);
			temp1->setType(PLAYERMODEL);
			temp1->setID(1);
			temp1->setText("<");
			temp1->setSize(25,20);
			temp1->setColor_Texture(0x00000000);
			temp1->setColor_Text(0xFF666666);
			temp1->setPosition(sk_back00->getX() + 16,sk_back00->getY()+sk_back00->getHeight()/2-temp1->getHeight()/2);
			g_engine->gui->addButton(temp1);		

			Button *temp2 = new Button();
			temp2->setImage(default_t);
			temp2->setType(PLAYERMODEL);
			temp2->setID(1);
			temp2->setText(">");
			temp2->setSize(25,20);
			temp2->setColor_Texture(0x00000000);
			temp2->setColor_Text(0xFF666666);
			temp2->setPosition(sk_back00->getX()+sk_back00->getWidth() - 16 - temp2->getWidth(), sk_back00->getY()+sk_back0->getHeight()/2-temp2->getHeight()/2);
			g_engine->gui->addButton(temp2);

			this->pl_models[1]->setX(sk_back00->getX()+sk_back00->getWidth()/2-this->pl_models[1]->hw());
			this->pl_models[1]->setY(sk_back00->getY()+sk_back00->getHeight()/2-this->pl_models[1]->hh());
		}
	}

	void Editor::addQuitButtons()
	{
		//reset gui, sprites
		g_engine->delete_GameSprites();
		g_engine->gui->delete_GuiButtons();		

		//QUIT BUTTONS
		Button *b_newgame0 = new Button();
		//b_newgame->texture_vector.push_back(this->t_menu_game0);
		//b_newgame->texture_vector.push_back(this->t_menu_game1);
		b_newgame0->clickable = false;
		b_newgame0->setImage(this->t_consoleback);
		b_newgame0->setPosition(g_engine->getScreenWidth()/2 - b_newgame0->getWidth()/2, g_engine->getScreenHeight()/2 - b_newgame0->getHeight()/2);
		g_engine->gui->addButton(b_newgame0);

		Button *b_newgame = new Button();
		//b_newgame->texture_vector.push_back(this->t_menu_game0);
		//b_newgame->texture_vector.push_back(this->t_menu_game1);
		b_newgame->clickable = false;
		b_newgame->setImage(this->t_areyousure);
		b_newgame->setPosition(g_engine->getScreenWidth()/2 - b_newgame->getWidth()/2, g_engine->getScreenHeight()/2 - 70);
		g_engine->gui->addButton(b_newgame);

		Button *b_newgame2 = new Button();
		b_newgame2->setType(EXIT_PROGRAM);
		//b_newgame2->texture_vector.push_back(this->t_menu_game0);
		//b_newgame2->texture_vector.push_back(this->t_menu_game1);
		b_newgame2->setImage(this->t_yes);		
		b_newgame2->setPosition(g_engine->getScreenWidth()/2 - 100, g_engine->getScreenHeight()/2);
		g_engine->gui->addButton(b_newgame2);

		Button *b_newgame3 = new Button();
		b_newgame3->setType(BUTTON_TOMAINMENU);
		//b_newgame3->texture_vector.push_back(this->t_menu_game0);
		//b_newgame3->texture_vector.push_back(this->t_menu_game1);
		b_newgame3->setImage(this->t_no);		
		b_newgame3->setPosition(g_engine->getScreenWidth()/2 + (100-b_newgame3->getWidth()), g_engine->getScreenHeight()/2);
		g_engine->gui->addButton(b_newgame3);

	}

	void Editor::setEditor(bool real_editor)
	{		
		g_engine->setEngineState(EDITOR);

		g_engine->delete_GameSprites();
		g_engine->gui->delete_GuiButtons();
		this->pl_models.clear();

		g_engine->gui->cursor->setImage(this->Cursor_Menu);
		g_engine->gui->cursor->setFramesTotal(0);
		g_engine->gui->cursor->setFrameColumns(1);
		g_engine->gui->cursor->setFrameCurrent(0);
		g_engine->gui->cursor->setFrameTimer(0);
		g_engine->gui->cursor->setRotation(0);
		g_engine->gui->cursor->setFrameX(0);
		g_engine->gui->cursor->setFrameY(0);

		//TOOLBARS
		Button *menu_bar = new Button();
		menu_bar->setImage(default_t);		
		menu_bar->setPosition(0,0);
		menu_bar->setSize(g_engine->getScreenWidth(),28);
		menu_bar->clickable = false;
		menu_bar->setColor_Texture(0xFF666666);
		g_engine->gui->addButton(menu_bar);

		Button *left_toolbar = new Button();
		left_toolbar->setImage(default_t);		
		left_toolbar->setPosition(0,menu_bar->getY()+menu_bar->getHeight());
		left_toolbar->setSize(32+4,g_engine->getScreenHeight()-left_toolbar->getY());
		left_toolbar->clickable = false;
		left_toolbar->setColor_Texture(0xFF666666);
		g_engine->gui->addButton(left_toolbar);

		//MENU BAR BUTTONS
		int posx = left_toolbar->getX() + left_toolbar->getWidth();
		int posy = menu_bar->getY() + 2;
		int d = 4;		

		Button *save = new Button();
		save->setType(BUTTON_SAVE);
		save->setImage(default_t);
		save->setText("SAVE");
		save->setSize(60,24);
		save->setColor_Texture(0xFF0044CC);
		save->setPosition(posx,posy);
		g_engine->gui->addButton(save);		

		posx+=save->getWidth()+d;

		Button *load = new Button();
		load->setType(BUTTON_ELOAD);
		load->setImage(default_t);
		load->setText("LOAD");
		load->setSize(60,24);
		load->setColor_Texture(0xFF0044CC);
		load->setPosition(posx,posy);
		g_engine->gui->addButton(load);		

		posx+=load->getWidth()+d;

		Button *Clear = new Button();
		Clear->setType(BUTTON_CLEAR_ALL);
		Clear->setState(0);//0 add yes/no, 1 clear layer
		Clear->setImage(default_t);
		Clear->setText("CLEAR");
		Clear->setSize(60,24);
		Clear->setColor_Texture(0xFF0044CC);
		Clear->setPosition(posx,posy);
		g_engine->gui->addButton(Clear);

		posx += Clear->getWidth() + 4*d;

		//SHOW GRID
		Button *grid_b = g_engine->gui->add_checkbox(
			BUTTON_GRID,
			posx,posy,
			24,24,"","",grid_t,0);	
		this->clipboard->grid=false;

		posx+=grid_b->getWidth()+d;

		//GRID SIZE
		std::vector <string> grid_width;		
		grid_width.push_back("8");
		grid_width.push_back("16");		
		grid_width.push_back("32");	
		grid_width.push_back("64");
		grid_width.push_back("6");
		grid_width.push_back("12");
		grid_width.push_back("24");
		grid_width.push_back("48");
		Button *grid_size_button = g_engine->gui->add_dropbox(
			BUTTON_GRID_SIZE,
			posx,posy,
			30,24,
			grid_width);
		grid_size_button->setText("32");
		this->clipboard->grid_size=32;

		posx += grid_size_button->getWidth()+grid_size_button->form[0]->getWidth()+d;

		//TILE
		Button *tile_b = g_engine->gui->add_checkbox(
			BUTTON_TILED,
			posx,posy,
			24,24,"","",tiled_t,0);
		this->tiled_sprite=false;

		posx+=tile_b->getWidth()+d;

		//CURSOR COLLISION DIRECTION
		Button *cur_coll_b = g_engine->gui->add_checkbox(
			BUTTON_CURSORCOLLISION,
			posx,posy,
			30,24,"F-B","B-F",NULL,1);
		g_engine->gui->collision_order=-1;

		posx+=cur_coll_b->getWidth()+d;

		//treba napraviti fju za resetiranje editora
		this->clipboard->working_layer = 0;		

		std::vector <string> work_layer;
		work_layer.push_back("BACKGROUND_0");
		work_layer.push_back("BACKGROUND_1");
		work_layer.push_back("WORLD_BACK_0");
		work_layer.push_back("SHADOW_BACK");
		work_layer.push_back("ENEMIES");
		work_layer.push_back("PLAYER");
		work_layer.push_back("PARTICLES_COLL");
		work_layer.push_back("PARTICLES_NON");
		work_layer.push_back("WORLD_FRONT_COLL");
		work_layer.push_back("WORLD_FRONT_NON");
		work_layer.push_back("LIGHT");
		work_layer.push_back("LAYER_SOUND");
		Button *work_layer_b = g_engine->gui->add_dropbox(
			BUTTON_WORKINGLAYER,
			posx,posy,
			200,24,
			work_layer);

		posx += work_layer_b->getWidth()+d;

		//EXIT BUTTON
		Button *Exit = new Button();
		Exit->setType(BUTTON_EXIT_EDITOR);
		Exit->setImage(default_t);
		Exit->setText("X");
		Exit->setSize(24,24);
		Exit->setColor_Texture(0xFFCC0000);
		Exit->setPosition(
			menu_bar->getX()+menu_bar->getWidth()-Exit->getWidth()-2,
			menu_bar->getY()+2);
		g_engine->gui->addButton(Exit);	

		//other buttons
		this->edit_sprite->Init();
		this->edit_layer->Init();
		if(real_editor) this->edit_layer->Reset(this->map_red,this->map_green,this->map_blue);	

		this->addTextureButtons_normal(2,128);
	}

	//////////////////////
	//  GUI & CONTROLS  //
	//////////////////////

	void Editor::keyPress(int key)
	{
		if (g_engine->gui->getSelectedSprite()==NULL)
		{
			switch (key)
			{
				//MANUAL SCROLL
			case DIK_UP:g_engine->offset_Y+=15*g_engine->GetDeltaFix();break;
			case DIK_DOWN:g_engine->offset_Y-=15*g_engine->GetDeltaFix();break;
			case DIK_LEFT:g_engine->offset_X+=15*g_engine->GetDeltaFix();break;
			case DIK_RIGHT:g_engine->offset_X-=15*g_engine->GetDeltaFix();break;
			}
		}
		else
		{
			switch (key)
			{
				//ROTATE SPRITE
			case DIK_R:
				g_engine->gui->getSelectedSprite()->setRotation(
					g_engine->math->AngleToTarget(					
					g_engine->gui->getSelectedSprite()->cx(),
					g_engine->gui->getSelectedSprite()->cy(),
					g_engine->gui->cursor->cx()-g_engine->getLayerOffsetX(g_engine->gui->getSelectedSprite()->getLayer()),
					g_engine->gui->cursor->cy()-g_engine->getLayerOffsetY(g_engine->gui->getSelectedSprite()->getLayer())));
				break;
			}
		}
	}	

	void setlayers2()
	{
		for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
		{		
			std::list<Sprite*>::iterator iter = g_engine->game_sprites[i]->sprites.begin();
			while (iter != g_engine->game_sprites[i]->sprites.end())
			{
				Sprite *a = (Sprite*)*iter;
				if(a->getImage()!=NULL)
				{
					if(a->getImage()->getFilePath()=="BLACK.png") a->setLayer(WORLD_BACK_1);
				}
				iter++;
			}
		}
	}

	bool Search(std::string path)
	{
		for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
		{		
			std::list<Sprite*>::iterator iter2 = g_engine->game_sprites[i]->sprites.begin();
			while(iter2 != g_engine->game_sprites[i]->sprites.end())
			{
				if((*iter2)->getImage())
				{
					if((*iter2)->getImage()->getFilePath()==path) return true;
				}
				iter2++;
			}		
		}
		return false;
	}

	void Editor::keyRelease(int key)
	{
		Sprite *cursor = g_engine->gui->cursor;
		Sprite *cursor_att = g_engine->gui->getSelectedSprite();

		switch (key)
		{
		case DIK_F1:
			/*{
				std::ofstream fileout("textures.txt");

				{
					std::vector<Texture*>::iterator iter = g_engine->texture_manager->texture_layers[4]->texture_vector.begin();
					while(iter != g_engine->texture_manager->texture_layers[4]->texture_vector.end())
					{
						if(Search((*iter)->getFilePath())) fileout << (*iter)->getFilePath() << std::endl;
						iter++;
					}
				}
				{
					std::vector<Texture*>::iterator iter = g_engine->texture_manager->texture_layers[5]->texture_vector.begin();
					while(iter != g_engine->texture_manager->texture_layers[5]->texture_vector.end())
					{
						if(Search((*iter)->getFilePath())) fileout << (*iter)->getFilePath() << std::endl;
						iter++;
					}
				}
				{
					std::vector<Texture*>::iterator iter = g_engine->texture_manager->texture_layers[6]->texture_vector.begin();
					while(iter != g_engine->texture_manager->texture_layers[6]->texture_vector.end())
					{
						if(Search((*iter)->getFilePath())) fileout << (*iter)->getFilePath() << std::endl;
						iter++;
					}
				}

				fileout.close();
			}*/
			if(g_engine->gui->getSelectedSprite()) g_engine->gui->getSelectedSprite()->setLayer(WORLD_BACK_0);
			break;

		case DIK_HOME:
			if(g_engine->gui->getSelectedSprite())
			{
				if(g_engine->gui->getSelectedSprite()->getClass()==SPAWNER)
				{
					Sprite *spam = g_engine->gui->getSelectedSprite();

					for(unsigned int i=0;i<spam->children.size();i++)
					{
						Sprite *a = spam->children[i];

						if(a->getClass()==ENEMY)
						{
							std::stringstream stream0;

							stream0
								<< a->getClass() << " " <<  a->getType()
								<< " " <<  a->getID() << " " << a->getState() << " " << a->getUIGID() << " " << a->getRenderType() << " ";

							if(a->children.empty())
							{
								stream0 << "0";
								spam->page.push_back(stream0.str());
							}
							else
							{
								stream0 << a->children.size();
								spam->page.push_back(stream0.str());
								for(unsigned int i=0;i<a->children.size();i++)
								{
									std::stringstream stream1;
									stream1 << a->children[i]->getUIGID();
									spam->page.push_back(stream1.str());
								}
							}

							std::stringstream stream2;

							stream2 << (int)a->getX() << " " << (int)a->getY()
								<< " " << a->getScaleX() << " " << a->getScaleY()								
								<< " " << a->energy
								<< " " << a->difficulty;

							spam->page.push_back(stream2.str());
							spam->page.push_back(a->getText());
						}

						a->setAlive(false);
						for(unsigned int i=0;i<a->skeleton.size();i++)
						{
							a->skeleton[i]->setAlive(false);
						}

					}//for(children)

					spam->children.clear();
					g_engine->gui->setSelectedSprite(NULL);
				}
			}
			break;

		case DIK_BACKSPACE:
			if(g_engine->gui->getSelectedSprite())
			{
				for(unsigned int i=0;i<g_engine->gui->getSelectedSprite()->children.size();i++)
				{
					g_engine->gui->getSelectedSprite()->children[i] = NULL;
				}
				g_engine->gui->getSelectedSprite()->children.clear();
			}
			break;


			//COPY-CUT-PASTE
		case DIK_C:
			if (g_engine->p_input->GetKeyState(DIK_LCONTROL) & 0x80)
			{
				if (g_engine->p_input->GetKeyState(DIK_A) & 0x80)
				{
					clipboard->CopyAll("copy");
					clipboard->clipboard_box_x1.clear();
					clipboard->clipboard_box_y1.clear();
					clipboard->clipboard_box_x2.clear();
					clipboard->clipboard_box_y2.clear();
				}
				else
				{
					if (g_engine->gui->getSelectedSprite() != NULL)
						clipboard->Copy_sprite(g_engine->gui->getSelectedSprite(),"copy");
					else
						clipboard->Copy("copy",this->clipboard->working_layer,true);//rusi se zbog necega kad je false, a nisam napravio kocku
				}
			}
			break;

		case DIK_X:
			if (g_engine->p_input->GetKeyState(DIK_LCONTROL) & 0x80)
			{
				if (g_engine->p_input->GetKeyState(DIK_A) & 0x80)
					clipboard->CopyAll("cut");
				else if (g_engine->gui->getSelectedSprite()!=NULL)
					clipboard->Copy_sprite(g_engine->gui->getSelectedSprite(),"cut");
				else 
					clipboard->Copy("cut",this->clipboard->working_layer,true);
			}			
			break;

		case DIK_V:
			if (g_engine->p_input->GetKeyState(DIK_LCONTROL) & 0x80) clipboard->Paste();
			break;

			//SPRITES
		case DIK_Q:addSound(g_engine->gui->cursor->getX()-g_engine->offset_X,g_engine->gui->cursor->getY()-g_engine->offset_Y);break;
		case DIK_W:addSpammer(g_engine->gui->cursor->getX()-g_engine->offset_X,g_engine->gui->cursor->getY()-g_engine->offset_Y);break;
		case DIK_S:addPlayerStart(g_engine->gui->cursor->getX()-g_engine->offset_X,g_engine->gui->cursor->getY()-g_engine->offset_Y);break;
		case DIK_E:addLevelBorder(g_engine->gui->cursor->getX()-g_engine->offset_X,g_engine->gui->cursor->getY()-g_engine->offset_Y,"#");break;

		case DIK_DELETE:
			if (g_engine->p_input->GetKeyState(DIK_LCONTROL) & 0x80)
			{
				if (g_engine->p_input->GetKeyState(DIK_A) & 0x80)
					clipboard->DeleteAll();
				else
					clipboard->Delete(this->clipboard->working_layer);
				break;
			}
			else if (g_engine->gui->getSelectedSprite()!=NULL)
			{
				for(int i=0;i<g_engine->gui->getSelectedSprite()->skeleton.size();i++)
					g_engine->gui->getSelectedSprite()->skeleton[i]->setAlive(false);

				for(int i=0;i<g_engine->gui->getSelectedSprite()->attachments.size();i++)
					g_engine->gui->getSelectedSprite()->attachments[i]->setAlive(false);

				if (g_engine->gui->getSelectedSprite()->child!=NULL)
				{
					g_engine->gui->getSelectedSprite()->child->setAlive(false);
					g_engine->gui->getSelectedSprite()->child->parent=NULL;
					g_engine->gui->getSelectedSprite()->child=NULL;
				}
				if (g_engine->gui->getSelectedSprite()->parent!=NULL)
				{
					g_engine->gui->getSelectedSprite()->parent->setAlive(false);
					g_engine->gui->getSelectedSprite()->parent->child=NULL;
					g_engine->gui->getSelectedSprite()->parent=NULL;
				}

				g_engine->gui->getSelectedSprite()->setAlive(false);
				g_engine->gui->setSelectedSprite(NULL);
			}
			break;

		case DIK_NUMPAD0:
			this->Beheaded_manager->addGuard(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				GUARD_LIGHT,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD1:
			this->Beheaded_manager->addGuard(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				GUARD_SHOTGUN,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD2:
			this->Beheaded_manager->addGuard(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				GUARD_MACHINEGUN,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD3:
			this->Beheaded_manager->addGuard(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				ENFORCER,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD4:
			this->Beheaded_manager->addGuard(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				PARASITE,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD5:
			this->Beheaded_manager->addGuard(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				GUNNER,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD6:
			this->Beshtiya_manager->addBeshtiya(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				FLYER,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD7:
			this->Beshtiya_manager->addBeshtiya(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				FLIPPER,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD8:
			this->Biomech_manager->addBiomech(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				BIOMECH_MINOR,1,1,-1,0,"#");
			break;

		case DIK_NUMPAD9:
			this->Biomech_manager->addBiomech(
				g_engine->gui->cursor->getX()-g_engine->offset_X,
				g_engine->gui->cursor->getY()-g_engine->offset_Y,
				BIOMECH_MAJOR,1,1,-1,0,"#");
		}

		if (g_engine->gui->getSelectedSprite()!=NULL)
			switch (key)
		{
			//MOVE SELECTED SPRITE
			case DIK_UP:
				g_engine->gui->getSelectedSprite()->setY((int)g_engine->gui->getSelectedSprite()->getY()-1);
				break;

			case DIK_DOWN:
				g_engine->gui->getSelectedSprite()->setY((int)g_engine->gui->getSelectedSprite()->getY()+1);
				break;

			case DIK_LEFT:
				g_engine->gui->getSelectedSprite()->setX((int)g_engine->gui->getSelectedSprite()->getX()-1);
				break;

			case DIK_RIGHT:
				g_engine->gui->getSelectedSprite()->setX((int)g_engine->gui->getSelectedSprite()->getX()+1);
				break;

				//RESIZE TILED SPRITE
			case DIK_I:
				if(g_engine->gui->getSelectedSprite()->getRenderType()==TILESCROLLER_X)
				{
					if(!g_engine->gui->getSelectedSprite()->texture_vector.empty())
						g_engine->gui->getSelectedSprite()->texture_vector.push_back(g_engine->gui->getSelectedSprite()->texture_vector[0]);
					return;
				}

				if(cursor_att->getRenderType()==TILE)
					cursor_att->setWidth(
					(int)(cursor_att->getWidth()/cursor_att->getImage()->tile_width*cursor_att->getImage()->tile_width
					+cursor_att->getImage()->tile_width));
				break;

			case DIK_O:
				if(cursor_att->getRenderType()==TILE)
					cursor_att->setHeight(
					(int)(cursor_att->getHeight()/cursor_att->getImage()->tile_height*cursor_att->getImage()->tile_height
					+cursor_att->getImage()->tile_height));
				break;

			case DIK_J:
				if(g_engine->gui->getSelectedSprite()->getRenderType()==TILESCROLLER_X)
				{
					if(!g_engine->gui->getSelectedSprite()->texture_vector.empty())
						g_engine->gui->getSelectedSprite()->texture_vector.pop_back();
					return;
				}

				if(cursor_att->getRenderType()==TILE)
					cursor_att->setWidth((int)(cursor_att->getWidth()-cursor_att->getImage()->tile_width));
				if (cursor_att->getWidth()<=cursor_att->getImage()->tile_width)
					cursor_att->setWidth(cursor_att->getImage()->tile_width);
				break;

			case DIK_K:
				if(cursor_att->getRenderType()==TILE)
					cursor_att->setHeight((int)(cursor_att->getHeight()-cursor_att->getImage()->tile_height));
				if (cursor_att->getHeight()<=cursor_att->getImage()->tile_height)
					cursor_att->setHeight(cursor_att->getImage()->tile_height);
				break;

		}
	}	

	void Editor::mousePress(int button)
	{
		switch(button)
		{
		case 0:
			if (g_engine->p_input->GetKeyState(DIK_LCONTROL) & 0x80)
			{
				//ne radi kada je layer offset 0, selection box se ne siri
				if(g_engine->gui->cursor->getX()>g_engine->getScreenWidth()-32) g_engine->offset_X -= 10;
				if(g_engine->gui->cursor->getX()<64) g_engine->offset_X += 10;
				if(g_engine->gui->cursor->getY()>g_engine->getScreenHeight()-32) g_engine->offset_Y -= 10;
				if(g_engine->gui->cursor->getY()<64) g_engine->offset_Y += 10;

				this->clipboard->SelectionBox_position(g_engine->gui->cursor->getX(),g_engine->gui->cursor->getY());

				return;
			}			

			if (g_engine->p_input->GetKeyState(DIK_RCONTROL) & 0x80)
			{
				if(this->clipboard->grid)
				{
					if (!this->clipboard->dotcollision(
						this->clipboard->getGridX(
						g_engine->gui->cursor->getX(),this->clipboard->working_layer)+this->clipboard->grid_size/2,
						this->clipboard->getGridY(
						g_engine->gui->cursor->getY(),this->clipboard->working_layer)+this->clipboard->grid_size/2,
						this->clipboard->working_layer))
						clipboard->Paste();
					return;
				}				
			}

			g_engine->gui->CursorCollision();
			if (g_engine->gui->getSelectedSprite()!=NULL) this->clipboard->movesprite(g_engine->gui->getSelectedSprite());			
			break;

		case 2:
			g_engine->gui->setSelectedSprite(NULL);
			g_engine->gui->setSelectedButton(NULL);
			this->edit_sprite->Update_form(NULL);
			break;
		}
	}

	void Editor::mouseRelease(int button)
	{
		this->clipboard->mouse_pressed = false;

		switch(button)
		{
		case 0:
			g_engine->gui->setSelectedSprite(NULL);
			break;

		case 1:
			if(g_engine->gui->getSelectedSprite()==NULL)
				g_engine->gui->CursorCollision();
			else
				g_engine->gui->setSelectedSprite(NULL);
			break;
		}

	}//mouseRelease

	void Editor::mouseWheel(int wheel){}//mouseWheel

	void Editor::buttonPress(Button *button, int mousebutton)
	{
		switch(button->getGUI())//trebalo bi biti obrnuto
		{
		case EDITLAYER_CLASS:
			this->edit_layer->ButtonPress(button,mousebutton,this->map_red,this->map_green,this->map_blue);
			break;

		case EDITSPRITE_CLASS:
			this->edit_sprite->ButtonPress(button,mousebutton);
			break;
		}
	}

	void Editor::buttonRelease(Button *button, int mousebutton)
	{
		this->edit_sprite->ButtonRelease(button,mousebutton);
		this->edit_layer->ButtonRelease(button,mousebutton);

		switch (mousebutton)
		{
		case 0:
			switch (button->getType())
			{
			case BUTTON_GRID_SIZE:
				if (1==1)
				{
					int value;
					std::stringstream stream;
					stream << button->getText();
					if (stream >> value) this->clipboard->grid_size=value;
				}
				break;

			case BUTTON_GRID:
				this->clipboard->grid=(bool)button->getState();				
				break;

			case BUTTON_TILED:
				this->tiled_sprite=(bool)button->getState();				
				break;	

			case BUTTON_CURSORCOLLISION:
				if(button->getState()==1)
					g_engine->gui->collision_order=-1;
				else
					g_engine->gui->collision_order=1;
				break;

			case BUTTON_WORKINGLAYER:
				this->clipboard->working_layer=button->getID();				
				break;			

			case BUTTON_SAVE:
				this->Map_LoadButtons(button,MAP_SAVE);
				break;

			case MAP_SAVE:
				if (button->getState()==0)
				{
					Button *temp_button = g_engine->gui->form_add_yesno(
						"SAVE MAP","Overwrite existing map ?",250,200);
					temp_button->form[3]->setType(MAP_SAVE);
					temp_button->form[3]->setText(button->getText());
					temp_button->form[3]->setWidth(temp_button->form[3]->getWidth()+16);
					temp_button->form[3]->setX(temp_button->form[3]->getX()-16);
					temp_button->form[3]->setID(1);
					g_engine->gui->form_delete(button->parent);
				}
				else
				{
					this->Map_SaveToFile(button->getText());
					//parent botuna je forma, parent forme je textbox,parent textboxa je forma
					if(button->parent->parent) g_engine->gui->form_delete(button->parent->parent->parent);
					g_engine->gui->form_delete(button->parent);			
				}
				break;

			case BUTTON_ELOAD:this->Map_LoadButtons(button,MAP_EDIT);break;	

			case MAP_EDIT:
				this->Map_LoadFromFile(button->getText(),1);
				g_engine->gui->form_delete(button->parent);				
				break;

			case BUTTON_CLEAR_ALL:
				if(button->getState()==0)
				{
					Button *temp_button = g_engine->gui->form_add_yesno("CLEAR ALL","Delete all sprites?",250,200);
					temp_button->form[3]->setType(BUTTON_CLEAR_ALL);
					temp_button->form[3]->setState(1);
				}
				else
				{
					g_engine->gui->setSelectedSprite(NULL);
					g_engine->gui->setSelectedButton(NULL);			
					for(int i=0;i<g_engine->game_sprites.size();i++)
					{
						g_engine->game_sprites[i]->setShader("null");
						g_engine->game_sprites[i]->setOffsetMultiplierX(1);
						g_engine->game_sprites[i]->setOffsetMultiplierY(1);			
					}
					g_engine->delete_GameSprites();
					this->edit_sprite->Update_form(NULL);
					this->edit_layer->Reset(this->map_red,this->map_green,this->map_blue);
					g_engine->gui->form_delete(button->parent);
				}
				break;			

			case BUTTON_TEXTURE:this->addSprite_texture(button);break;
			case SELECT_TILEMAP:if (!button->texture_vector.empty()) button->child->setImage(button->texture_vector[0]);break;	

			case BUTTON_EXIT_EDITOR:
				g_engine->delete_GameSprites();
				g_engine->gui->delete_GuiButtons();

				g_engine->setEngineState(GAME);
				g_engine->setGameState(MENU);				

				g_engine->gui->setSelectedSprite(NULL);
				g_engine->gui->setSelectedButton(NULL);

				this->edit_sprite->setNULL();
				this->edit_layer->Reset(this->map_red,this->map_green,this->map_blue);
				this->edit_layer->setNULL();
				this->addMenuButtons();				

				for(int i=0;i<g_engine->game_sprites.size();i++)
				{
					g_engine->game_sprites[i]->setVisible(true);
					g_engine->game_sprites[i]->setCollidable(true);
					g_engine->game_sprites[i]->setShader("");
				}

				this->Reset();
				break;
			}
			break;
		}

	}//buttonRelease()	


	////////////
	//  MISC  //
	////////////

	void Editor::addTextureButtons_tilemap(int x,int y)
	{
		//kada bi minimizira ovu formu a da nijedna tekstura nije ucitana program bi se srusio

		Button *autohide = new Button();
		autohide->setType(FORM_AUTOHIDE);
		autohide->setImage(this->default_t);
		autohide->setText("Tm");
		autohide->setColor_Texture(0xFF888888);
		autohide->setSize(32,48);
		autohide->setPosition(x,y);		
		g_engine->gui->addButton(autohide);		

		int maxw = 0;
		int maxh = 0;

		//calculate form size (biggest tilemap)
		for(int k=0;k<g_engine->texture_manager->texture_layers.size();k++)
		{
			for(int i=0;i<g_engine->texture_manager->texture_layers[k]->texture_vector.size();i++)
			{
				//treba fja get texture(m,n)
				if (g_engine->texture_manager->texture_layers[k]->texture_vector[i]->getTilemap())
				{
					if(g_engine->texture_manager->texture_layers[k]->texture_vector[i]->getWidth()>maxw)
						maxw = g_engine->texture_manager->texture_layers[k]->texture_vector[i]->getWidth();

					if(g_engine->texture_manager->texture_layers[k]->texture_vector[i]->getHeight()>maxh)
						maxh = g_engine->texture_manager->texture_layers[k]->texture_vector[i]->getHeight();
				}
			}
		}

		//add form
		Button *panel = g_engine->gui->form_add("TILEMAPS",
			autohide->getX()+autohide->getWidth(),autohide->getY(),
			maxw+8+16,maxh+50+30,
			'L',false);

		panel->form[0]->setText("O");
		panel->form[0]->clickable=false;
		panel->form[0]->setCollidable(false);
		autohide->child = panel;

		double posx = panel->form[1]->getX()+2; 
		double posy = panel->form[1]->getY()+2;

		//tilemap big button
		Button *tile = new Button();
		panel->children.push_back(tile);
		g_engine->gui->addButton(tile);		

		posx = panel->form[1]->getX() + 2; 
		posy = panel->form[1]->getY() + panel->form[1]->getHeight() - 2 - 20;

		//texture for big button
		Texture *first_tilemap_texture = NULL;
		bool first_tilemap = false;

		int j = 0;
		for(int k=0;k<g_engine->texture_manager->texture_layers.size();k++)
		{
			for(int i=0;i<g_engine->texture_manager->texture_layers[k]->texture_vector.size();i++)
			{
				if (g_engine->texture_manager->texture_layers[k]->texture_vector[i]->getTilemap())
				{
					//save for big button
					if (!first_tilemap)
					{
						first_tilemap_texture = g_engine->texture_manager->texture_layers[k]->texture_vector[i];
						first_tilemap = true;
					}

					//select tilemap button
					Button *tilemap = new Button();
					//put the tilemap in buttons texture_vector
					tilemap->texture_vector.push_back(g_engine->texture_manager->texture_layers[k]->texture_vector[i]);

					tilemap->setType(SELECT_TILEMAP);
					tilemap->setImage(g_engine->gui->default_t);
					tilemap->setSize(20,20);
					tilemap->setPosition(posx,posy);			

					//button text (number)
					std::stringstream stream;
					stream << j;
					tilemap->setText(stream.str());

					tilemap->setColor_Texture(0xFF000088);
					tilemap->setColor_Text(0xFFFFFFFF);

					g_engine->gui->addButton(tilemap);
					panel->children.push_back(tilemap);	
					tilemap->child=tile;//connect with big button

					//positon for next button
					posx += tilemap->getWidth()+2;
					j++;
				}
			}
		}

		//if no tilemaps found delete buttons
		if(panel->children.size()==1)
		{
			autohide->setAlive(false);
			panel->setAlive(false);
			g_engine->gui->form_delete(panel);
			return;
		}

		if (first_tilemap_texture!=NULL)
		{
			//button image is first tilmap
			tile->setImage(first_tilemap_texture);
			tile->setType(BUTTON_TEXTURE);
			tile->setPosition(panel->form[1]->getX()+2,panel->form[1]->getY()+2);		
		}

	}//addTextureButtons_tilemap()

	void Editor::addTextureButtons_normal(int x,int y)
	{
		this->addTextureButtons_tilemap(x,y);

		y += 48 + 2;

		for(unsigned int j=0;j<g_engine->texture_manager->texture_layers.size();j++)
		{
			Button *autohide = new Button();
			autohide->setType(FORM_AUTOHIDE);
			autohide->setImage(this->default_t);
			autohide->setText("T"+g_engine->inttostring(j));
			autohide->setColor_Texture(0xFF888888);
			autohide->setSize(32,48);
			autohide->setPosition(x,y);		
			g_engine->gui->addButton(autohide);

			Button *panel =	g_engine->gui->form_add(
				"TEXTURES",
				autohide->getX()+autohide->getWidth(),28,
				750,g_engine->getScreenHeight()-28,
				'L',false);

			panel->form[0]->setText("O");
			panel->form[0]->clickable=false;
			panel->form[0]->setCollidable(false);
			autohide->child=panel;


			//hide
			panel->setVisible(false);
			for(unsigned int i=0;i<panel->form.size();i++)
			{
				panel->form[i]->setVisible(false);
			}


			double posx = panel->form[1]->getX() + 2; 
			double posy = panel->form[1]->getY() + 2;
			int x_counter = 0;

			int button_size = 35;


			for(unsigned int i=0;i<g_engine->texture_manager->texture_layers[j]->texture_vector.size();i++)
			{
				if (!g_engine->texture_manager->texture_layers[j]->texture_vector[i]->getTilemap())
				{
					Button *tile = new Button();
					tile->setType(BUTTON_TEXTURE);
					tile->setImage(g_engine->texture_manager->texture_layers[j]->texture_vector[i]);
					tile->setSize(button_size,button_size);
					tile->setVisible(false);

					if(tile->getImage()->getWidth()>button_size)
						tile->setScaleX((double)button_size/tile->getImage()->getWidth());
					if(tile->getImage()->getHeight()>button_size)
						tile->setScaleY((double)button_size/tile->getImage()->getHeight());

					tile->setPosition(posx+(x_counter*(button_size+2)),posy);
					g_engine->gui->addButton(tile);
					panel->children.push_back(tile);

					if (x_counter < 12)
						x_counter++;
					else
					{
						x_counter = 0;
						posy += button_size + 2;
					}
				}
			}

			y += autohide->getHeight()+2;
			g_engine->gui->form_fitpanel_w(panel);
		}
	}

	////////////////////////////
	//     LOAD & SAVE MAP    //
	////////////////////////////

	bool Editor::Map_ListLoad()
	{
		//UPDATE MAP LIST AND REFRESH MAP LIST FILE
		this->map_list.clear();		
		std::string filename = "Data/Maps/map_list.txt";

		//open the animation list file
		std::ifstream filein(filename.c_str());
		if(!filein.is_open())
		{
			g_engine->log_text.push_back("ERROR#MAP_LIST - > File " + filename + " not found !");
			return false;
		}

		std::string line;
		while(std::getline(filein,line))
		{
			std::string temp_string = "Data/Maps/" + line + ".txt";
			std::ifstream temp_file(temp_string.c_str());

			if(temp_file.is_open())//test if file really exists
			{
				this->map_list.push_back(line);
				temp_file.close();
			}
		}


		filein.close();		

		return true;

	}//Map_RefreshList()

	bool Editor::Map_ListSave()
	{
		//SAVE REFRESHED LIST
		std::string filename = "Data/Maps/map_list.txt";
		std::ofstream fileout(filename.c_str());
		if(!fileout.is_open())
		{
			g_engine->gui->messagebox_add("Error","File " + filename + " not created !",10);
			return false;
		}

		for(unsigned int i=0;i<this->map_list.size();i++)
		{
			fileout << this->map_list[i] << std::endl;
		}

		fileout.close();

		return true;

	}//Map_RefreshList()

	bool Editor::Map_ListOn(std::string filename)
	{
		for(unsigned int i=0;i<this->map_list.size();i++)
		{
			if(this->map_list[i]==filename) return true;
		}

		return false;

	}

	void Editor::Map_LoadButtons(Button *button,int button_type)
	{
		int pos_x = g_engine->getScreenWidth()/2-160;
		int pos_y = 0;

		if(button_type==MAP_EDIT || button_type==MAP_SAVE)
		{
			pos_x = button->getX();
			pos_y = button->getY() + button->getHeight();
		}

		std::string form_title = "LOAD MAP";
		if(button_type==MAP_SAVE) form_title = "SAVE MAP";

		Button *panel = g_engine->gui->form_add(
			form_title,
			pos_x,pos_y,320,200,
			'L',true);

		pos_x = panel->form[1]->getX() + 2*2;
		pos_y = panel->form[1]->getY() + 2*2;	

		if(button_type==MAP_SAVE)
		{
			Button *save_b = new Button();
			save_b->setClass(TEXTBOX);
			save_b->setType(MAP_SAVEAS);
			save_b->setState(0);//check overwrite
			save_b->setImage(default_t);
			save_b->setSize(300,g_engine->e_font->size);
			save_b->setColor_Texture(0xFFFFFFFF);
			save_b->setColor_Text(0xFF000000);
			save_b->setPosition(pos_x,pos_y);
			g_engine->gui->addButton(save_b);

			save_b->parent = panel;
			panel->form.push_back(save_b);

			pos_y += save_b->getHeight() + 2*2;		
		}		

		int i=0;
		bool load = true;

		//ADD BUTTONS
		for(unsigned int i=0;i<this->map_list.size();i++)
		{
			Button *level = new Button();
			level->setType(button_type);//EDIT_LEVEL || LOAD_LEVEL - postoji li uopce razlika izmedju ova dva tipa botuna-ima:polozaj
			level->setState(0);//check overwrite
			level->setImage(default_t);
			level->setText(this->map_list[i]);
			level->setSize(300,g_engine->e_font->size);
			level->setColor_Texture(0xFF888888);
			level->setPosition(pos_x,pos_y+i*(2+level->getHeight()));
			g_engine->gui->addButton(level);

			panel->children.push_back(level);
			level->parent = panel;
		}		

		//g_engine->gui->form_fitpanel_w(panel);
		g_engine->gui->form_fitpanel_h(panel);

	}//Map_LoadButtons()

	void UpdateUIGID()	
	{
		//u editoru su povezani preko pokazivaca
		int uigid = 0;		
		for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
		{
			std::list<Sprite*>::iterator iter = g_engine->game_sprites[i]->sprites.begin();
			while(iter != g_engine->game_sprites[i]->sprites.end())
			{
				(*iter)->setUIGID((int)uigid);
				uigid++;
				iter++;
			}
		}
	}

	void FindChild()
	{
		float uigid = 0;

		if(g_engine->getEngineState()==GAME)
		{
			for(unsigned int i=0;i<g_engine->game_maps[g_engine->map_current]->sprite_layers.size();i++)
			{
				std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[i]->sprites.begin();
				while(iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[i]->sprites.end())
				{
					for(unsigned int j=0;j<g_engine->game_maps[g_engine->map_current]->sprite_layers.size();j++)
					{
						std::list<Sprite*>::iterator iter2 = g_engine->game_maps[g_engine->map_current]->sprite_layers[j]->sprites.begin();
						while(iter2 != g_engine->game_maps[g_engine->map_current]->sprite_layers[j]->sprites.end())
						{
							for(unsigned int k=0;k<(*iter)->child_uigid.size();k++)
							{
								if((*iter)->child_uigid[k]==(*iter2)->getUIGID())
								{
									(*iter)->children.push_back((*iter2));//g_engine->audio->Play("Bfg__x1b");
								}
							}
							iter2++;				
						}
					}
					iter++;				
				}
			}
		}
		else if(g_engine->getEngineState()==EDITOR)
		{
			//u editoru su ionako povezani preko pokazivaca
			std::list<Sprite*>::iterator iter;
			for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
			{
				iter = g_engine->game_sprites[i]->sprites.begin();
				while (iter != g_engine->game_sprites[i]->sprites.end())
				{
					std::list<Sprite*>::iterator iter2;
					for(unsigned int j=0;j<g_engine->game_sprites.size();j++)
					{
						iter2 = g_engine->game_sprites[j]->sprites.begin();
						while (iter2 != g_engine->game_sprites[j]->sprites.end())
						{
							for(unsigned int k=0;k<(*iter)->child_uigid.size();k++)
							{
								if((*iter)->child_uigid[k]==(*iter2)->getUIGID())
								{
									(*iter)->children.push_back((*iter2));//g_engine->audio->Play("Bfg__x1b");
								}
							}

							iter2++;	
						}
					}
					iter++;				
				}
			}
		}
	}

	bool Editor::Map_LoadFromFile(std::string filename, int player_num)
	{
		for(int i=0;i<this->Player_managers.size();i++)
		{
			this->Player_managers[i]->screen->border_down_vec.clear();
			this->Player_managers[i]->screen->border_x_vec.clear();
		}

		//OPEN FILE
		std::string filename_fin = "Data/Maps/" + filename + ".txt";
		std::ifstream filein(filename_fin.c_str());		
		if(!filein.is_open())
		{
			g_engine->gui->messagebox_add("Error","File \"" + filename_fin + "\" not found !",10);
			return false;
		}		

		//store content of file to string vector and load entities
		std::vector <std::string> file_vec;
		std::string line;
		while(std::getline(filein,line)) file_vec.push_back(line);
		filein.close();		
		if(!this->Map_LoadEntitiesFromFile(file_vec,player_num))
		{
			return false;
		}		

		//LOAD LAYER DATA
		this->edit_layer->LayerData_load(filename_fin,this->map_red,this->map_green,this->map_blue);

		//CREATE SECTORS
		if(!g_engine->save_to_map)
		{
			//create sector grid
			g_engine->sectors.clear();

			for(int i=0;i<fabs(this->border_right-this->border_left)/this->sector_size;i++)
			{
				for(int j=0;j<fabs(this->border_down-this->border_up)/this->sector_size;j++)
				{
					Sector temp_sector;
					temp_sector.x = this->border_left + i*this->sector_size;
					temp_sector.y = this->border_up + j*this->sector_size;
					temp_sector.w = this->sector_size;
					temp_sector.h = this->sector_size;
					g_engine->sectors.push_back(temp_sector);
				}
			}

			//sector sprites
			for(int i=0;i<g_engine->sectors.size();i++)
			{
				std::list<Sprite*>::iterator iter = g_engine->game_sprites[WORLD_FRONT_COLL]->sprites.begin();
				while (iter != g_engine->game_sprites[WORLD_FRONT_COLL]->sprites.end())
				{
					Sprite *a = (Sprite*)*iter;

					if(a->getCollidable() && a->getType()==PLATFORMA)
					{
						if(g_engine->coll_manager->dot_dotrect_nocount(
							a->getX(),a->getY(),
							g_engine->sectors[i].x,g_engine->sectors[i].y,
							g_engine->sectors[i].w,g_engine->sectors[i].h))
						{
							g_engine->sectors[i].sprite_list.push_back(a);
						}
						else if(g_engine->coll_manager->dot_dotrect_nocount(
							a->getX()+a->w(),a->getY(),
							g_engine->sectors[i].x,g_engine->sectors[i].y,
							g_engine->sectors[i].w,g_engine->sectors[i].h))
						{
							g_engine->sectors[i].sprite_list.push_back(a);
						}
						else if(g_engine->coll_manager->dot_dotrect_nocount(
							a->getX()+a->w(),a->getY()+a->h(),					
							g_engine->sectors[i].x,g_engine->sectors[i].y,
							g_engine->sectors[i].w,g_engine->sectors[i].h))
						{
							g_engine->sectors[i].sprite_list.push_back(a);
						}
						else if(g_engine->coll_manager->dot_dotrect_nocount(
							a->getX(),a->getY()+a->h(),
							g_engine->sectors[i].x,g_engine->sectors[i].y,
							g_engine->sectors[i].w,g_engine->sectors[i].h))
						{
							g_engine->sectors[i].sprite_list.push_back(a);
						}
					}

					iter++;
				}
			}
		}
		else
		{
			g_engine->game_maps[g_engine->map_current]->setName(filename);

			//create sector grid
			g_engine->game_maps[g_engine->map_current]->sectors.clear();
			for(int i=0;i<fabs(this->border_right-this->border_left)/this->sector_size;i++)
			{
				for(int j=0;j<fabs(this->border_down-this->border_up)/this->sector_size;j++)
				{
					Sector temp_sector;
					temp_sector.x = this->border_left + i*this->sector_size;
					temp_sector.y = this->border_up + j*this->sector_size;
					temp_sector.w = this->sector_size;
					temp_sector.h = this->sector_size;
					g_engine->game_maps[g_engine->map_current]->sectors.push_back(temp_sector);
				}
			}

			//sector sprites
			for(int i=0;i<g_engine->game_maps[g_engine->map_current]->sectors.size();i++)
			{
				std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_COLL]->sprites.begin();
				while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_COLL]->sprites.end())
				{
					Sprite *a = (Sprite*)*iter;

					if(a->getCollidable() && a->getType()==PLATFORMA)
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

					iter++;
				}
			}
		}

		FindChild();		

		return true;

	}//Map_LoadFromFile()

	bool Editor::Map_LoadEntitiesFromFile(std::vector <std::string> file_vec, int player_num)
	{
		int my_class = 0;
		int type = 0;
		int id = 0;
		int uigid = 0;
		int state = 0;
		int render_type = 0;
		int x = 0;
		int y = 0;
		int w,h,layer,coll;		
		std::string line = "";	
		std::string text;

		for(unsigned int n=0;n<file_vec.size();n++)
		{
			float child = 0;
			std::vector <int> child_uigids;
			std::stringstream stream(file_vec[n]);	

			if(stream >> my_class >> type >> id >> state >> uigid >> render_type >> child)
			{
				Sprite *temp_sprite = NULL;

				if(child != 0)
				{
					for(unsigned int i=0;i<child;i++)
					{
						n++;
						std::string child_uigid = file_vec[n];
						child_uigids.push_back((int)g_engine->stringtoint(child_uigid));
					}
				}

				n++;
				std::string second_line = file_vec[n];
				std::stringstream stream2;

				if(my_class==ENEMY)
				{
					double scale_x = 0;
					double scale_y = 0;
					int energy = 0;
					int difficulty = 0;

					stream2 << second_line;
					if (stream2 >> x >> y >> scale_x >> scale_y >> energy >> difficulty)
					{
						n++;
						std::string third_line = file_vec[n];
						
						if(difficulty <= g_engine->difficulty || g_engine->getEngineState()==EDITOR)//load only if difficulty is OK
						{
							switch(type)
							{
							case GNAAR:temp_sprite = this->GnaarM_manager->addGnaar(x,y,id,scale_x,scale_y,energy,difficulty,third_line);break;
							case BIOMECH:temp_sprite = this->Biomech_manager->addBiomech(x,y,id,scale_x,scale_y,energy,difficulty,third_line);break;
							case BEHEADED:temp_sprite = this->Beheaded_manager->addGuard(x,y,id,scale_x,scale_y,energy,difficulty,third_line);break;
							case BESHTIYA:temp_sprite = this->Beshtiya_manager->addBeshtiya(x,y,id,scale_x,scale_y,energy,difficulty,third_line);break;
							}
						}
					}

					if(temp_sprite && child != 0)
					{
						temp_sprite->child_uigid = child_uigids;
					}					
				}

				switch(type)
				{
				case PLATFORMA:
					{
						if(render_type==TILESCROLLER_X)
						{
							stream2 << second_line;
							int tile_num = 0;						

							if(stream2 >> tile_num)
							{
								std::vector <string> textures;
								for(int i=0;i<tile_num;i++)
								{
									n++;
									textures.push_back(file_vec[n]);
								}

								n++;
								std::string third_line = file_vec[n];
								temp_sprite = this->World_manager->addTileScroller(my_class,type,id,state,second_line,textures,third_line);
							}							
						}
						else
						{
							n++;
							std::string third_line = file_vec[n];
							temp_sprite = this->World_manager->addSprite_string(my_class,type,id,state,second_line,third_line);
						}

						if(temp_sprite)
						{
							if(child != 0) temp_sprite->child_uigid = child_uigids;

							if(my_class == SPAWNER)
							{
								if(g_engine->getEngineState()==GAME) temp_sprite->setVisible(false);
								n++;
								int k = (int)g_engine->stringtoint(file_vec[n]);
								for(int i=0;i<k;i++)
								{
									n++;
									temp_sprite->page.push_back(file_vec[n]);
								}
								if(g_engine->getEngineState()==GAME) temp_sprite->setVisible(false);
							}
							else if(my_class == FLY)
							{
								this->Particle_manager->addFlies(temp_sprite);
							}
							else if(my_class == SKELETON)
							{
								if(g_engine->getEngineState()==GAME) temp_sprite->setVisible(false);
								else temp_sprite->setColor_Texture(0x88FFFFFF);
								g_engine->skeleton_manager.GetSkeleton(temp_sprite->getArg(0))->setSkeleton(temp_sprite);
								if(!temp_sprite->skeleton.empty())
								{
									temp_sprite->FlipAll_H(temp_sprite->flip_h);
									if(g_engine->animation_manager.getAnimation(temp_sprite->getArg(1)))
									{
										temp_sprite->setFramesTotal(g_engine->animation_manager.getAnimation(temp_sprite->getArg(1))->totalframes);
										temp_sprite->setFrameTimer_ms(g_engine->animation_manager.getAnimation(temp_sprite->getArg(1))->timer);
										g_engine->animation_manager.getAnimation(temp_sprite->getArg(1))->setRotation(temp_sprite);
									}								
								}
								g_engine->skeleton_manager.ConnectBones(temp_sprite);
							}
						}						
					}
					break;				

				case PLAYER_START:					
					stream2 << second_line;
					if (g_engine->getEngineState()==GAME)
					{
						if((g_engine->save_to_map && g_engine->map_current == 0) || !g_engine->save_to_map)
						{
							g_engine->offset_X=0;
							g_engine->offset_Y=0;

							if (stream2 >> x >> y)
							{
								if (player_num==2)
								{
									this->Player_managers[0]->active = true;
									this->Player_managers[1]->active = true;

									//P1
									this->Player_managers[0]->p_player = this->Player_managers[0]->addPlayer(x,y);								
									this->Player_managers[0]->UpdateSkeleton();
									this->Player_managers[0]->setWeaponTexture();
									//P2
									this->Player_managers[1]->p_player = this->Player_managers[1]->addPlayer(x+50,y);
									this->Player_managers[1]->UpdateSkeleton();
									this->Player_managers[1]->setWeaponTexture();								

									if (!this->options->s_splitscreen)//false==vertical
									{
										this->Player_managers[0]->screen_create(
											0,0,
											g_engine->getScreenWidth()/2,g_engine->getScreenHeight());
										this->Player_managers[1]->screen_create(
											g_engine->getScreenWidth()/2,0,
											g_engine->getScreenWidth()/2,g_engine->getScreenHeight());
									}
									else
									{
										this->Player_managers[0]->screen_create(
											0,0,
											g_engine->getScreenWidth(),g_engine->getScreenHeight()/2);
										this->Player_managers[1]->screen_create(
											0,g_engine->getScreenHeight()/2,
											g_engine->getScreenWidth(),g_engine->getScreenHeight()/2);

									}
								}							
								else
								{
									this->Player_managers[0]->active = true;
									this->Player_managers[1]->active = false;

									//P1
									this->Player_managers[0]->p_player = this->Player_managers[0]->addPlayer(x,y);
									this->Player_managers[0]->UpdateSkeleton();
									this->Player_managers[0]->setWeaponTexture();

									this->Player_managers[0]->screen_create(
										0,0,
										g_engine->getScreenWidth(),g_engine->getScreenHeight());
								}
							}
						}
						else if (stream2 >> x >> y)
						{
							this->addPlayerStart(x,y);
						}
					}
					else if (stream2 >> x >> y)
					{
						this->addPlayerStart(x,y);
					}
					break;//PLAYER_START

				case LEVEL_BORDER:					
					stream2 << second_line;
					if (stream2 >> x >> y >> text)
						this->addLevelBorder(x,y,text);
					break;//LEVEL_END
				}

				//set UIGID
				if(temp_sprite) temp_sprite->setUIGID(uigid);

			}//if

		}//for

		return true;

	}//Map_LoadEntitiesFromFile()

	bool Editor::Map_LoadSubMaps(std::string filename,int player_num)
	{
		//g_engine->audio->Play("jump1");

		//OPEN FILE
		std::string filename_fin = "Data/Maps/" + filename + ".txt";
		std::ifstream filein(filename_fin.c_str());		
		if(!filein.is_open())
		{
			g_engine->gui->messagebox_add("Error","File \"" + filename_fin + "\" not found !",10);
			return false;
		}		

		std::string line;
		while(std::getline(filein, line))
		{			
			std::stringstream stream(line);	

			if(stream.str() == "load_map")
			{
				std::string second_line;
				std::getline(filein, second_line);
				std::stringstream stream2(second_line);

				std::string map_name;
				int map_index = 0;

				if(stream2 >> map_name >> map_index)
				{
					g_engine->map_current = map_index;
					if(!this->Map_LoadFromFile(map_name,player_num))
					{
						g_engine->gui->messagebox_add("Error","Loading sub-map 0",10);
						return false;
					}
				}
				else
				{
					g_engine->gui->messagebox_add("Error","Loading sub-map 1",10);
				}
			}
		}

		g_engine->map_current = 0;
		filein.close();

		//g_engine->audio->Play("jump1");

		return true;

	}

	bool Editor::Map_SaveToFile(std::string filename)
	{
		UpdateUIGID();

		std::string filename_fin = "Data/Maps/" + filename + ".txt";		
		std::ofstream fileout(filename_fin.c_str());
		std::stringstream text;

		//random stuff
		fileout << "//random data" << std::endl;

		//save map color
		fileout << "map_color" << std::endl;
		fileout << this->map_red; 
		fileout << " " << this->map_green; 
		fileout << " " << this->map_blue << std::endl;

		//save map music
		fileout << "dark" << std::endl;
		fileout << this->edit_layer->darkness->getText() << std::endl;

		//save layer data
		fileout << "//layer data" << std::endl;
		for(int i=0;i<g_engine->game_sprites.size();i++)
		{
			fileout << "LAYER" << std::endl;
			fileout << i;			
			fileout << " " << g_engine->game_sprites[i]->getOffsetMultiplierX(); 
			fileout << " " << g_engine->game_sprites[i]->getOffsetMultiplierY();
			fileout << " " << g_engine->game_sprites[i]->getShader() << std::endl;
		}


		Sprite *a;
		std::list<Sprite*>::iterator iter;		

		for(int i=0;i<g_engine->game_sprites.size();i++)
		{
			fileout << "//layer " << i << std::endl;

			iter = g_engine->game_sprites[i]->sprites.begin();
			while (iter != g_engine->game_sprites[i]->sprites.end())
			{
				a = (Sprite*)*iter;

				//zbog te neke greske se ne upise nista
				if (a->getText()=="") a->setText("#");

				if (a->getAlive())  
				{
					if(a->getClass()==ENEMY)
					{
						//default (ne moze odma zbog -999 entiteta)
						fileout
							<< a->getClass() << " " <<  a->getType()
							<< " " <<  a->getID() << " " << a->getState() << " " << a->getUIGID() << " " << a->getRenderType() << " ";

						if(a->children.empty())
						{
							fileout << "0" << std::endl;
						}
						else
						{
							fileout << a->children.size() << std::endl;
							for(unsigned int i=0;i<a->children.size();i++)
							{
								fileout << a->children[i]->getUIGID() << std::endl;
							}
						}

						fileout << (int)a->getX() << " " << (int)a->getY()
							<< " " << a->getScaleX() << " " << a->getScaleY()								
							<< " " << a->energy
							<< " " << a->difficulty
							<< std::endl;

						fileout << a->getText() << std::endl;
					}
					else if (a->getType()==PLATFORMA)
					{
						//default (ne moze odma zbog -999 entiteta)
						fileout
							<< a->getClass() << " " <<  a->getType()
							<< " " <<  a->getID() << " " << a->getState() << " " << a->getUIGID() << " " << a->getRenderType() << " ";

						if(a->children.empty())
						{
							fileout << "0" << std::endl;
						}
						else
						{
							fileout << a->children.size() << std::endl;
							for(unsigned int i=0;i<a->children.size();i++)
							{
								fileout << a->children[i]->getUIGID() << std::endl;
							}
						}
						//

						if(a->getRenderType()==TILESCROLLER_X)//ne ucita se za negativni scrollx
						{
							//number of textures
							fileout
								<< " " << a->texture_vector.size()
								<< " " << (int)a->getX() << " " << (int)a->getY()
								<< " " << a->getLayer() << " " << a->getCollidable()
								<< " " << a->getScrollX() << " " << a->getScrollY()
								<< " " << a->getScrollStepX() << " " << a->getScrollStepY()
								<< " " << g_engine->ar_graphics->getAlpha(a->getColor_Texture())
								<< " " << g_engine->ar_graphics->getRed(a->getColor_Texture())
								<< " " << g_engine->ar_graphics->getGreen(a->getColor_Texture())
								<< " " << g_engine->ar_graphics->getBlue(a->getColor_Texture())
								<< std::endl;

							//texture paths
							for(unsigned int i=0;i<a->texture_vector.size();i++)
							{
								fileout << a->texture_vector[i]->getFilePath() << std::endl;
							}

							fileout << a->getText() << std::endl;
						}
						else
						{
							fileout << (int)a->getX() << " " << (int)a->getY()
								<< " " << a->getWidth() << " " << a->getHeight()
								<< " " << a->getLayer() << " " << a->getCollidable()
								<< " " << a->getRenderType() << " " << a->getScaleX() << " " << a->getScaleY() << " " << a->getRotation()
								<< " " << a->getImage()->getFilePath()
								<< " " << a->getFrameCurrent() << " " << a->getFrameRelX() << " " << a->getFrameRelY()
								<< " " << a->getScrollX() << " " << a->getScrollY()
								<< " " << a->getScrollStepX() << " " << a->getScrollStepY()
								<< " " << a->getOffsetMX() << " " << a->getOffsetMY()
								<< " " << a->flip_h << " " << a->flip_v 
								<< " " << a->getVelocityX() << " " << a->getVelocityY()
								<< " " << a->energy
								<< " " << g_engine->ar_graphics->getAlpha(a->getColor_Texture())
								<< " " << g_engine->ar_graphics->getRed(a->getColor_Texture())
								<< " " << g_engine->ar_graphics->getGreen(a->getColor_Texture())
								<< " " << g_engine->ar_graphics->getBlue(a->getColor_Texture())
								<< std::endl;

							fileout << a->getText() << std::endl;
						}

						if(a->getClass()==SPAWNER)
						{
							fileout << a->page.size() << std::endl;
							for(unsigned int i=0;i<a->page.size();i++)
							{
								fileout << a->page[i] << std::endl;
							}
						}
					}
					else if (a->getType()==PLAYER_START)
					{
						//default (ne moze odma zbog -999 entiteta)
						fileout
							<< a->getClass() << " " <<  a->getType()
							<< " " <<  a->getID() << " " << a->getState() << " " << a->getUIGID() << " " << a->getRenderType() << " ";

						if(a->children.empty())
						{
							fileout << "0" << std::endl;
						}
						else
						{
							fileout << a->children.size() << std::endl;
							for(unsigned int i=0;i<a->children.size();i++)
							{
								fileout << a->children[i]->getUIGID() << std::endl;
							}
						}
						//
						fileout << (int)a->getX() << " " << (int)a->getY() << std::endl;
					}
					else if (a->getType()==LEVEL_BORDER)
					{
						//default (ne moze odma zbog -999 entiteta)
						fileout
							<< a->getClass() << " " <<  a->getType()
							<< " " <<  a->getID() << " " << a->getState() << " " << a->getUIGID() << " " << a->getRenderType() << " ";

						if(a->children.empty())
						{
							fileout << "0" << std::endl;
						}
						else
						{
							fileout << a->children.size() << std::endl;
							for(unsigned int i=0;i<a->children.size();i++)
							{
								fileout << a->children[i]->getUIGID() << std::endl;
							}
						}
						//
						fileout << (int)a->getX() << " " << (int)a->getY() << " " << a->getText() << std::endl;
					}					
				}
				iter++;
			}
		}//for

		fileout.close();

		if(!this->Map_ListOn(filename)) this->map_list.push_back(filename);
		this->Map_ListSave();

		return true;

	}//Map_SaveToFile()

	void Editor::addSprite_texture(Button *button)
	{
		int x, y, w, h, render_type;

		int frame = 0;

		if(button->getImage()->getTilemap())
			frame = this->clipboard->getFrame(
			button->getX(),button->getY(),
			button->getImage()->tile_width,button->getImage()->tile_height,
			button->getImage()->tile_columns,
			button->getImage()->tile_offset_x,button->getImage()->tile_offset_y);

		x = this->clipboard->getFramePositionX(
			button->getX(),
			button->getImage()->tile_width,
			button->getImage()->tile_offset_x)-g_engine->getLayerOffsetX(this->clipboard->working_layer);

		y = this->clipboard->getFramePositionY(
			button->getY(),
			button->getImage()->tile_height,
			button->getImage()->tile_offset_y)-g_engine->getLayerOffsetY(this->clipboard->working_layer);

		if (g_engine->gui->getSelectedSprite()==NULL)//add new sprite
		{
			if (this->tiled_sprite)
			{
				if (g_engine->p_input->GetKeyState(DIK_LCONTROL) & 0x80)
				{
					x = this->clipboard->getX1(this->clipboard->working_layer);
					y = this->clipboard->getY1(this->clipboard->working_layer);
					w = this->clipboard->getX2(this->clipboard->working_layer)-this->clipboard->getX1(this->clipboard->working_layer);
					h = this->clipboard->getY2(this->clipboard->working_layer)-this->clipboard->getY1(this->clipboard->working_layer);
				}
				else
				{
					w = button->getImage()->tile_width;
					h = button->getImage()->tile_height;
				}
				render_type = TILE;
			}
			else
			{
				w = button->getImage()->tile_width;
				h = button->getImage()->tile_height;

				render_type = NORMAL;
			}

			if(w==0 || h==0)
			{
				g_engine->gui->messagebox_add("ERROR","Clipboard w || h == 0 !",10);
				return;
			}

			this->World_manager->addSprite_basic(
				PLATFORMA,x,y,w,h,
				this->clipboard->working_layer,render_type,
				button->getImage()->getFilePath(),frame);
		}
		else//sprite selected
		{
			Sprite *a = g_engine->gui->getSelectedSprite();			

			if(a->getRenderType()==TILESCROLLER_X)//add a texture to tilescroller
				a->texture_vector.push_back(button->getImage());
			else
			{
				w = a->w();
				h = a->h();

				a->setImage(button->getImage());//change texture
				a->setFrameColumns(button->getImage()->tile_columns);
				a->setFrameCurrent(frame);

				//if(a->getRenderType()!=NORMAL)
				//{
				a->setSize(w,h);//keep the tiled size
				a->setScale(1,1);
				//}				
			}
		}

	}//addSprite_texture()

	bool Editor::LoadGameAssets()
	{
		g_engine->log_text.push_back("Loading game assets...");

		std::ifstream filein("Data/game_assets.txt");
		if(!filein.is_open())
		{
			g_engine->log_text.push_back("Missing file \"Data/game_assets.txt\"");
			return false;
		}

		std::string line;
		while(std::getline(filein,line))
		{
			std::stringstream command(line);

			std::string arg0;
			command >> arg0;

			if(arg0=="load")
			{
				std::string arg1;
				command >> arg1;

				if(arg1=="texture_list")
				{
					std::string path;
					int index;

					if(command >> path >> index)
					{
						if (!g_engine->texture_manager->loadTextures(path,index)) return false;
					}
				}
				else if(arg1=="sound_list")
				{
					std::string path;
					int index;

					if(command >> path)
					{
						//audio files are not crucial
						g_engine->audio->Load_SoundList(path);
					}
				}				
				else if(arg1=="font")
				{
					std::string path;

					if(command >> path)
					{
						//return false ???
						g_engine->font_manager.LoadFont(path,path);
					}
				}
				else if(arg1=="skeleton_list")
				{
					std::string path;

					if(command >> path)
					{
						if (!g_engine->skeleton_manager.Load_SkeletonList(path)) return false;
					}
				}
				else if(arg1=="animation_list")
				{
					std::string path;

					if(command >> path)
					{
						if (!g_engine->animation_manager.Load_AnimationList(path)) return false;
					}
				}
			}			
		}

		filein.close();

		g_engine->log_text.push_back("Loading game assets...COMPLETED");

		return true;

	}

	void Editor::SortBorders()
	{
		/*for(unsigned int i=0;i<this->Player_managers.size();i++)
		{
		this->Player_managers[i]->screen->border_down_vec.push_back(y);
		this->Player_managers[i]->screen->border_x_vec.push_back(x);				
		}

		g_engine->game_maps[g_engine->map_current]->border_down_vec.push_back(y);
		g_engine->game_maps[g_engine->map_current]->border_x_vec.push_back(x);*/
	}

	void Editor::DrawF1()
	{
		int x = g_engine->getScreenWidth()/2 - this->gui_scale*this->t_ingame_helpconsole->getWidth()/2;
		int y = g_engine->getScreenHeight()/2 - this->gui_scale*this->t_ingame_helpconsole->getHeight()/2;

		g_engine->ar_graphics->draw_texture(
			this->t_ingame_helpconsole,
			x,y,
			this->t_ingame_helpconsole->getWidth(),this->t_ingame_helpconsole->getHeight(),
			0,0,
			this->t_ingame_helpconsole->getWidth(),this->t_ingame_helpconsole->getHeight(),
			this->gui_scale,this->gui_scale,
			0xFFFFFFFF);

		//difficulty
		if(g_engine->difficulty == 0)g_engine->font_manager.fonts[0]->PrintText("easy",x+this->gui_scale*175,y+this->gui_scale*5,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
		if(g_engine->difficulty == 1)g_engine->font_manager.fonts[0]->PrintText("normal",x+this->gui_scale*175,y+this->gui_scale*5,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
		if(g_engine->difficulty == 2)g_engine->font_manager.fonts[0]->PrintText("hard",x+this->gui_scale*175,y+this->gui_scale*5,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
		//if(g_engine->difficulty == 3)g_engine->font_manager.fonts[0]->PrintText("serious",x+this->gui_scale*175,y+this->gui_scale*5,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);

		//location
		g_engine->font_manager.fonts[0]->PrintText("Stroggos",x+this->gui_scale*24,y+this->gui_scale*17,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
		//primary objective
		g_engine->font_manager.fonts[0]->PrintText(g_engine->game_maps[g_engine->map_current]->objective_p,x+this->gui_scale*24,y+this->gui_scale*45,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
		//secondary objective
		g_engine->font_manager.fonts[0]->PrintText(g_engine->game_maps[g_engine->map_current]->objective_s,x+this->gui_scale*24,y+this->gui_scale*100,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
		//kills
		g_engine->font_manager.fonts[0]->PrintText("0/0",x+this->gui_scale*22,y+this->gui_scale*154,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
		//goals
		g_engine->font_manager.fonts[0]->PrintText("0/0",x+this->gui_scale*100,y+this->gui_scale*154,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
		//secrets
		g_engine->font_manager.fonts[0]->PrintText("0/0",x+this->gui_scale*180,y+this->gui_scale*154,1,g_engine->font_manager.fonts[0]->char_dist,0xFF00FF00);
	}

	void Editor::DrawInventory(int id)
	{
		int x = this->Player_managers[id]->screen->getX() + this->Player_managers[id]->screen->getWidth()/2 - this->gui_scale*this->t_inventory->getWidth()/2;
		int y = this->Player_managers[id]->screen->getY() + this->Player_managers[id]->screen->getHeight()/2 - this->gui_scale*this->t_inventory->getHeight()/2;

		g_engine->ar_graphics->draw_texture(
			this->t_inventory,
			x,y,
			this->t_inventory->getWidth(),this->t_inventory->getHeight(),
			0,0,
			this->t_inventory->getWidth(),this->t_inventory->getHeight(),
			this->gui_scale,this->gui_scale,
			0xFFFFFFFF);

		std::vector <string> text;
		text.push_back("item");
		text.push_back("----");

		for(unsigned int i=0;i<this->Player_managers[id]->inventory.size();i++) text.push_back(this->Player_managers[id]->inventory[i]);

		g_engine->font_manager.fonts[0]->PrintText(text,x+24*this->gui_scale,y+24*this->gui_scale,1,g_engine->font_manager.fonts[0]->char_dist,0xFFFFFFFF);		

	}

}//namespace