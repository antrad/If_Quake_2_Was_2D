#include "Options.h"

namespace Advanced2D
{
	Options::Options()
	{
		//screen
		this->s_fullscreen = false;
		this->s_vsync = false;
		this->s_screenw = 800;
		this->s_screenh = 600;
		this->s_volumemusic = 100;
		this->s_volumesound = 100;	
		this->s_splitscreen = true;	
		this->s_scalehud = 1;
		this->s_lockframerate = g_engine->lock_framerate;
		this->s_gamelocked = true;

		this->t_save = NULL;
		this->t_back = NULL;

		this->setNULL();

		this->options_visible = true;

		//player controls(options)
		PlayerControls *player_1 = new PlayerControls();
		this->player_controls.push_back(player_1);
		player_1->id = 0;

		PlayerControls *player_2 = new PlayerControls();
		this->player_controls.push_back(player_2);
		player_2->id = 1;

	}

	void Options::setNULL()
	{
		//screen
		this->fullscreen = NULL;
		this->v_sync = NULL;
		this->scr_resolution = NULL;
		this->splitscreen = NULL;
		this->b_scalehud = NULL;
		this->b_controls = NULL;

		//misc
		this->volume_music = NULL;
		this->volume_sound = NULL;
	}

	bool Options::LoadTextures()
	{
		this->t_save = g_engine->texture_manager->findTexture("Data/Textures/menu/m_save.png");
		if (this->t_save == NULL){g_engine->message("\"Data/Textures/menu/m_save.png\" not preloaded.");return false;}
		
		this->t_back = g_engine->texture_manager->findTexture("Data/Textures/menu/m_back.png");
		if (this->t_back == NULL){g_engine->message("\"Data/Textures/menu/m_back.png\" not preloaded.");return false;}

		return true;
	}

