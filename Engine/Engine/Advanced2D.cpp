// Advanced2D Engine
// Main source code file

//includes
#include "Advanced2D.h"

namespace Advanced2D
{
	Engine::Engine()
	{
		this->display_sectors = false;
		this->display_lineofsight = false;
		this->display_rotationaxis = false;
		this->display_collisiondots = false;
		this->lock_framerate = 60;
		this->locked = true;
		this->display_fps = false;

		this->difficulty = 0;

		this->texture_manager = NULL;	

		this->noclip = false;

		time_past=timeGetTime();
		time_present=timeGetTime();
		time_delta=14;

		ar_direct3d = NULL;		
		pBackBuffer = NULL;

		srand((unsigned int)time(NULL));
		p_frameCount_core = 1;
		p_frameRate_core = 1;
		p_frameCount_real = 1;
		p_frameRate_real = 1;
		p_windowHandle = 0;
		p_pauseMode = false;
		p_UpdateTime=14;

		this->bullet_time = false;

		//save time
		time_t rawtime;
		time ( &rawtime );
		std::string temp = ctime (&rawtime);

		this->log_text.push_back("    (");
		this->log_text.push_back("      )");
		this->log_text.push_back("    ____");
		this->log_text.push_back(" ,|`----`|");
		this->log_text.push_back("((|      |");
		this->log_text.push_back(" \\|      |");
		this->log_text.push_back("  |      |");
		this->log_text.push_back("  '------'");

		this->log_text.push_back("");
		this->log_text.push_back("MUG engine started at: " + temp);
		this->log_text.push_back("");		

		//AR
		this->v_sync = false;
		this->game_speed = 1;
		this->engine_state = PRELOAD;
		this->render_type = TEXTURE;

		this->collisioncheck_counter=0;
		this->collisioncheck_counter_other=0;
		this->count_check_onscreen=0;
		this->count_check_insector=0;
		this->collision_counter=0;
		this->collidable_counter=0;
		this->lineline_counter=0;

		this->gui = NULL;
		this->ar_graphics = NULL;
		this->p_effect = NULL;
		this->coll_manager = NULL;

		this->game_level=0;
		this->game_state=0;
		this->offset_Y=0;
		this->offset_X=0;		
		
		//set default values
		this->setAppTitle("MUG engine v0.1");
		this->setScreenWidth(640);
		this->setScreenHeight(480);
		this->setColorDepth(32);
		this->setFullscreen(false);

		//window handle must be set later on for DirectX!
		this->setWindowHandle(0);

		this->ar_direct3d = new AR_Direct3D();
	}


	Engine::~Engine()
	{
		audio->StopAll();

		delete audio;
		delete p_input;
		delete math;

		if (this->p_effect) this->p_effect->Release();		
		if (this->p_device) this->p_device->Release();
		if (this->p_d3d) this->p_d3d->Release();

	}	

