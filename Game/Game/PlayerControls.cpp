#include "PlayerControls.h"

namespace Advanced2D
{
	PlayerControls::PlayerControls()
	{
		this->s_scalehud = 1;
		//controls
		this->left = NULL;
		this->right = NULL;
		this->up = NULL;
		this->down = NULL;
		this->jump = NULL;
		this->fire1 = NULL;
		this->fire2 = NULL;
		this->action = NULL;
		this->pda = NULL;
		this->b_fly = NULL;
		this->b_bullettime = NULL;
		//weapon selection
		this->prev_weapon = NULL;
		this->next_weapon = NULL;
		this->weapon0 = NULL;
		this->weapon1 = NULL;
		this->weapon2 = NULL;
		this->weapon3 = NULL;
		this->weapon4 = NULL;
		this->weapon5 = NULL;
		this->weapon6 = NULL;
		this->weapon7 = NULL;
		this->weapon8 = NULL;
		this->weapon9 = NULL;

		//controls
		this->LEFT = DIK_A;
		this->RIGHT = DIK_D;
		this->UP = DIK_W;
		this->DOWN = DIK_S;
		this->JUMP = DIK_SPACE;
		this->FIRE1 = 0;
		this->FIRE2 = -1;
		this->ACTION = DIK_E;
		this->PDA = DIK_F1;
		this->C_FLY = DIK_LCONTROL;
		this->BULLETTIME = DIK_LSHIFT;
		this->PREV_WEAPON = -1200;
		this->NEXT_WEAPON = 1200;
		this->WEAPON0 = DIK_1;
		this->WEAPON1 = DIK_2;
		this->WEAPON2 = DIK_3;
		this->WEAPON3 = DIK_4;
		this->WEAPON4 = DIK_5;
		this->WEAPON5 = DIK_6;
		this->WEAPON6 = DIK_7;
		this->WEAPON7 = DIK_8;
		this->WEAPON8 = DIK_9;
		this->WEAPON9 = DIK_0;
	}