	void Options::setOptions()
	{
		if (!this->loadSettings()) return;

		g_engine->gui->delete_GuiButtons();
		g_engine->setGameState(OPTIONS);

		for(unsigned int i=0;i<this->player_controls.size();i++)
		{
			this->player_controls[i]->s_scalehud = this->s_scalehud;
		}

		this->options_visible = true;

		int posx = g_engine->getScreenWidth()/2 - 150*this->s_scalehud;
		int posy = g_engine->getScreenHeight()/2 - 100*this->s_scalehud;
		int row_height = g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud + g_engine->font_manager.fonts[0]->row_dist*this->s_scalehud;

		Button *options_text3 = new Button();
		options_text3->setGUI(OPTIONS_CLASS);
		options_text3->setColor_Texture(0x00000000);
		options_text3->setImage(g_engine->gui->default_t);		
		options_text3->setPosition(posx,posy);
		options_text3->setSize(150*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud);
		options_text3->font_png = true;
		options_text3->setColor_Text(0xFF00FF00);
		options_text3->text_positioning = 'R';	
		options_text3->page.push_back("scale hud");
		options_text3->page.push_back("sound volume");		
		options_text3->page.push_back("music volume");
		options_text3->clickable = false;
		options_text3->row_height = g_engine->e_font->size+2;
		g_engine->gui->addButton(options_text3);

		//SCALE HUD
		std::vector <string> scalehud;	
		scalehud.push_back("1");
		scalehud.push_back("1.5");
		scalehud.push_back("2");
		scalehud.push_back("2.5");
		scalehud.push_back("3");
		this->b_scalehud = g_engine->gui->add_dropbox(
			O_SCALEHUD,
			posx+166*this->s_scalehud,posy,
			100*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud,
			scalehud);	
		this->b_scalehud->setText(g_engine->inttostring(this->s_scalehud));	
		this->b_scalehud->font_png = true;
		for(unsigned int i=0;i<this->b_scalehud->form.size();i++)
		{
			this->b_scalehud->form[i]->font_png = true;
		}

		posy += row_height;

		//VOLUME SOUND
		this->volume_sound = g_engine->gui->add_slider_h(
			posx+166*this->s_scalehud,posy,
			200*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud,this->s_volumesound,100);
		this->volume_sound->setType(O_VOLUMESOUND);
		this->volume_sound->font_png = true;

		posy += row_height;

		//VOLUME MUSIC
		this->volume_music = g_engine->gui->add_slider_h(
			posx+166*this->s_scalehud,posy,
			200*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud,this->s_volumemusic,100);
		this->volume_music->setType(O_VOLUMEMUSIC);
		this->volume_music->font_png = true;

		posy += 2*row_height;	

		//CUSTOMIZE CONTROLS
		this->b_controls = new Button();
		this->b_controls->setGUI(OPTIONS_CLASS);
		this->b_controls->setType(O_CONTROLS);
		this->b_controls->setImage(g_engine->gui->default_t);
		this->b_controls->text_positioning = 'L';
		this->b_controls->setText("customize controls");
		this->b_controls->setColor_Text(0xFFFFFFFF);
		this->b_controls->setColor_Texture(0x00000000);
		this->b_controls->setPosition(posx+166*this->s_scalehud,posy);
		this->b_controls->setSize(200*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud);
		this->b_controls->font_png = true;		
		this->b_controls->row_height = g_engine->e_font->size + 2;
		g_engine->gui->addButton(this->b_controls);	

		posy = g_engine->getScreenHeight()/2 + 100;		

		//ADD OPTIONS TEXT
		Button *options_text = new Button();
		options_text->setGUI(OPTIONS_CLASS);
		options_text->setImage(g_engine->gui->default_t);		
		options_text->setPosition(posx,posy);
		options_text->setSize(150*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud);
		options_text->font_png = true;
		options_text->setColor_Text(0xFF00FF00);
		options_text->setColor_Texture(0x00000000);
		options_text->text_positioning = 'R';
		options_text->page.push_back("resolution");
		options_text->page.push_back("fullscreen");
		options_text->page.push_back("vsync");
		options_text->page.push_back("splitscreen");
		options_text->page.push_back("lock framerate");
		options_text->clickable = false;
		options_text->row_height = g_engine->e_font->size+2;
		g_engine->gui->addButton(options_text);		

		//SCREEN RESOLUTION
		std::vector <string> screen_resolution;	
		screen_resolution.push_back("1024x768");
		screen_resolution.push_back("1152x864");
		screen_resolution.push_back("1280x720");
		screen_resolution.push_back("1280x768");
		screen_resolution.push_back("1280x960");
		screen_resolution.push_back("1280x1024");
		screen_resolution.push_back("1366x768");
		screen_resolution.push_back("1440x900");
		screen_resolution.push_back("1600x1200");
		screen_resolution.push_back("1920x1080");
		screen_resolution.push_back("1920x1200");		
		this->scr_resolution = g_engine->gui->add_dropbox(
			O_RESOLUTION,
			posx+166*this->s_scalehud,posy,
			100*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud,
			screen_resolution);	
		this->scr_resolution->setText(g_engine->inttostring(this->s_screenw)+"x"+g_engine->inttostring(this->s_screenh));	
		this->scr_resolution->font_png = true;
		for(unsigned int i=0;i<this->scr_resolution->form.size();i++)
		{
			this->scr_resolution->form[i]->font_png = true;
		}

		posy += row_height;

		//FULLSCREEN
		this->fullscreen = g_engine->gui->add_checkbox(
			O_FULLSCREEN,
			posx+166*this->s_scalehud,posy,
			32*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud,"yes","no",NULL,this->s_fullscreen);
		this->fullscreen->font_png = true;
		this->fullscreen->setColor_Texture(0x00000000);
		this->fullscreen->setColor_Text(0xFFFFFFFF);

		posy += row_height;		

		//VSYNC
		this->v_sync = g_engine->gui->add_checkbox(
			O_VSYNC,
			posx+166*this->s_scalehud,posy,
			32*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud,"yes","no",NULL,this->s_vsync);
		this->v_sync->font_png = true;
		this->v_sync->setColor_Texture(0x00000000);
		this->v_sync->setColor_Text(0xFFFFFFFF);

		posy += row_height;

		//SPLITSCREEN
		this->splitscreen = g_engine->gui->add_checkbox(
			O_SPLITSCREEN,
			posx+166*this->s_scalehud,posy,
			32*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud,"hor","ver",NULL,this->s_splitscreen);
		this->splitscreen->font_png = true;	
		this->splitscreen->setColor_Texture(0x00000000);
		this->splitscreen->setColor_Text(0xFFFFFFFF);

		posy += row_height;

		//LOCK FRAMERATE
		this->b_lockframerate = new Button();
		this->b_lockframerate->setType(O_LOCKFRAMERATE);
		this->b_lockframerate->setClass(TEXTBOX);
		this->b_lockframerate->setImage(g_engine->gui->default_t);
		this->b_lockframerate->setSize(64*this->s_scalehud,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud);
		this->b_lockframerate->setPosition(posx+166*this->s_scalehud,posy);
		g_engine->gui->addButton(this->b_lockframerate);
		this->b_lockframerate->font_png = true;	
		this->b_lockframerate->setColor_Texture(0x00000000);
		this->b_lockframerate->setColor_Text(0xFFFFFFFF);
		std::stringstream stream;
		stream << g_engine->lock_framerate;
		this->b_lockframerate->setText(stream.str());

		posy += row_height;	

		//EXIT STUFF
		Button *options_exit = new Button();
		options_exit->setType(OPTIONS_EXIT);
		options_exit->setImage(this->t_back);
		options_exit->setColor_Texture(0xFFCCCCCC);
		options_exit->setColor_Text(0xFF000000);
		options_exit->setPosition(
			g_engine->getScreenWidth()/2 + options_exit->getWidth()/2,
			g_engine->getScreenHeight() - 2*options_exit->getHeight());
		g_engine->gui->addButton(options_exit);
		options_exit->font_png = true;

		Button *options_save = new Button();
		options_save->setGUI(OPTIONS_CLASS);
		options_save->setType(OPTIONS_SAVE);
		options_save->setImage(this->t_save);
		options_save->setColor_Texture(0xFFCCCCCC);
		options_save->setColor_Text(0xFF000000);
		options_save->setPosition(
			g_engine->getScreenWidth()/2 - options_save->getWidth() - options_save->getWidth()/2,
			options_exit->getY());
		g_engine->gui->addButton(options_save);
		options_save->font_png = true;

		this->buttons.clear();

		this->buttons.push_back(b_lockframerate);
		this->buttons.push_back(fullscreen);
		this->buttons.push_back(v_sync);
		this->buttons.push_back(scr_resolution);
		this->buttons.push_back(splitscreen);
		this->buttons.push_back(b_scalehud);
		this->buttons.push_back(volume_music);
		this->buttons.push_back(volume_sound);
		this->buttons.push_back(b_controls);
		this->buttons.push_back(options_save);
		this->buttons.push_back(options_exit);
		this->buttons.push_back(options_text);
		this->buttons.push_back(options_text3);

		for(unsigned int i=0;i<this->buttons.size();i++)
		{
			for(unsigned int j=0;j<this->buttons[i]->form.size();j++)
			{
				this->buttons.push_back(this->buttons[i]->form[j]);
			}
		}

		this->setOptions_Controls();

	}