	int Engine::Init(int width, int height, int colordepth, bool fullscreen)
	{
		//initialize Direct3D
		if (this->ar_direct3d->create_d3d())
		{
			this->p_d3d=this->ar_direct3d->getD3D();
		}
		else
		{
			MessageBox(getWindowHandle(), "Error initializing Direct3D !", "Error", MB_OK);
			return 0;
		}
		
		//initialize Direct3D device
		if (this->ar_direct3d->create_device(getWindowHandle(),width,height,fullscreen,v_sync))
		{
			this->p_device=this->ar_direct3d->getDevice();

			this->ar_graphics = new AR_Graphics();
			if (this->ar_graphics==NULL) return false;	

			//set the global font
			if(this->font_manager_dx.LoadFont(this->p_device,"Courier New",18,0,400,false))
				e_font = this->font_manager_dx.fonts[0];//prijelazno rjesenje
			else
			{
				this->log_text.push_back("Failed creating DX font.");				
				return false;
			}
				

			//g_engine->e_font = new AR_DXFont();
			//g_engine->e_font->setFont(g_engine->p_device,"Arial",18,0,400,false);
			/*if (g_engine->getScreenHeight()<480)
				e_font->setFont(this->p_device,"Arial",12,0,400,false);
			else if (g_engine->getScreenHeight()>=900)
				e_font->setFont(this->p_device,"Arial",18,0,400,false);
			else*/

			this->log_text.push_back("Direct3D initialized.");
			this->log_text.push_back("Direct3D device initialized.");
			this->e_font->write_stringvector(this->p_device,this->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
			
		}
		else
		{
			MessageBox(getWindowHandle(), "Error initializing Direct3D device !", "Error", MB_OK);
			return 0;			
		}

		//my stuff (mozda bi ipak trebalo iz igre pozvat?)
		this->gui = new GUI();
		if (this->gui==NULL) return 0;
		if (!this->gui->LoadTextures()) return 0;
		g_engine->coll_manager = new eng_Collision();

		//initialize DirectX sprite handler		
		if (this->ar_direct3d->create_spritehandler())
		{
			this->p_sprite_handler=ar_direct3d->getSpriteHandler();
			this->log_text.push_back("Direct3D sprite handler initialized.");
			this->e_font->write_stringvector(this->p_device,this->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);			
		}
		else
		{
			MessageBox(getWindowHandle(), "Error initializing sprite handler !", "Error", MB_OK);
			return 0;
		}		

		//initialize directinput	
		p_input = new Input();
		if (!p_input->Init(this->getWindowHandle()))
		{
			MessageBox(getWindowHandle(), "Error initializing DirectInput !", "Error", MB_OK);
			return 0;
		}
		else
		{
			if (this->p_input->init_joystick())
			{
				g_engine->log_text.push_back("Joystick detected.");
				this->e_font->write_stringvector(this->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);			
			}
			else
			{
				g_engine->log_text.push_back("Joystick NOT detected.");
				this->e_font->write_stringvector(this->p_device,g_engine->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);			
			}			
		}		

		//create audio system
		audio = new AR_Audio();
		if (!audio->Init())
		{
			MessageBox(getWindowHandle(), "Error initializing FMOD audio system () !", "Error", MB_OK);
			return 0;
		}
		else
		{
			this->log_text.push_back("FMOD audio system initialized.");
			this->e_font->write_stringvector(this->p_device,this->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
		}

		//initialize math library
		math = new Math();

		this->log_text.push_back("Loading game assets...");
		this->e_font->write_stringvector(this->p_device,this->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
		
		//call game initialization extern function
		if (!game_init(this->getWindowHandle()))
		{
			this->log_text.push_back("... failed.");
			this->e_font->write_stringvector(this->p_device,this->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
			return 0;
		}
		else
		{
			this->log_text.push_back("... complete.");
			this->e_font->write_stringvector(this->p_device,this->log_text,0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),'L',0xFFFFFFFF);
		}

		this->p_device->GetRenderTarget(0,&pBackBuffer);			

		return 1;
	}

	void Engine::UpdateFin()
	{
		this->time_present = timeGetTime();
		this->time_delta = this->time_present-this->time_past;
		this->time_past = this->time_present;

		//calculate real framerate
		p_frameCount_real++; 
		if (p_realTimer.stopwatch(999))
		{
			p_frameRate_real = p_frameCount_real;
			p_frameCount_real = 0;
		}

		this->lineline_counter=0;
		this->collidable_counter=0;

		this->p_input->Update();//update input devices	
		if(this->gui) gui->Update();
		this->audio->burySound();
		this->audio->Update();//update audio

		//game
		game_update();
		game_render();

		//remove dead entities and buttons from the list	
		this->BurySprites();
		if (this->gui!=NULL) this->gui->BuryButtons();

		for (int n=0; n<255; n++)
		{
			this->p_input->button_released[n] = false;
		}
	}

	void Engine::Update()
	{
		//calculate core framerate
		p_frameCount_core++;
		if (p_coreTimer.stopwatch(999))
		{
			p_frameRate_core = p_frameCount_core;
			p_frameCount_core = 0;
		}

		if(this->v_sync) this->UpdateFin();
		else
		{
			if(this->lock_framerate!=0)
			{
				if(timedUpdate.stopwatch(this->p_UpdateTime)) this->UpdateFin();//timed loop
			}
			else this->UpdateFin();//no timer
		}		
	}

	void Engine::addSpriteLayer(int amount)
	{
		for(int i=0;i<amount;i++)
		{
			SpriteLayer *layer=new SpriteLayer();
			game_sprites.push_back(layer);
		}
	}

	void Engine::addSprite(Sprite *sprite, int sprite_layer)
	{
		if(this->save_to_map) 
		{
			this->addSprite(sprite, this->map_current, sprite_layer);
			return;
		}

		if (sprite_layer>game_sprites.size() || game_sprites.empty())
		{
			MessageBox(NULL, "Adding entity to a non existing list?! No you won't :P", NULL, MB_OK);
			g_engine->Close();
			return;
		}

		if(sprite->rotate)
			sprite->vector_rect.Set(sprite->w(),sprite->h(),sprite->getRotateX_real(),sprite->getRotateY_real());
		else
			sprite->vector_rect.Set(sprite->w(),sprite->h(),sprite->hw(),sprite->hh());

		sprite->setLayer(sprite_layer);
		game_sprites[sprite_layer]->sprites.push_back(sprite);
	}

	void Engine::addSprite(Sprite *sprite, int map, int sprite_layer)
	{
		if (map > game_maps.size() || game_maps.empty())
		{
			MessageBox(NULL, "Adding entity to a non existing map?!", NULL, MB_OK);
			g_engine->Close();
			return;
		}

		if (sprite_layer > game_maps[map]->sprite_layers.size() || game_maps[map]->sprite_layers.empty())
		{
			MessageBox(NULL, "Adding entity to a non existing list?!", NULL, MB_OK);
			g_engine->Close();
			return;
		}		

		if(sprite->rotate)
			sprite->vector_rect.Set(sprite->w(),sprite->h(),sprite->getRotateX_real(),sprite->getRotateY_real());
		else
			sprite->vector_rect.Set(sprite->w(),sprite->h(),sprite->hw(),sprite->hh());

		sprite->setLayer(sprite_layer);
		this->game_maps[map]->sprite_layers[sprite_layer]->sprites.push_back(sprite);
		sprite->map_current = map;

	}

	void Engine::BurySprites()
	{
		for(unsigned int i=0;i<game_sprites.size();i++)
			game_sprites[i]->BurySprites();

		for(unsigned int i=0;i<this->game_maps.size();i++)
		{
			for(unsigned int j=0;j<this->game_maps[i]->sprite_layers.size();j++)
				this->game_maps[i]->sprite_layers[j]->BurySprites();
		}

	}

	int Engine::getSpriteCount()
	{
		int counter=0;
		for (int i=0;i<game_sprites.size();i++) counter+=game_sprites[i]->getSize();
		return counter;
	}

	void Engine::delete_GameSprites()
	{
		Sprite *a ;
		std::list<Sprite*>::iterator iter;

		for (unsigned int i=0;i<g_engine->game_sprites.size();i++)
		{
			iter = g_engine->game_sprites[i]->sprites.begin();
			while (iter != g_engine->game_sprites[i]->sprites.end())
			{
				a = *iter;
				a->setAlive(false);
				iter++;
			}
		}

		for (unsigned int i=0;i<g_engine->game_maps.size();i++)
		{
			g_engine->game_maps[i]->DeleteSprites();
			g_engine->game_maps[i]->Reset();
		}
	}

	void Engine::Shutdown()
	{
		gameover = true;
	}

	void Engine::Close()
	{
		try {
			gameover = true;
		}
		catch (...) { }
	}

	void Engine::save_log()
	{
		std::ofstream fileout("log.txt");	

		for(int i=0;i<this->log_text.size();i++)
			fileout << log_text[i] << std::endl;

		fileout.close();
	}

	void Engine::message(std::string message, std::string title)
	{
		MessageBox(this->getWindowHandle(), message.c_str(), title.c_str(), MB_OK);
		//MessageBox(0, message.c_str(), title.c_str(), 0);
	}

	void Engine::fatalerror(std::string message, std::string title)
	{
		this->message(message,title);
		Shutdown();
	}		

	double Engine::stringtoint(std::string text)
	{
		double i = 0;
		std::stringstream stream(text);

		if(stream >> i)
			return i;
		else
			return 0;//ili da vratim 1 ?
	}

	std::string Engine::inttostring(double i)
	{
		std::stringstream stream;
		stream << i;

		return stream.str();		
	}

} //namespace