	void PlayerControls::SetButtons(int x, int y)
	{
		this->buttons.clear();

		int posx = x;
		int posy = y;
		int row_height = g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud + g_engine->font_manager.fonts[0]->row_dist*this->s_scalehud;

		//ADD OPTIONS TEXT
		Button *options_text = new Button();
		options_text->setGUI(CONTROLS_CLASS);
		options_text->setImage(g_engine->gui->default_t);
		options_text->setColor_Texture(0x00000000);
		options_text->setColor_Text(0xFFFFFFFF);
		options_text->setPosition(posx,posy);
		options_text->setSize(150,g_engine->e_font->size);
		options_text->font_png = true;
		options_text->text_positioning = 'R';
		options_text->setColor_Text(0xFF00FF00);
		options_text->page.push_back(".player" + g_engine->inttostring(this->id+1) + " controls.");
		options_text->page.push_back("");
		options_text->page.push_back("left");
		options_text->page.push_back("right");
		options_text->page.push_back("up");
		options_text->page.push_back("down");
		options_text->page.push_back("jump");
		options_text->page.push_back("fire1");
		options_text->page.push_back("fire2");	
		options_text->page.push_back("action");
		options_text->page.push_back("inventory");
		options_text->page.push_back("fly");
		options_text->page.push_back("bullet time");
		options_text->page.push_back("weapon +");
		options_text->page.push_back("weapon -");
		for(int i=0;i<10;i++) options_text->page.push_back("weapon " + g_engine->inttostring(i));

		options_text->clickable = false;
		options_text->row_height = g_engine->e_font->size+2;
		g_engine->gui->addButton(options_text);

		posx += 166;
		posy += 2*row_height;

		//ADD PLAYER CONTROLS BUTTONS
		for(int i=0;i<23;i++)
		{
			Button *control = new Button();
			control->setGUI(CONTROLS_CLASS);
			control->setClass(TEXTBOX);
			control->setID(this->id);
			control->setImage(g_engine->gui->default_t);
			control->font_png = true;
			control->text_positioning = 'L';
			control->setColor_Text(0xFFFFFFFF);
			control->setColor_Texture(0x00FFFFFF);
			control->setPosition(posx,posy);
			control->setSize(150,g_engine->font_manager.fonts[0]->getHeight()*this->s_scalehud);
			g_engine->gui->addButton(control);

			if(i==0)
			{
				control->setText(g_engine->gui->key_to_string(this->LEFT));
				control->setType(O_LEFT);
				this->left=control;
			}
			else if(i==1)
			{
				control->setText(g_engine->gui->key_to_string(this->RIGHT));
				control->setType(O_RIGHT);
				this->right=control;
			}		
			else if(i==2)
			{
				control->setText(g_engine->gui->key_to_string(this->UP));
				control->setType(O_UP);
				this->up=control;
			}
			else if(i==3)
			{
				control->setText(g_engine->gui->key_to_string(this->DOWN));
				control->setType(O_DOWN);
				this->down=control;
			}		
			else if(i==4)
			{
				control->setText(g_engine->gui->key_to_string(this->JUMP));
				control->setType(O_JUMP);
				this->jump=control;
			}
			else if(i==5)
			{
				control->setText(g_engine->gui->key_to_string(this->FIRE1));
				control->setType(O_FIRE1);
				this->fire1=control;
			}		
			else if(i==6)
			{
				control->setText(g_engine->gui->key_to_string(this->FIRE2));
				control->setType(O_FIRE2);
				this->fire2=control;
			}
			else if(i==7)
			{
				control->setText(g_engine->gui->key_to_string(this->ACTION));
				control->setType(O_ACTION);
				this->action=control;
			}		
			else if(i==8)
			{
				control->setText(g_engine->gui->key_to_string(this->PDA));
				control->setType(O_PDA);
				this->pda = control;
			}
			else if(i==9)
			{
				control->setText(g_engine->gui->key_to_string(this->C_FLY));
				control->setType(O_FLY);
				this->b_fly = control;
			}
			else if(i==10)
			{
				control->setText(g_engine->gui->key_to_string(this->BULLETTIME));
				control->setType(O_BULLETTIME);
				this->b_bullettime = control;
			}
			else if(i==11)
			{
				control->setText(g_engine->gui->key_to_string(this->NEXT_WEAPON));
				control->setType(O_NEXTWEAPON);
				this->next_weapon=control;
			}		
			else if(i==12)
			{
				control->setText(g_engine->gui->key_to_string(this->PREV_WEAPON));
				control->setType(O_PREVWEAPON);
				this->prev_weapon=control;
			}
			else if(i==13)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON0));
				control->setType(O_WEAPON0);
				this->weapon0=control;
			}
			else if(i==14)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON1));
				control->setType(O_WEAPON1);
				this->weapon1=control;
			}
			else if(i==15)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON2));
				control->setType(O_WEAPON2);
				this->weapon2=control;
			}
			else if(i==16)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON3));
				control->setType(O_WEAPON3);
				this->weapon3=control;
			}
			else if(i==17)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON4));
				control->setType(O_WEAPON4);
				this->weapon4=control;
			}
			else if(i==18)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON5));
				control->setType(O_WEAPON5);
				this->weapon5=control;
			}
			else if(i==19)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON6));
				control->setType(O_WEAPON6);
				this->weapon6=control;
			}
			else if(i==20)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON7));
				control->setType(O_WEAPON7);
				this->weapon7=control;
			}
			else if(i==21)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON8));
				control->setType(O_WEAPON8);
				this->weapon8=control;
			}
			else if(i==22)
			{
				control->setText(g_engine->gui->key_to_string(this->WEAPON9));
				control->setType(O_WEAPON9);
				this->weapon9=control;
			}	

			posy += row_height;

			this->buttons.push_back(control);
			control->setVisible(false);
			for(unsigned int j=0;j<control->form.size();j++)
			{
				this->buttons.push_back(control->form[j]);
				control->form[j]->setVisible(false);
			}
		}

		this->buttons.push_back(options_text);
		options_text->setVisible(false);

	}

	bool PlayerControls::Load(std::string filename)
	{
		std::ifstream file(filename.c_str());

		//if no file, create it (later)
		if(!file.is_open()) return false;

		//read file
		std::string line;
		while(std::getline(file,line))
		{
			std::stringstream stream(line);
			std::string control;
			std::string key;

			if(stream >> control >> key)
			{
				if(control == "left_" + g_engine->inttostring(this->id+1))
				{ 
					this->LEFT = g_engine->gui->string_to_key(key);
				}
				else if(control == "right_" + g_engine->inttostring(this->id+1))
				{ 
					this->RIGHT = g_engine->gui->string_to_key(key);
				}	
				else if(control == "up_" + g_engine->inttostring(this->id+1))
				{ 
					this->UP = g_engine->gui->string_to_key(key);
				}
				else if(control == "down_" + g_engine->inttostring(this->id+1))
				{ 
					this->DOWN = g_engine->gui->string_to_key(key);
				}
				else if(control == "jump_" + g_engine->inttostring(this->id+1))
				{ 
					this->JUMP = g_engine->gui->string_to_key(key);
				}	
				else if(control == "fire1_" + g_engine->inttostring(this->id+1))
				{ 
					this->FIRE1 = g_engine->gui->string_to_key(key);
				}
				else if(control == "fire2_" + g_engine->inttostring(this->id+1))
				{ 
					this->FIRE2 = g_engine->gui->string_to_key(key);
				}
				else if(control == "action_" + g_engine->inttostring(this->id+1))
				{ 
					this->ACTION = g_engine->gui->string_to_key(key);
				}
				else if(control == "pda_" + g_engine->inttostring(this->id+1))
				{ 
					this->PDA = g_engine->gui->string_to_key(key);
				}
				else if(control == "fly_" + g_engine->inttostring(this->id+1))
				{ 
					this->C_FLY = g_engine->gui->string_to_key(key);
				}
				else if(control == "bullettime_" + g_engine->inttostring(this->id+1))
				{ 
					this->BULLETTIME = g_engine->gui->string_to_key(key);
				}
				else if(control == "nextweapon_" + g_engine->inttostring(this->id+1))
				{ 
					this->NEXT_WEAPON = g_engine->gui->string_to_key(key);
				}	
				else if(control == "prevweapon_" + g_engine->inttostring(this->id+1))
				{ 
					this->PREV_WEAPON = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon1_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON0 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon2_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON1 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon3_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON2 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon4_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON3 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon5_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON4 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon6_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON5 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon7_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON6 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon8_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON7 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon9_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON8 = g_engine->gui->string_to_key(key);
				}
				else if(control == "weapon0_" + g_engine->inttostring(this->id+1))
				{ 
					this->WEAPON9 = g_engine->gui->string_to_key(key);
				}
			}			
		}

		file.close();

		return true;

	}//Load()

	void PlayerControls::UpdateControls()
	{
		this->LEFT = g_engine->gui->string_to_key(this->left->getText());
		this->RIGHT = g_engine->gui->string_to_key(this->right->getText());
		this->UP = g_engine->gui->string_to_key(this->up->getText());
		this->DOWN = g_engine->gui->string_to_key(this->down->getText());
		this->JUMP = g_engine->gui->string_to_key(this->jump->getText());
		this->FIRE1 = g_engine->gui->string_to_key(this->fire1->getText());
		this->FIRE2 = g_engine->gui->string_to_key(this->fire2->getText());
		this->ACTION = g_engine->gui->string_to_key(this->action->getText());
		this->PDA = g_engine->gui->string_to_key(this->pda->getText());
		this->C_FLY = g_engine->gui->string_to_key(this->b_fly->getText());
		this->BULLETTIME = g_engine->gui->string_to_key(this->b_bullettime->getText());
		//weapon selection
		this->NEXT_WEAPON = g_engine->gui->string_to_key(this->next_weapon->getText());
		this->PREV_WEAPON = g_engine->gui->string_to_key(this->prev_weapon->getText());
		this->WEAPON0 = g_engine->gui->string_to_key(this->weapon0->getText());
		this->WEAPON1 = g_engine->gui->string_to_key(this->weapon1->getText());
		this->WEAPON2 = g_engine->gui->string_to_key(this->weapon2->getText());
		this->WEAPON3 = g_engine->gui->string_to_key(this->weapon3->getText());
		this->WEAPON4 = g_engine->gui->string_to_key(this->weapon4->getText());
		this->WEAPON5 = g_engine->gui->string_to_key(this->weapon5->getText());
		this->WEAPON6 = g_engine->gui->string_to_key(this->weapon6->getText());
		this->WEAPON7 = g_engine->gui->string_to_key(this->weapon7->getText());	
		this->WEAPON8 = g_engine->gui->string_to_key(this->weapon8->getText());
		this->WEAPON9 = g_engine->gui->string_to_key(this->weapon9->getText());	


	}//UpdateControls()

	void PlayerControls::getPlayerControls(
		int &LEFT,int &RIGHT,int &UP,int &DOWN,int &JUMP,
		int &FIRE1,int &FIRE2,
		int &ACTION,int &PDA,
		int &C_FLY,int &BULLETTIME,
		int &NEXT_WEAPON,int &PREV_WEAPON,
		int &WEAPON0,int &WEAPON1,int &WEAPON2,int &WEAPON3,
		int &WEAPON4,int &WEAPON5,int &WEAPON6,int &WEAPON7,int &WEAPON8,int &WEAPON9)
	{
		LEFT = this->LEFT;
		RIGHT = this->RIGHT;
		UP = this->UP;
		DOWN = this->DOWN;
		JUMP = this->JUMP;
		FIRE1 = this->FIRE1;
		FIRE2 = this->FIRE2;
		ACTION = this->ACTION;
		PDA = this->PDA;
		C_FLY = this->C_FLY;
		BULLETTIME = this->BULLETTIME;

		NEXT_WEAPON = this->NEXT_WEAPON;
		PREV_WEAPON = this->PREV_WEAPON;
		WEAPON0 = this->WEAPON0;
		WEAPON1 = this->WEAPON1;
		WEAPON2 = this->WEAPON2;
		WEAPON3 = this->WEAPON3;
		WEAPON4 = this->WEAPON4;
		WEAPON5 = this->WEAPON5;
		WEAPON6 = this->WEAPON6;
		WEAPON7 = this->WEAPON7;	
		WEAPON8 = this->WEAPON8;
		WEAPON9 = this->WEAPON9;	

	}//getPlayerControls()	

}//Advanced2D