	void Options::setOptions_Controls()
	{
		this->buttons_controls.clear();

		int posx = g_engine->getScreenWidth()/2 - 150*this->s_scalehud;
		int posy = g_engine->getScreenHeight()/2 - 150*this->s_scalehud;
		int row_height = g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud + g_engine->font_manager.fonts[0]->row_dist*this->s_scalehud;

		//player controls
		this->player_controls[0]->SetButtons(posx - 150*this->s_scalehud, posy);
		this->player_controls[1]->SetButtons(posx + 150*this->s_scalehud, posy);

		posy += 275*this->s_scalehud;

		Button *options_text2 = new Button();
		options_text2->setGUI(OPTIONS_CLASS);
		options_text2->setImage(g_engine->gui->default_t);		
		options_text2->setPosition(posx,posy);
		options_text2->setSize(150,g_engine->e_font->size);
		options_text2->font_png = true;
		options_text2->text_positioning = 'R';
		options_text2->setColor_Texture(0x00000000);		
		options_text2->setColor_Text(0xFF00FF00);
		options_text2->page.push_back(".other controls.");
		options_text2->page.push_back("");
		options_text2->page.push_back("console");
		options_text2->page.push_back("cheat console");	
		options_text2->page.push_back("pause");
		options_text2->page.push_back("toggle splitscreen");
		options_text2->page.push_back("display framerate");
		options_text2->clickable = false;
		options_text2->row_height = g_engine->e_font->size+2;
		g_engine->gui->addButton(options_text2);

		posy += 2*row_height;

		Button *options_text4 = new Button();
		options_text4->setGUI(OPTIONS_CLASS);
		options_text4->setImage(g_engine->gui->default_t);		
		options_text4->setPosition(posx+166*this->s_scalehud,posy);
		options_text4->setSize(150,g_engine->e_font->size);
		options_text4->font_png = true;
		options_text4->text_positioning = 'L';
		options_text4->setColor_Texture(0x00000000);		
		options_text4->setColor_Text(0xFFFFFFFF);		
		options_text4->page.push_back("\"~\"");
		options_text4->page.push_back("\"F12\"");	
		options_text4->page.push_back("\"p\"");
		options_text4->page.push_back("\"F9\"");
		options_text4->page.push_back("\"F4\"");
		options_text4->clickable = false;
		options_text4->row_height = g_engine->e_font->size+2;
		g_engine->gui->addButton(options_text4);

		//EXIT STUFF
		Button *options_exit = new Button();
		options_exit->setGUI(OPTIONS_CLASS);
		options_exit->setType(OPTIONS_EXITCONTROLS);
		options_exit->setImage(this->t_back);
		options_exit->setColor_Texture(0xFFCCCCCC);
		options_exit->setColor_Text(0xFF000000);
		options_exit->setPosition(
			g_engine->getScreenWidth()/2 + options_exit->getWidth()/2,
			g_engine->getScreenHeight() - 2*options_exit->getHeight());
		g_engine->gui->addButton(options_exit);
		options_exit->font_png = true;

		Button *options_save = new Button();
		options_save->setGUI(OPTIONS_CLASS);
		options_save->setType(OPTIONS_SAVE);
		options_save->setImage(this->t_save);
		options_save->setColor_Texture(0xFFCCCCCC);
		options_save->setColor_Text(0xFF000000);
		options_save->setPosition(
			g_engine->getScreenWidth()/2 - options_save->getWidth() - options_save->getWidth()/2,
			options_exit->getY());
		g_engine->gui->addButton(options_save);
		options_save->font_png = true;

		//hide unhide when "customize controls"
		this->buttons_controls.push_back(options_text2);
		this->buttons_controls.push_back(options_text4);
		this->buttons_controls.push_back(options_save);
		this->buttons_controls.push_back(options_exit);
		options_text2->setVisible(false);
		options_text4->setVisible(false);
		options_save->setVisible(false);
		options_exit->setVisible(false);

			
		for(unsigned int i=0;i<buttons_controls.size();i++)
		{
			for(unsigned int j=0;j<buttons_controls[i]->form.size();j++)
			{
				this->buttons.push_back(buttons_controls[i]->form[j]);
				buttons_controls[i]->form[j]->setVisible(false);
			}
		}

	}

	bool Options::loadSettings()
	{
		std::ifstream file("System/config.txt");

		//if no file, create it
		if(!file.is_open()) return false;				

		//read file
		std::string line;
		while(std::getline(file,line))
		{
			std::stringstream stream(line);
			std::string control;
			double value;

			if(stream >> control >> value)
			{
				if (control == "lock_this_game")
				{
					this->s_gamelocked = value;
				}
				else if (control == "fullscreen")
				{
					this->s_fullscreen = value;
				}
				else if (control == "v_sync")
				{
					this->s_vsync = value;
				}
				else if (control == "scr_width")
				{
					this->s_screenw = (int)value;
				}
				else if (control == "scr_height")
				{
					this->s_screenh = (int)value;
				}
				else if (control == "splitscreen")
				{
					this->s_splitscreen = value;
				}
				else if (control == "volume_sound")
				{
					this->s_volumesound = (int)value;
				}
				else if (control == "volume_music")
				{
					this->s_volumemusic = (int)value;
				}
				else if (control == "scale_hud")
				{
					this->s_scalehud = value;
				}
				else if (control == "lock_framerate")
				{
					this->s_lockframerate = value;
				}
			}			
		}

		file.close();

		return true;

	}//loadSettings()

	void Options::options_savetofile()
	{
		std::ofstream fileout("System/config.txt");

		fileout << "//SETTINGS" << std::endl;

		//screen resolution
		std::string res = this->scr_resolution->getText();
		size_t found;
		found=res.find("x");
		if (found!=string::npos)
		{
			std::string w;
			std::string h;
			for(int i=0;i<found;i++)
			{
				w.push_back(res[i]);
			}
			for(int i=found+1;i<res.size();i++)
			{
				h.push_back(res[i]);
			}			
			fileout << "scr_width " << w << std::endl;
			fileout << "scr_height " << h << std::endl;
		}

		if(!g_engine->locked) fileout << "lock_this_game " << this->s_gamelocked << std::endl;
		fileout << "fullscreen " << this->fullscreen->getState() << std::endl;
		fileout << "splitscreen " << this->splitscreen->getState() << std::endl;
		fileout << "v_sync " << this->v_sync->getState() << std::endl;
		fileout << "volume_sound " << this->s_volumesound << std::endl;
		fileout << "volume_music " << this->s_volumemusic << std::endl;
		fileout << "scale_hud " << this->s_scalehud << std::endl;
		fileout << "lock_framerate " << this->b_lockframerate->getText() << std::endl;

		//save in game
		this->s_splitscreen = this->splitscreen->getState();
		this->s_fullscreen = this->fullscreen->getState();
		this->s_vsync = this->v_sync->getState();
		this->s_scalehud = g_engine->stringtoint(this->b_scalehud->getText());
		int x;
		std::stringstream stream(this->b_lockframerate->getText());
		if(stream >> x) this->s_lockframerate = x;
		g_engine->lock_framerate = this->s_lockframerate;
		g_engine->p_UpdateTime = 999/this->s_lockframerate;

		//save player controls to file
		for(unsigned int i=0;i<this->player_controls.size();i++)
		{
			fileout << std::endl;
			fileout << "//PLAYER_" + g_engine->inttostring(i+1) + " CONTROLS" << std::endl;
			fileout << "left_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->left->getText() << std::endl;
			fileout << "right_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->right->getText() << std::endl;
			fileout << "up_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->up->getText() << std::endl;
			fileout << "down_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->down->getText() << std::endl;
			fileout << "jump_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->jump->getText() << std::endl;
			fileout << "fire1_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->fire1->getText() << std::endl;
			fileout << "fire2_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->fire2->getText() << std::endl;
			fileout << "action_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->action->getText() << std::endl;
			fileout << "pda_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->pda->getText() << std::endl;
			fileout << "fly_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->b_fly->getText() << std::endl;
			fileout << "bullettime_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->b_bullettime->getText() << std::endl;
			fileout << "nextweapon_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->next_weapon->getText() << std::endl;
			fileout << "prevweapon_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->prev_weapon->getText() << std::endl;
			fileout << "weapon1_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon0->getText() << std::endl;
			fileout << "weapon2_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon1->getText() << std::endl;
			fileout << "weapon3_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon2->getText() << std::endl;
			fileout << "weapon4_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon3->getText() << std::endl;
			fileout << "weapon5_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon4->getText() << std::endl;
			fileout << "weapon6_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon5->getText() << std::endl;
			fileout << "weapon7_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon6->getText() << std::endl;
			fileout << "weapon8_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon7->getText() << std::endl;
			fileout << "weapon9_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon8->getText() << std::endl;
			fileout << "weapon0_" + g_engine->inttostring(i+1) + " " << this->player_controls[i]->weapon9->getText() << std::endl;
		}
		
		fileout.close();

		//save in game
		for(unsigned int i=0;i<this->player_controls.size();i++) this->player_controls[i]->UpdateControls();

	}//options_savetofile()

	void Options::options_createdefaultfile()
	{
		return;

		/*std::ofstream fileout("System/config.txt");

		fileout << "//SETTINGS" << std::endl;		
		fileout << "scr_width " << 800 << std::endl;
		fileout << "scr_height " << 600 << std::endl;
		fileout << "fullscreen " << 0 << std::endl;
		fileout << "v_sync " << 0 << std::endl;
		fileout << "volume_sound " << 100 << std::endl;
		fileout << "volume_music " << 100 << std::endl;

		//save player controls
		fileout << std::endl;
		fileout << "//PLAYER CONTROLS" << std::endl;
		fileout << "left " << g_engine->gui->key_to_string(this->LEFT) << std::endl;
		fileout << "right " << g_engine->gui->key_to_string(this->RIGHT) << std::endl;
		fileout << "up " << g_engine->gui->key_to_string(this->UP) << std::endl;
		fileout << "down " << g_engine->gui->key_to_string(this->DOWN) << std::endl;
		fileout << "jump " << g_engine->gui->key_to_string(this->JUMP) << std::endl;
		fileout << "fire1 " << g_engine->gui->key_to_string(this->FIRE1) << std::endl;
		fileout << "fire2 " << g_engine->gui->key_to_string(this->FIRE2) << std::endl;
		fileout << "next_weapon " << g_engine->gui->key_to_string(this->NEXT_WEAPON) << std::endl;
		fileout << "prev_weapon " << g_engine->gui->key_to_string(this->PREV_WEAPON) << std::endl;

		fileout.close();*/

	}	

	void Options::ButtonRelease(Button *button,int mousebutton)
	{
		switch(mousebutton)
		{
		case 0:
			switch (button->getType())
			{
			case O_SCALEHUD:
				this->s_scalehud = g_engine->stringtoint(button->getText());
				break;

			case O_CONTROLS:
			case OPTIONS_EXITCONTROLS:
				this->options_visible = !this->options_visible;

				for(unsigned int i=0;i<this->buttons.size();i++)
				{
					this->buttons[i]->setVisible(!this->buttons[i]->getVisible());
				}
				for(unsigned int i=0;i<this->buttons_controls.size();i++)
				{
					this->buttons_controls[i]->setVisible(!this->buttons_controls[i]->getVisible());
				}
				for(unsigned int i=0;i<this->player_controls.size();i++)
				{
					for(unsigned int j=0;j<this->player_controls[i]->buttons.size();j++)
					{
						this->player_controls[i]->buttons[j]->setVisible(!this->player_controls[i]->buttons[j]->getVisible());
					}
				}				
				break;			
			}
		}
	}

}//Advanced2D