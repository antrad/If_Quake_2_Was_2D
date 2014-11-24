/* Antonio R.*/
#include "Advanced2D.h"

namespace Advanced2D
{
	GUI::GUI(void)
	{
		console_text.push_back("What is thy bidding, my Master ?");
		console_text.push_back("");
		console_text.push_back("");

		this->collision_order = 1;

		this->selected_button = NULL;
		this->selected_sprite = NULL;
		this->cursor = NULL;
		this->console_panel = NULL;
		this->default_t = NULL;

	}//GUI()

	GUI::~GUI(void)
	{
		if (cursor) delete cursor;
		if (console_panel) delete console_panel;
		if (default_t) delete default_t;

	}//~GUI()

	bool GUI::LoadTextures(void)
	{
		default_t = new Texture();
		if (!default_t->Load("System/GUI/default_t.tga"))
		{
			g_engine->message("Error loading ...System/GUI/default_t.tga");
			return false;
		}
		return true;

	}//LoadTextures

	void GUI::addButton(Button *button)
	{
		p_buttons.push_back(button);

	}//addButton

	Button* GUI::CursorInsideAButton()
	{
		//the button that is most front will collide first, making ones behind non clickable
		std::list <Button*>::reverse_iterator i = p_buttons.rbegin();

		//only one button per frame
		while (i != p_buttons.rend())
		{
			if ((*i)->getCollidable() && (*i)->getVisible() ) 
			{
				if (g_engine->coll_manager->dot_dotrect(this->cursor->getX(),this->cursor->getY(),(*i)->getX(),(*i)->getY(),(*i)->getWidth(),(*i)->getHeight()))
				{
					return (*i);
				}
			}
			i++;
		}

		return NULL;

	}//CursorInsideAButton()()

	bool GUI::CheckButtonsPress(int mousebutton)
	{
		//the button that is most front will collide first
		std::list <Button*>::reverse_iterator i = p_buttons.rbegin();

		//only one button per frame
		while (i != p_buttons.rend())
		{
			if ((*i)->getCollidable() && (*i)->getVisible())
			{
				if (g_engine->coll_manager->dot_dotrect(this->cursor->getX(), this->cursor->getY(),(*i)->getX(),(*i)->getY(),(*i)->getWidth(),(*i)->getHeight()))
				{
					//check is some common button class pressed
					switch ((*i)->getClass())
					{
					case SLIDER_BODY:
						if((*i)->getID()==SLIDER_H)
							this->update_slider_h((*i),this->cursor->getX(),this->cursor->getY());
						else
							this->update_slider_v((*i),this->cursor->getX(),this->cursor->getY());
						break;
					}//switch(class)

					GUI_ButtonPress((*i),mousebutton);
					//button pressed skip cursor-sprite check
					return true;
				}
			}
			i++;
		}

		return false;

	}//CheckButtonsPress()	

	bool GUI::CheckButtonsRelease(int mousebutton)
	{
		//the button that is most front will collide first
		Button *a;
		std::list <Button*>::reverse_iterator i = p_buttons.rbegin();

		//only one button per frame
		while (i != p_buttons.rend())
		{
			a=(*i);

			if (a->getCollidable() && a->getVisible() )
			{
				if (g_engine->coll_manager->dot_dotrect(this->cursor->getX(),this->cursor->getY(),a->getX(),a->getY(),a->getWidth(),a->getHeight()))
				{
					//check is some common button class pressed
					switch (a->getClass())
					{
					case FORM_CLOSE:
						this->form_close(a);
						break;

					case TEXTBOX:
						if (mousebutton==0) g_engine->gui->setSelectedButton(a);
						break;

					case DROPBOX_ARROW:
						if (a->getState()==0)
						{
							this->dropbox_hide();//to close other opened dropboxes						
							this->dropbox_show(a->parent,a);
						}
						else
							this->dropbox_hide();
						break;

					case DROPBOX_OPTION:
						a->parent->setText(a->getText());
						this->dropbox_hide();
						break;

					case CHECKBOX:
						if (a->getState()==0)
						{
							a->setState(1);
							a->setText(a->text1);//yes
							a->setColor_Texture(0xFF00FF00);
						}
						else if (a->getState()==1)
						{
							a->setState(0);
							a->setText(a->text2);//no
							a->setColor_Texture(0xFFFF0000);
						}						
						break;

					}//switch(class)

					//inform game about button release
					GUI_ButtonRelease(a,mousebutton);
					//if something other than textbox is pressed release attached button
					if (a->getClass()!=TEXTBOX && g_engine->getGameState()!=PLAY)
					{
						g_engine->gui->setSelectedButton(NULL);
					}
					if (a->getClass()!=DROPBOX_ARROW)
					{
						this->dropbox_hide();
					}
					//return true to skip cursor-sprite collision
					return true;
				}
			}
			i++;
		}
		//no button pressed, release attached button, delte dropbox options
		this->dropbox_hide();		

		return false;//check cursor-sprite collision

	}//CheckButtonsRelease

	void GUI::Update()
	{
		std::list<Button*>::iterator iter = this->p_buttons.begin();
		while (iter != this->p_buttons.end())
		{
			if((*iter)->getAlive()) GUI_ButtonUpdate(*iter);
			iter++;
		}
	}

	void GUI::delete_GuiButtons()
	{
		Button *a ;
		std::list<Button*>::iterator iter = this->p_buttons.begin();

		while (iter != this->p_buttons.end())
		{
			a = *iter;
			a->setAlive(false);
			iter++;
		}
	}

	void GUI::BuryButtons()
	{
		std::list <Button*>::iterator i = p_buttons.begin();

		while (i != p_buttons.end())
		{
			//see if this entity will auto-expire
			if ( (*i)->getLifetime() > 0)
			{
				(*i)->setAge( (*i)->getAge() + 1*g_engine->game_speed*g_engine->GetDeltaFix() );
				if ( (*i)->getAge() >= (*i)->getLifetime())
				{
					this->form_delete((*i));;
				}
			}				

			if ( (*i)->getAlive() == false )
			{
				//delete dropbox-options inside dropbox form (because only copies are in gui list)
				if ((*i)->getClass()==DROPBOX)
				{
					std::vector <Button*>::iterator j = (*i)->form.begin();
					while (j != (*i)->form.end())
					{
						if((*j)->getClass()==DROPBOX_OPTION)
						{
							delete (*j);
							j = (*i)->form.erase( j );
						}
						else j++;
					}
				}

				delete (*i);
				i = p_buttons.erase( i );
			}
			else i++;
		}

	}//DeleteButtons

	void GUI::DrawGUI_Buttons()
	{
		Button* me_first = CursorInsideAButton();

		Button* RenderOverButton = NULL;

		std::list <Button*>::iterator i = p_buttons.begin();
		while (i != p_buttons.end())////draw button texture, text and bounding box
		{
			Button *a = *i;

			if (a->getVisible() && a->getAlive()) 
			{
				//draw texture
				a->drawtexture(a->getColor_Texture());

				//draw text
				if (a->font_png)
				{
					if( a->text_positioning == 'C' )
					{
						if(g_engine->font_manager.fonts.size()!=0)
							g_engine->font_manager.fonts[0]->PrintText(
							a->getText(),
							(a->getX()+a->getWidth()/2)-(g_engine->font_manager.fonts[0]->getLength(a->getText(),g_engine->font_manager.fonts[0]->char_dist,1))/2,
							a->getY()+(a->getHeight()-g_engine->font_manager.fonts[0]->getHeight()*g_engine->font_manager.fonts[0]->scale)/2,
							1, g_engine->font_manager.fonts[0]->char_dist,
							a->getColor_Text());
					}
					else if( a->text_positioning == 'R' )
					{
						if(g_engine->font_manager.fonts.size()!=0)
							g_engine->font_manager.fonts[0]->PrintTextRight(
							a->getText(),
							a->getX()+a->getWidth(),							
							a->getY()+g_engine->font_manager.fonts[0]->getHeight()*g_engine->font_manager.fonts[0]->scale/2,
							1, g_engine->font_manager.fonts[0]->char_dist,
							a->getColor_Text());
					}
					else
					{
						if(g_engine->font_manager.fonts.size()!=0)
							g_engine->font_manager.fonts[0]->PrintText(
							a->getText(),
							a->getX(),a->getY(),
							1, g_engine->font_manager.fonts[0]->char_dist,
							a->getColor_Text());
					}

					if (!a->page.empty())
					{
						if(g_engine->font_manager.fonts.size()!=0)
						{
							if( a->text_positioning == 'R' )
							{
								g_engine->font_manager.fonts[0]->PrintTextRight(
									a->page,
									a->getX()+a->getWidth(),a->getY(),
									1,g_engine->font_manager.fonts[0]->char_dist,
									a->getColor_Text());
							}
							else
							{
								g_engine->font_manager.fonts[0]->PrintText(
									a->page,
									a->getX(),a->getY(),
									1,g_engine->font_manager.fonts[0]->char_dist,
									a->getColor_Text());
							}
						}
					}
				}
				else
				{
					g_engine->ar_graphics->Render2D_Stop();

					g_engine->e_font->write_text(a->getText(),
						(int)a->getX(),(int)a->getY()+2,(int)a->getWidth(),(int)a->getHeight(),
						a->text_positioning,a->getColor_Text());

					//draw text
					if (!a->page.empty())
					{
						g_engine->e_font->write_stringvector2(a->page,
							a->getX(),a->getY(),a->getWidth(),a->getHeight(),
							a->text_positioning,a->row_height,a->getColor_Text());						
					}

					g_engine->ar_graphics->Render2D_Start();
				}

				if (me_first!=NULL)//render bounding box for a button over which the cursor is
				{
					if (a->clickable && a==me_first)
					{
						RenderOverButton = me_first;
						me_first = NULL;//finished, don't check for others
					}
				}					

				if (a->selected)
				{
					g_engine->ar_graphics->draw_box(a->getX(),a->getY(),a->getWidth(),a->getHeight(),0,0,0xFFFFFF00);
				}

			}
			
			a->cursor_insidepast = a->cursor_inside;
			a->cursor_inside = false;
			i++;

		}//while

		if (g_engine->gui->getSelectedButton()!=NULL)
		{
			Button *a = g_engine->gui->getSelectedButton();
			g_engine->ar_graphics->draw_box(a->getX(),a->getY(),a->getWidth(),a->getHeight(),0,0,0xFFFF8800);
			
		}

		if(RenderOverButton)
		{
			if (g_engine->p_input->GetMouseButton(0))//cursor is inside and mouse is pressed
			{
				GUI_RenderCursorButtonPress(RenderOverButton);								
			}
			else//cursor is inside, but mouse is not pressed
			{
				GUI_RenderCursorOverButton(RenderOverButton);								
			}

		}

		//draw cursor		
		if (g_engine->gui->cursor->getVisible()) g_engine->ar_graphics->draw_sprite(g_engine->gui->cursor,0,0);	

		if(this->getSelectedButton())
		{
			if(!this->getSelectedButton()->getVisible()) this->setSelectedButton(NULL);
		}

	}//DrawGUI()

	void GUI::DrawGUI_Cursor()
	{
		if (g_engine->gui->cursor->getVisible()) g_engine->ar_graphics->draw_sprite(g_engine->gui->cursor,0,0);		

	}//DrawGUI()

	std::string GUI::text_input(int key, std::string edit_text)
	{
		std::stringstream temp_textstream;
		temp_textstream << edit_text;

		std::string temp_string="";

		switch(key)
		{
			//DELETE
		case DIK_BACK:
			if (temp_textstream.str().size()==0) break;
			temp_string=temp_textstream.str();
			temp_string.erase(temp_string.length()-1);
			temp_textstream.str("");
			temp_textstream << temp_string;
			break;
		case DIK_DELETE:temp_textstream.str("");break;

			////MISC		
		case DIK_EQUALS:temp_textstream<<"=";break;//radi ali je na krivom mistu
		case DIK_SPACE:temp_textstream<<" ";break;			
		case DIK_COMMA:case DIK_PERIOD:case DIK_DECIMAL:temp_textstream<<".";break;
			//NUMPAD OTHER
		case DIK_SLASH:case DIK_NUMPADSLASH:temp_textstream<<"/";break;
		
		case DIK_MULTIPLY:
			if(g_engine->p_input->GetKeyState(DIK_RSHIFT)&0x80 || g_engine->p_input->GetKeyState(DIK_LSHIFT)&0x80)
				temp_textstream<<"&";
			else 
				temp_textstream<<"*";

		case DIK_ADD:temp_textstream<<"+";break;

		case DIK_SUBTRACT:
			if(g_engine->p_input->GetKeyState(DIK_RSHIFT)&0x80 || g_engine->p_input->GetKeyState(DIK_LSHIFT)&0x80)
				temp_textstream<<"_";
			else 
				temp_textstream<<"-";
			break;			

			//NUMBERS
		case DIK_1:case DIK_NUMPAD1:temp_textstream<<"1";break;
		case DIK_2:case DIK_NUMPAD2:temp_textstream<<"2";break;
		case DIK_3:case DIK_NUMPAD3:temp_textstream<<"3";break;
		case DIK_4:case DIK_NUMPAD4:temp_textstream<<"4";break;
		case DIK_5:case DIK_NUMPAD5:temp_textstream<<"5";break;
		case DIK_6:case DIK_NUMPAD6:temp_textstream<<"6";break;
		case DIK_7:case DIK_NUMPAD7:temp_textstream<<"7";break;
		case DIK_8:case DIK_NUMPAD8:temp_textstream<<"8";break;
		case DIK_9:case DIK_NUMPAD9:temp_textstream<<"9";break;
		case DIK_0:case DIK_NUMPAD0:temp_textstream<<"0";break;			

		}//key

		if(g_engine->p_input->GetKeyState(DIK_RSHIFT)&0x80 || g_engine->p_input->GetKeyState(DIK_LSHIFT)&0x80)
		{
			switch(key)//LETTERS
			{				
			case DIK_A:temp_textstream<<"A";break;
			case DIK_B:temp_textstream<<"B";break;
			case DIK_C:temp_textstream<<"C";break;
			case DIK_D:temp_textstream<<"D";break;
			case DIK_E:temp_textstream<<"E";break;
			case DIK_F:temp_textstream<<"F";break;
			case DIK_G:temp_textstream<<"G";break;
			case DIK_H:temp_textstream<<"H";break;
			case DIK_I:temp_textstream<<"I";break;
			case DIK_J:temp_textstream<<"J";break;
			case DIK_K:temp_textstream<<"K";break;
			case DIK_L:temp_textstream<<"L";break;
			case DIK_M:temp_textstream<<"M";break;
			case DIK_N:temp_textstream<<"N";break;
			case DIK_O:temp_textstream<<"O";break;
			case DIK_P:temp_textstream<<"P";break;
			case DIK_Q:temp_textstream<<"Q";break;
			case DIK_R:temp_textstream<<"R";break;
			case DIK_S:temp_textstream<<"S";break;
			case DIK_T:temp_textstream<<"T";break;
			case DIK_U:temp_textstream<<"U";break;
			case DIK_V:temp_textstream<<"V";break;
			case DIK_W:temp_textstream<<"W";break;
			case DIK_X:temp_textstream<<"X";break;
			case DIK_Z:temp_textstream<<"Y";break;
			case DIK_Y:temp_textstream<<"Z";break;
			}//key
		}
		else
		{
			switch(key)//LETTERS
			{				
			case DIK_A:temp_textstream<<"a";break;
			case DIK_B:temp_textstream<<"b";break;
			case DIK_C:temp_textstream<<"c";break;
			case DIK_D:temp_textstream<<"d";break;
			case DIK_E:temp_textstream<<"e";break;
			case DIK_F:temp_textstream<<"f";break;
			case DIK_G:temp_textstream<<"g";break;
			case DIK_H:temp_textstream<<"h";break;
			case DIK_I:temp_textstream<<"i";break;
			case DIK_J:temp_textstream<<"j";break;
			case DIK_K:temp_textstream<<"k";break;
			case DIK_L:temp_textstream<<"l";break;
			case DIK_M:temp_textstream<<"m";break;
			case DIK_N:temp_textstream<<"n";break;
			case DIK_O:temp_textstream<<"o";break;
			case DIK_P:temp_textstream<<"p";break;
			case DIK_Q:temp_textstream<<"q";break;
			case DIK_R:temp_textstream<<"r";break;
			case DIK_S:temp_textstream<<"s";break;
			case DIK_T:temp_textstream<<"t";break;
			case DIK_U:temp_textstream<<"u";break;
			case DIK_V:temp_textstream<<"v";break;
			case DIK_W:temp_textstream<<"w";break;
			case DIK_X:temp_textstream<<"x";break;
			case DIK_Z:temp_textstream<<"y";break;
			case DIK_Y:temp_textstream<<"z";break;
			}//key
		}

		return temp_textstream.str();	

	}//text_input()

	int GUI::string_to_key(std::string text)
	{
		if (text=="\"NUMPAD_ENTER\"") return DIK_NUMPADENTER;
		if (text=="\"NUMPAD_ADD\"") return DIK_ADD;
		if (text=="\"NUMPAD_SUBTRACT\"") return DIK_SUBTRACT;
		if (text=="\"NUMPAD_0\"") return DIK_NUMPAD0;
		if (text=="\"NUMPAD_1\"") return DIK_NUMPAD1;
		if (text=="\"NUMPAD_2\"") return DIK_NUMPAD2;
		if (text=="\"NUMPAD_3\"") return DIK_NUMPAD3;
		if (text=="\"NUMPAD_4\"") return DIK_NUMPAD4;
		if (text=="\"NUMPAD_5\"") return DIK_NUMPAD5;
		if (text=="\"NUMPAD_6\"") return DIK_NUMPAD6;
		if (text=="\"NUMPAD_7\"") return DIK_NUMPAD7;
		if (text=="\"NUMPAD_8\"") return DIK_NUMPAD8;
		if (text=="\"NUMPAD_9\"") return DIK_NUMPAD9;

		if (text=="\"ENTER\"") return DIK_RETURN;
		if (text=="\"SPACE\"") return DIK_SPACE;
		if (text=="\"TAB\"") return DIK_TAB;

		if (text=="\"BACKSPACE\"") return DIK_BACKSPACE;
		if (text=="\"LEFT_SHIFT\"") return DIK_LSHIFT;
		if (text=="\"RIGHT_SHIFT\"") return DIK_RSHIFT;
		if (text=="\"LEFT_CTRL\"") return DIK_LCONTROL;
		if (text=="\"RIGHT_CTRL\"") return DIK_RCONTROL;

		if (text=="\"ARROW_LEFT\"") return DIK_LEFT;
		if (text=="\"ARROW_RIGHT\"") return DIK_RIGHT;
		if (text=="\"ARROW_UP\"") return DIK_UP;
		if (text=="\"ARROW_DOWN\"") return DIK_DOWN;

		if (text=="\"INSERT\"") return DIK_INSERT;
		if (text=="\"HOME\"") return DIK_HOME;
		if (text=="\"DELETE\"") return DIK_DELETE;
		if (text=="\"END\"") return DIK_END;
		if (text=="\"PAGE_DOWN\"") return DIK_NEXT;
		if (text=="\"PAGE_UP\"") return DIK_PRIOR;

		if (text=="\"F1\"") return DIK_F1;
		if (text=="\"F2\"") return DIK_F2;
		if (text=="\"F3\"") return DIK_F3;
		if (text=="\"F4\"") return DIK_F4;
		if (text=="\"F5\"") return DIK_F5;
		if (text=="\"F6\"") return DIK_F6;
		if (text=="\"F7\"") return DIK_F7;
		if (text=="\"F8\"") return DIK_F8;
		if (text=="\"F9\"") return DIK_F9;
		if (text=="\"F10\"") return DIK_F10;
		if (text=="\"F11\"") return DIK_F11;
		if (text=="\"F12\"") return DIK_F12;

		if (text=="\"0\"") return DIK_0;
		if (text=="\"1\"") return DIK_1;
		if (text=="\"2\"") return DIK_2;
		if (text=="\"3\"") return DIK_3;
		if (text=="\"4\"") return DIK_4;
		if (text=="\"5\"") return DIK_5;
		if (text=="\"6\"") return DIK_6;
		if (text=="\"7\"") return DIK_7;
		if (text=="\"8\"") return DIK_8;
		if (text=="\"9\"") return DIK_9;

		if (text=="\"a\"") return DIK_A;
		if (text=="\"b\"") return DIK_B;
		if (text=="\"c\"") return DIK_C;
		if (text=="\"d\"") return DIK_D;
		if (text=="\"e\"") return DIK_E;
		if (text=="\"f\"") return DIK_F;
		if (text=="\"g\"") return DIK_G;
		if (text=="\"h\"") return DIK_H;
		if (text=="\"i\"") return DIK_I;
		if (text=="\"j\"") return DIK_J;
		if (text=="\"k\"") return DIK_K;
		if (text=="\"l\"") return DIK_L;
		if (text=="\"m\"") return DIK_M;
		if (text=="\"n\"") return DIK_N;
		if (text=="\"o\"") return DIK_O;
		if (text=="\"p\"") return DIK_P;
		if (text=="\"q\"") return DIK_Q;
		if (text=="\"r\"") return DIK_R;
		if (text=="\"s\"") return DIK_S;
		if (text=="\"t\"") return DIK_T;
		if (text=="\"u\"") return DIK_U;
		if (text=="\"v\"") return DIK_V;
		if (text=="\"w\"") return DIK_W;
		if (text=="\"x\"") return DIK_X;
		if (text=="\"y\"") return DIK_Y;
		if (text=="\"z\"") return DIK_Z;

		if (text=="\"MOUSE_0\"") return 0;
		if (text=="\"MOUSE_1\"") return -1;
		if (text=="\"MOUSE_2\"") return -2;
		if (text=="\"MOUSE_3\"") return -3;
		if (text=="\"MOUSE_4\"") return -4;
		if (text=="\"MOUSE_5\"") return -5;
		if (text=="\"MOUSE_6\"") return -6;
		if (text=="\"MOUSE_7\"") return -7;

		if (text=="\"WHEEL_UP\"") return 1200;
		if (text=="\"WHEEL_DOWN\"") return -1200;

		if (text=="\"JOY_0\"") return 1000;
		if (text=="\"JOY_1\"") return 1001;
		if (text=="\"JOY_2\"") return 1002;
		if (text=="\"JOY_3\"") return 1003;
		if (text=="\"JOY_4\"") return 1004;
		if (text=="\"JOY_5\"") return 1005;
		if (text=="\"JOY_6\"") return 1006;
		if (text=="\"JOY_7\"") return 1007;
		if (text=="\"JOY_8\"") return 1008;
		if (text=="\"JOY_9\"") return 1009;
		if (text=="\"JOY_10\"") return 1010;
		if (text=="\"JOY_11\"") return 1011;

		if (text=="\"JOY_X+\"") return -1000;
		if (text=="\"JOY_X-\"") return -1001;
		if (text=="\"JOY_Y+\"") return -1002;
		if (text=="\"JOY_Y-\"") return -1003;		

		return -999;

	}//STRING_TO_KEY

	std::string GUI::key_to_string(int key)
	{
		if (key==DIK_NUMPADENTER) return "\"NUMPAD_ENTER\"";
		if (key==DIK_ADD) return "\"NUMPAD_ADD\"";
		if (key==DIK_SUBTRACT) return "\"NUMPAD_SUBTRACT\"";
		if (key==DIK_NUMPAD0) return "\"NUMPAD_0\"";
		if (key==DIK_NUMPAD1) return "\"NUMPAD_1\"";
		if (key==DIK_NUMPAD2) return "\"NUMPAD_2\"";
		if (key==DIK_NUMPAD3) return "\"NUMPAD_3\"";
		if (key==DIK_NUMPAD4) return "\"NUMPAD_4\"";
		if (key==DIK_NUMPAD5) return "\"NUMPAD_5\"";
		if (key==DIK_NUMPAD6) return "\"NUMPAD_6\"";
		if (key==DIK_NUMPAD7) return "\"NUMPAD_7\"";
		if (key==DIK_NUMPAD8) return "\"NUMPAD_8\"";
		if (key==DIK_NUMPAD9) return "\"NUMPAD_9\"";

		if (key==DIK_RETURN) return "\"ENTER\"";
		if (key==DIK_SPACE) return "\"SPACE\"";
		if (key==DIK_TAB) return "\"TAB\"";

		if (key==DIK_BACKSPACE) return "\"BACKSPACE\"";
		if (key==DIK_LSHIFT) return "\"LEFT_SHIFT\"";
		if (key==DIK_RSHIFT) return "\"RIGHT_SHIFT\"";
		if (key==DIK_LCONTROL) return "\"LEFT_CTRL\"";
		if (key==DIK_RCONTROL) return "\"RIGHT_CTRL\"";

		if (key==DIK_INSERT) return "\"INSERT\"";
		if (key==DIK_HOME) return "\"HOME\"";
		if (key==DIK_DELETE) return "\"DELETE\"";
		if (key==DIK_END) return "\"END\"";
		if (key==DIK_NEXT) return "\"PAGE_DOWN\"";
		if (key==DIK_PRIOR) return "\"PAGE_UP\"";

		if (key==DIK_LEFT) return "\"ARROW_LEFT\"";
		if (key==DIK_RIGHT) return "\"ARROW_RIGHT\"";
		if (key==DIK_UP) return "\"ARROW_UP\"";
		if (key==DIK_DOWN) return "\"ARROW_DOWN\"";

		if (key==DIK_F1) return "\"F1\"";
		if (key==DIK_F2) return "\"F2\"";
		if (key==DIK_F3) return "\"F3\"";
		if (key==DIK_F4) return "\"F4\"";
		if (key==DIK_F5) return "\"F5\"";
		if (key==DIK_F6) return "\"F6\"";
		if (key==DIK_F7) return "\"F7\"";
		if (key==DIK_F8) return "\"F8\"";
		if (key==DIK_F9) return "\"F9\"";
		if (key==DIK_F10) return "\"F10\"";
		if (key==DIK_F11) return "\"F11\"";
		if (key==DIK_F12) return "\"F12\"";		

		if (key==DIK_0) return "\"0\"";
		if (key==DIK_1) return "\"1\"";
		if (key==DIK_2) return "\"2\"";
		if (key==DIK_3) return "\"3\"";
		if (key==DIK_4) return "\"4\"";
		if (key==DIK_5) return "\"5\"";
		if (key==DIK_6) return "\"6\"";
		if (key==DIK_7) return "\"7\"";
		if (key==DIK_8) return "\"8\"";
		if (key==DIK_9) return "\"9\"";

		if (key==DIK_A) return "\"a\"";
		if (key==DIK_B) return "\"b\"";
		if (key==DIK_C) return "\"c\"";
		if (key==DIK_D) return "\"d\"";
		if (key==DIK_E) return "\"e\"";
		if (key==DIK_F) return "\"f\"";
		if (key==DIK_G) return "\"g\"";
		if (key==DIK_H) return "\"h\"";
		if (key==DIK_I) return "\"i\"";
		if (key==DIK_J) return "\"j\"";
		if (key==DIK_K) return "\"k\"";
		if (key==DIK_L) return "\"l\"";
		if (key==DIK_M) return "\"m\"";
		if (key==DIK_N) return "\"n\"";
		if (key==DIK_O) return "\"o\"";
		if (key==DIK_P) return "\"p\"";
		if (key==DIK_Q) return "\"q\"";
		if (key==DIK_R) return "\"r\"";
		if (key==DIK_S) return "\"s\"";
		if (key==DIK_T) return "\"t\"";
		if (key==DIK_U) return "\"u\"";
		if (key==DIK_V) return "\"v\"";
		if (key==DIK_W) return "\"w\"";
		if (key==DIK_X) return "\"x\"";
		if (key==DIK_Y) return "\"y\"";
		if (key==DIK_Z) return "\"z\"";		

		if (key==0) return "\"MOUSE_0\"";
		if (key==-1) return "\"MOUSE_1\"";
		if (key==-2) return "\"MOUSE_2\"";
		if (key==-3) return "\"MOUSE_3\"";
		if (key==-4) return "\"MOUSE_4\"";
		if (key==-5) return "\"MOUSE_5\"";
		if (key==-6) return "\"MOUSE_6\"";
		if (key==-7) return "\"MOUSE_7\"";	

		if (key==1200) return "\"WHEEL_UP\"";
		if (key==-1200) return "\"WHEEL_DOWN\"";	

		if (key==1000) return "\"JOY_0\"";
		if (key==1001) return "\"JOY_1\"";
		if (key==1002) return "\"JOY_2\"";
		if (key==1003) return "\"JOY_3\"";	
		if (key==1004) return "\"JOY_4\"";
		if (key==1005) return "\"JOY_5\"";
		if (key==1006) return "\"JOY_6\"";
		if (key==1007) return "\"JOY_7\"";	
		if (key==1008) return "\"JOY_8\"";
		if (key==1009) return "\"JOY_9\"";
		if (key==1010) return "\"JOY_10\"";
		if (key==1011) return "\"JOY_11\"";	

		if (key==-1000) return "\"JOY_X+\"";
		if (key==-1001) return "\"JOY_X-\"";
		if (key==-1002) return "\"JOY_Y+\"";
		if (key==-1003) return "\"JOY_Y-\"";	

		return "\"#NA#\"";

	}//STRING_TO_KEY

	///////////////////////
	//	FORM MANAGEMENT	 //
	///////////////////////

	Button* GUI::form_add(std::string title, int x, int y, int w, int h, char pos,bool hide_background)
	{
		Button *background = new Button();

		if (hide_background)
		{			
			background->setImage(default_t);
			background->setColor_Texture(0x88000000);
			//cover all screen
			background->setSize(g_engine->getScreenWidth(),g_engine->getScreenHeight());
			background->setScaleX((double)g_engine->getScreenWidth()/background->getImage()->getWidth());
			background->setScaleY((double)g_engine->getScreenHeight()/background->getImage()->getHeight());
			//
			background->setPosition(0,0);
			background->clickable=false;
			g_engine->gui->addButton(background);
		}

		Button *form = new Button();
		form->setImage(default_t);
		form->setText(title);
		form->setColor_Texture(0xFF666666);
		form->setColor_Box(0xFFBBBBBB);
		form->setSize(w,h);

		if (pos=='C') form->setPosition(x-w/2,y-h/2);
		else if (pos=='L') form->setPosition(x,y);

		form->clickable=false;
		g_engine->gui->addButton(form);

		Button *panel = new Button();
		panel->setImage(default_t);
		panel->setColor_Texture(0xFFAAAAAA);
		panel->setColor_Box(0xFFBBBBBB);
		panel->setColor_Text(0xFF000000);
		panel->setPosition(form->getX()+8,form->getY()+28);
		panel->setSize(form->getWidth()-16,form->getHeight()-4-32);
		panel->clickable=false;
		g_engine->gui->addButton(panel);

		Button *close = new Button();
		close->setID(FORM_CLOSE);
		close->setType(FORM_CLOSE);
		close->setClass(FORM_CLOSE);
		close->setImage(default_t);
		close->setText("X");
		close->setColor_Texture(0xFF880000);
		close->setColor_Box(0xFFBBBBBB);
		close->setSize(20,20);
		close->setPosition(form->getX()+form->getWidth()-close->getWidth()-8,form->getY()+4);
		g_engine->gui->addButton(close);

		form->form.push_back(close);
		form->form.push_back(panel);

		if (hide_background)
			form->form.push_back(background);
		else
			delete background;		

		close->parent=form;
		return form;

	}

	Button* GUI::form_add_yesno(std::string title, std::string message, int w, int h)
	{
		Button *background = new Button();

		background->setImage(default_t);
		background->setColor_Texture(0x66000000);
		background->setColor_Box(0x66000000);
		//cover all screen
		background->setSize(g_engine->getScreenWidth(),g_engine->getScreenHeight());
		background->setScaleX((double)g_engine->getScreenWidth()/background->getImage()->getWidth());
		background->setScaleY((double)g_engine->getScreenHeight()/background->getImage()->getHeight());
		//
		background->setPosition(0,0);
		background->clickable=false;
		g_engine->gui->addButton(background);		

		Button *form = new Button();
		form->setImage(default_t);
		form->setText(title);
		form->setColor_Texture(0xFF666666);
		form->setColor_Box(0xFFBBBBBB);
		form->setSize(w,h);
		form->setPosition(g_engine->getScreenWidth()/2-w/2,g_engine->getScreenHeight()/2-h/2);
		form->clickable=false;
		g_engine->gui->addButton(form);

		Button *panel = new Button();
		panel->setImage(default_t);
		panel->setText(message);
		panel->setColor_Texture(0xFFAAAAAA);
		panel->setColor_Box(0xFFBBBBBB);
		panel->setColor_Text(0xFF000000);
		panel->setPosition(form->getX()+2,form->getY()+28);
		panel->setSize(form->getWidth()-4,form->getHeight()-4-32);
		panel->clickable=false;
		g_engine->gui->addButton(panel);

		Button *close = new Button();
		close->setID(FORM_CLOSE);
		close->setType(FORM_CLOSE);
		close->setClass(FORM_CLOSE);
		close->setImage(default_t);
		close->setText("X");
		close->setColor_Texture(0xFF880000);
		close->setColor_Box(0xFFBBBBBB);
		close->setSize(20,20);
		close->setPosition(form->getX()+form->getWidth()-close->getWidth()-4,form->getY()+4);
		g_engine->gui->addButton(close);

		Button *yes = new Button();
		yes->setImage(default_t);
		yes->setText("YES");
		yes->setColor_Texture(0xFF888888);
		yes->setColor_Box(0xFFDDDDDD);
		yes->setSize(60,25);
		yes->setPosition(form->getX()+form->getWidth()/2-yes->getWidth()-20,form->getY()+form->getHeight()-45);
		g_engine->gui->addButton(yes);

		Button *no = new Button();
		no->setID(FORM_CLOSE);
		no->setType(FORM_CLOSE);
		no->setClass(FORM_CLOSE);
		no->setImage(default_t);
		no->setText("NO");
		no->setColor_Texture(0xFF888888);
		no->setColor_Box(0xFFDDDDDD);
		no->setSize(60,25);
		no->setPosition(form->getX()+form->getWidth()/2+20,form->getY()+form->getHeight()-45);
		g_engine->gui->addButton(no);

		form->form.push_back(close);
		form->form.push_back(panel);
		form->form.push_back(background);
		form->form.push_back(yes);
		form->form.push_back(no);			

		close->parent=form;
		no->parent=form;
		yes->parent=form;

		return form;

	}


	Button* GUI::form_add_textinput(std::string title, int x, int y, int w, int h, bool hide_background)
	{
		Button *background = new Button();

		if (hide_background)
		{			
			background->setImage(default_t);
			background->setColor_Texture(0x88000000);
			//cover all screen
			background->setSize(g_engine->getScreenWidth(),g_engine->getScreenHeight());
			background->setScaleX((double)g_engine->getScreenWidth()/background->getImage()->getWidth());
			background->setScaleY((double)g_engine->getScreenHeight()/background->getImage()->getHeight());
			//
			background->setPosition(0,0);
			background->clickable=false;
			g_engine->gui->addButton(background);
		}

		Button *form = new Button();
		form->setImage(default_t);
		form->setText(title);
		form->setColor_Texture(0xFF666666);
		form->setColor_Box(0xFFBBBBBB);
		form->setSize(w,h);
		form->setPosition(x,y);

		form->clickable=false;
		g_engine->gui->addButton(form);

		Button *panel = new Button();
		panel->setImage(default_t);
		panel->setColor_Texture(0xFFAAAAAA);
		panel->setColor_Box(0xFFBBBBBB);
		panel->setColor_Text(0xFF000000);
		panel->setPosition(form->getX()+2,form->getY()+28);
		panel->setSize(form->getWidth()-4,form->getHeight()-4-32);
		panel->clickable=false;
		g_engine->gui->addButton(panel);

		Button *close = new Button();
		close->setID(FORM_CLOSE);
		close->setType(FORM_CLOSE);
		close->setClass(FORM_CLOSE);
		close->setImage(default_t);
		close->setText("X");
		close->setColor_Texture(0xFF880000);
		close->setColor_Box(0xFFBBBBBB);
		close->setSize(20,20);
		close->setPosition(form->getX()+form->getWidth()-close->getWidth()-4,form->getY()+4);
		g_engine->gui->addButton(close);

		Button *textbox = new Button();		
		textbox->setClass(TEXTBOX);
		textbox->setImage(default_t);
		textbox->setColor_Texture(0xFFFFFFFF);
		textbox->setColor_Text(0xFF000000);
		textbox->setSize(panel->getWidth()-10,g_engine->e_font->size);
		textbox->setPosition(panel->getX()+5,panel->getY()+5);
		g_engine->gui->addButton(textbox);

		form->form.push_back(close);
		form->form.push_back(panel);
		form->form.push_back(textbox);

		textbox->parent = form;

		if (hide_background)
			form->form.push_back(background);
		else
			delete background;		

		close->parent=form;
		return form;

	}

	void GUI::form_fitpanel_w(Button *b)//treba posebno fitx i fity za editor texture okomito
	{
		if (b->children.empty() || b->form.empty()) return;

		//treba izbacit children ili form !!!
		int max_x = -9999;		

		//check button positions
		for (int i=0;i<b->children.size();i++)
		{
			if (b->children[i]->getX()+b->children[i]->getWidth()>max_x)
				max_x = b->children[i]->getX()+b->children[i]->getWidth()*b->children[i]->getScaleX();			
		}

		//fit panel
		b->form[1]->setWidth((max_x - b->form[1]->getX())+8);

		b->setWidth(b->form[1]->getWidth()+16);//8
		//x
		b->form[0]->setX(b->getX()+b->getWidth()-b->form[0]->getWidth()-8);
		//panel
		b->form[1]->setX(b->getX()+b->getWidth()/2-b->form[1]->getWidth()/2);

	}

	void GUI::form_fitpanel_h(Button *b)//treba posebno fitx i fity za editor texture okomito
	{
		if (b->children.empty() || b->form.empty()) return;

		//treba izbacit children ili form !!!
		int max_y = -999;

		//check button positions
		for (int i=0;i<b->children.size();i++)
		{
			if (b->children[i]->getY()+b->children[i]->getHeight()>max_y)
				max_y = b->children[i]->getY()+b->children[i]->getHeight()*b->children[i]->getScaleY();
		}

		//fit panel
		b->form[1]->setHeight(max_y - b->form[1]->getY()+2*2);

		b->setHeight(b->form[1]->getHeight()+4*2+32);
		b->form[0]->setY(b->getY()+4*2);			

	}

	void GUI::form_move(Button *b, int shift_h, int shift_v)
	{		
		b->setPosition(b->getX()+shift_h,b->getY()+shift_v);

		for (int i=0;i<b->form.size();i++)
			b->form[i]->setPosition(b->form[i]->getX()+shift_h,b->form[i]->getY()+shift_v);

		for (int i=0;i<b->children.size();i++)
			b->children[i]->setPosition(b->children[i]->getX()+shift_h,b->children[i]->getY()+shift_v);

	}

	void GUI::form_delete(Button* form)
	{
		for(int i=0;i<form->children.size();i++)
			form->children[i]->setAlive(false);

		for(int i=0;i<form->form.size();i++)
			form->form[i]->setAlive(false);

		form->setAlive(false);

	}

	void GUI::form_hidepanel(Button* form)
	{
		for(int i=0;i<form->children.size();i++)
			form->children[i]->setVisible(false);

		//skip close button
		for(int i=1;i<form->form.size();i++)
			form->form[i]->setVisible(false);

	}

	void GUI::form_unhidepanel(Button* form)
	{
		for(int i=0;i<form->children.size();i++)
			form->children[i]->setVisible(true);

		for(int i=0;i<form->form.size();i++)
			form->form[i]->setVisible(true);			

	}

	void GUI::form_close(Button *button)
	{
		if (button->parent==NULL) return;

		int shift_h = button->parent->getWidth()-button->getWidth()-2;
		int shift_v = button->parent->getHeight()-button->getHeight()-2;

		if (button->getText()=="<<")
		{
			this->form_move(button->parent,-shift_h,0);
			this->form_hidepanel(button->parent);
			button->setText(">>");
		}
		else if (button->getText()==">>")
		{
			this->form_move(button->parent,shift_h,0);
			this->form_unhidepanel(button->parent);
			button->setText("<<");
		}
		else if (button->getText()=="vv")
		{
			this->form_move(button->parent,0,shift_v);
			this->form_hidepanel(button->parent);
			button->setText("^^");
		}
		else if (button->getText()=="^^")
		{
			this->form_move(button->parent,0,-shift_v);
			this->form_unhidepanel(button->parent);
			button->setText("vv");
		}
		else
			g_engine->gui->form_delete(button->parent);


	}//close

	bool GUI::CursorInsideSprite(float dot_x, float dot_y, Sprite* a, float offsetx, float offsety)
	{
		//treba ovdje koristit tocke, i treba pazit koliko ih ima, ali treba bti ovako za curosr provjeru
		if (!a->rotate)
		{
			if (
				( (dot_x >= (a->getX()+offsetx)) && (dot_x <= (a->getX()+a->w()+offsetx)) ) &&
				( (dot_y >= (a->getY()+offsety)) && (dot_y <= (a->getY()+a->h()+offsety)) )
				)
				return true;
		}
		else
		{
			if (
				( (dot_x >= (a->getBoxCX()-a->hw()+offsetx)) && (dot_x <= (a->getBoxCX()+a->hw()+offsetx)) ) &&
				( (dot_y >= (a->getBoxCY()-a->hh()+offsety)) && (dot_y <= (a->getBoxCY()+a->hh()+offsety)) )
				)
				return true;
		}

		return false;

	}//DotInsideSprite

	void GUI::CursorCollision()
	{
		if (this->collision_order==-1)
		{
			this->ReverseCursorCollision();
			return;
		}

		Sprite *a;
		std::list<Sprite*>::iterator iter;

		for (int i=0;i<g_engine->game_sprites.size();i++)
		{
			if (g_engine->game_sprites[i]->getCollidable())
			{
				iter = g_engine->game_sprites[i]->sprites.begin();

				while (iter != g_engine->game_sprites[i]->sprites.end())
				{
					a = *iter;if (a->getAlive())
					{
						if (this->CursorInsideSprite(this->cursor->getX(),this->cursor->getY(),a,g_engine->offset_X*g_engine->game_sprites[i]->getOffsetMultiplierX(),g_engine->offset_Y*g_engine->game_sprites[i]->getOffsetMultiplierY()))
							game_CursorCollision(this->cursor,a);
					}
					iter++;
				}
			}
		}

	}//CursorCollision

	void GUI::ReverseCursorCollision()
	{

		Sprite *a;
		std::list<Sprite*>::reverse_iterator iter;

		for (int i=g_engine->game_sprites.size()-1;i>=0;i--)
		{
			//check if layer is collidable
			if (g_engine->game_sprites[i]->getCollidable())
			{
				iter = g_engine->game_sprites[i]->sprites.rbegin();

				while (iter != g_engine->game_sprites[i]->sprites.rend())
				{
					a = *iter;
					if (a->getAlive())
					{
						if (this->CursorInsideSprite(this->cursor->getX(),this->cursor->getY(),a,g_engine->offset_X*g_engine->game_sprites[i]->getOffsetMultiplierX(),g_engine->offset_Y*g_engine->game_sprites[i]->getOffsetMultiplierY()))
							game_CursorCollision(this->cursor,a);
					}
					iter++;
				}
			}
		}

	}//ReverseCursorCollision



	void GUI::messagebox_add(std::string title,std::string text,int pos)
	{
		float x=0;
		int w=350;
		int h=100;

		if (pos==100) x=0;
		else if (pos==10) x=g_engine->getScreenWidth()/2-w/2;
		else if (pos==1) x=g_engine->getScreenWidth()-w;

		Button *form = new Button();
		form->setImage(default_t);
		form->setText(title);
		form->setColor_Texture(0xFF880000);
		form->setColor_Box(0xFFBBBBBB);
		form->setPosition(x,0);
		form->setSize(w,h);
		form->clickable=false;
		form->setLifetime(5000);
		g_engine->gui->addButton(form);

		Button *panel = new Button();
		panel->setImage(default_t);
		panel->setColor_Texture(0xFFFFFFFF);
		panel->setColor_Box(0xFFBBBBBB);
		panel->setColor_Text(0xFF880000);
		panel->setPosition(form->getX()+2,form->getY()+28);
		panel->setSize(form->getWidth()-4,form->getHeight()-4-32);
		panel->clickable=false;
		panel->setText(text);
		g_engine->gui->addButton(panel);

		Button *close = new Button();
		close->setID(FORM_CLOSE);
		close->setType(FORM_CLOSE);
		close->setClass(FORM_CLOSE);
		close->setImage(default_t);
		close->setText("X");
		close->setColor_Texture(0xFFFFFFFF);
		close->setColor_Box(0xFFBBBBBB);
		close->setColor_Text(0xFF880000);
		close->setSize(20,20);
		close->setPosition(form->getX()+form->getWidth()-close->getWidth()-4,form->getY()+4);
		g_engine->gui->addButton(close);

		form->form.push_back(close);
		form->form.push_back(panel);
		close->parent=form;
	}

	///////////////
	//	DROPBOX  //
	///////////////

	Button* GUI::add_dropbox(int type, int x, int y, int w ,int h, std::vector <std::string> text)
	{
		//add display box (main button)
		Button *display_box = new Button();
		display_box->setImage(default_t);	
		display_box->setClass(DROPBOX);
		display_box->setColor_Box(0xFFBBBBBB);
		display_box->setColor_Text(0xFF000000);
		display_box->setPosition(x,y);
		display_box->setSize(w,h);
		display_box->clickable=false;
		g_engine->gui->addButton(display_box);

		//set display text
		display_box->setText(text[0]);

		//add dropbox arrow
		Button *arrow = new Button();
		arrow->setImage(default_t);
		arrow->setClass(DROPBOX_ARROW);
		arrow->setText(" vv");
		arrow->setColor_Texture(0xFF880000);
		arrow->setColor_Box(0xFFBBBBBB);
		arrow->setColor_Text(0xFF000000);
		arrow->setPosition(x+display_box->getWidth(),y);
		arrow->setSize(32,h);	
		arrow->setState(0);	
		g_engine->gui->addButton(arrow);

		//connect it to main button
		arrow->parent=display_box;
		display_box->form.push_back(arrow);

		//add options
		for(int i=0;i<text.size();i++)
		{
			Button *option = new Button();
			option->setID(i);//for option numerization
			option->setImage(default_t);
			option->setClass(DROPBOX_OPTION);
			option->setType(type);
			option->setColor_Box(0xFFBBBBBB);
			option->setColor_Text(0xFF000000);
			option->setPosition(x,y+display_box->getHeight()+i*display_box->getHeight());
			option->setSize(w,h);
			option->setText(text[i]);
			option->setVisible(false);	

			//g_engine->gui->addButton(option);//copied to the gui button list when arrow pressed

			//connect with main button				
			option->parent=display_box;
			display_box->form.push_back(option);
		}

		return display_box;

	}//add_dropbox()

	void GUI::dropbox_show(Button* dropbox,Button* arrow)
	{
		arrow->setState(1);
		arrow->setText(" ^^");

		for(int i=0;i<dropbox->form.size();i++)
		{
			//copy option buttons to the button list (to draw over buttons added later)
			if (dropbox->form[i]->getClass()==DROPBOX_OPTION)
			{
				Button *option = new Button();
				option->setGUI(-666);//treba nesto smislit umisto ovog
				option->setID(dropbox->form[i]->getID());
				option->setImage(dropbox->form[i]->getImage());
				option->setClass(dropbox->form[i]->getClass());
				option->setType(dropbox->form[i]->getType());
				option->font_png = dropbox->font_png;
				option->setColor_Box(dropbox->form[i]->getColor_Box());
				option->setColor_Text(dropbox->form[i]->getColor_Text());
				option->setPosition(
					dropbox->form[i]->getX(),
					dropbox->form[i]->getY());
				option->setSize(
					dropbox->form[i]->getWidth(),
					dropbox->form[i]->getHeight());
				option->setText(dropbox->form[i]->getText());
				option->setVisible(true);	

				//connect with main button				
				option->parent=dropbox;
				g_engine->gui->addButton(option);
			}
		}
	}

	void GUI::dropbox_hide()
	{
		std::list <Button*>::iterator i = p_buttons.begin();

		while (i != p_buttons.end())
		{
			Button *a=(*i);

			//delete temporary dropbox options (-666)
			if ( a->getClass()==DROPBOX_OPTION && a->getGUI()==-666 )
			{
				//update arrow
				for(int i=0;i<a->parent->form.size();i++)
				{
					if(a->parent->form[i]->getClass()==DROPBOX_ARROW)
					{
						a->parent->form[i]->setState(0);
						a->parent->form[i]->setText(" vv");
					}
				}
				a->setAlive(false);
			}
			i++;
		}
	}		

	Button* GUI::add_checkbox(int type, int x, int y, int w, int h, std::string text_yes, std::string text_no,Texture *texture, int state)
	{
		Button *checkbox = new Button();
		checkbox->setClass(CHECKBOX);
		checkbox->setType(type);
		if (texture==NULL) checkbox->setImage(default_t);
		else checkbox->setImage(texture);

		//yes/no
		checkbox->setState(state);			
		checkbox->text1=text_yes;
		checkbox->text2=text_no;			
		if (state==1)
		{
			checkbox->setColor_Texture(0xFF00FF00);
			checkbox->setText(text_yes);
		}
		else
		{
			checkbox->setColor_Texture(0xFFFF0000);
			checkbox->setText(text_no);
		}

		checkbox->setColor_Text(0xFF000000);
		checkbox->setPosition(x,y);
		checkbox->setSize(w,h);
		g_engine->gui->addButton(checkbox);			

		return checkbox;

	}//add_checkbox()

	Button* GUI::add_slider_h(int x,int y,int w,int h,int value,int max)
	{
		std::stringstream stream;
		stream << value;

		Button *body = new Button();
		body->setClass(SLIDER_BODY);
		body->setID(SLIDER_H);
		body->text_positioning = 'L';
		body->setState(max);
		body->setImage(this->default_t);
		body->setText(stream.str());
		body->setPosition(x,y);
		body->setSize(w,h);
		body->setColor_Texture(0xFF888888);	
		this->addButton(body);

		Button *line = new Button();
		line->setImage(this->default_t);
		line->setSize(w-40,6);		
		line->setPosition(
			body->getX()+30,
			body->getY()+body->getHeight()/2-line->getHeight()/2);
		line->setColor_Texture(0xFFAAAAAA);	
		this->addButton(line);

		Button *slider = new Button();
		slider->setClass(SLIDER_H);
		slider->setImage(this->default_t);
		slider->setSize(5,(int)line->getHeight()*2.5);
		slider->setPosition(
			line->getX()+(int)(((double)value/max)*line->getWidth()),
			line->getY()+line->getHeight()/2-slider->getHeight()/2);		
		slider->setColor_Texture(0xFFCCCCCC);
		this->addButton(slider);

		//set and connect
		line->clickable=false;
		line->setCollidable(false);
		slider->clickable=false;
		slider->setCollidable(false);

		body->form.push_back(line);
		body->form.push_back(slider);

		return body;

	}

	Button* GUI::add_slider_v(int x,int y,int w,int h,int value,int max)
	{
		std::stringstream stream;
		stream << value;

		h = (h/max)*max;

		Button *body = new Button();
		body->setClass(SLIDER_BODY);
		body->setID(SLIDER_V);
		body->setState(max);
		body->setImage(this->default_t);
		body->setText(stream.str());
		body->setPosition(x,y);
		body->setSize(w,h+20);
		body->setColor_Texture(0xFF888888);	
		this->addButton(body);

		Button *line = new Button();
		line->setImage(this->default_t);
		line->setSize(6,h);		
		line->setPosition(
			body->getX()+body->getWidth()/2-line->getWidth()/2,
			body->getY()+body->getHeight()/2-line->getHeight()/2);
		line->setColor_Texture(0xFFAAAAAA);	
		this->addButton(line);

		Button *slider = new Button();
		slider->setClass(SLIDER_H);
		slider->setImage(this->default_t);
		slider->setSize((int)line->getWidth()*2.5,h/100);
		slider->setPosition(
			line->getX()+line->getWidth()/2-slider->getWidth()/2,
			line->getY()+(int)(((double)value/100)*line->getHeight()));
		slider->setColor_Texture(0xFFCCCCCC);
		this->addButton(slider);

		//set and connect
		line->clickable=false;
		line->setCollidable(false);
		slider->clickable=false;
		slider->setCollidable(false);

		body->form.push_back(line);
		body->form.push_back(slider);

		return body;

	}

	void GUI::update_slider_h_value(Button *body,int value)
	{
		Button *line = body->form[0];
		Button *slider = body->form[1];

		std::stringstream stream;
		stream << value;		

		body->setText(stream.str());

		slider->setPosition(
			line->getX()-slider->getWidth()/2+(int)(((double)value/body->getState())*line->getWidth()),
			line->getY()+line->getHeight()/2-slider->getHeight()/2);
	}

	void GUI::update_slider_h(Button *body,int x,int y)
	{
		Button *line = body->form[0];
		Button *slider = body->form[1];

		int left_x = line->getX();
		int right_x = line->getX()+line->getWidth();

		//update value and slider
		int value = 0;

		if (x<=left_x)
		{
			x = left_x;
			value = 0;
		}
		else if (x>=right_x)
		{
			x = right_x;
			value = body->getState();
		}
		else
		{
			value = ((double)(x-left_x)/line->getWidth())*body->getState();
		}

		this->update_slider_h_value(body,value);

	}

	void GUI::update_slider_v(Button *body,int x,int y)
	{
		Button *line = body->form[0];
		Button *slider = body->form[1];

		int up = line->getY();
		int down = line->getY()+line->getHeight();

		//update value and slider
		int value = 0;

		if (y<=up)
		{
			y = up;
			value = 0;
		}
		else if (y>=down)
		{
			y = down;
			value = body->getState()-1;
		}
		else
		{
			value = (int)(((double)abs(up-y)/line->getHeight())*body->getState());//100
		}

		std::stringstream stream;
		stream << value;		

		body->setText(stream.str());

		slider->setPosition(			
			line->getX()+line->getWidth()/2-slider->getWidth()/2,
			line->getY()+(int)(value*slider->getHeight()));

		//line->getY()-slider->getHeight()/2+(int)(((double)value/100)*line->getHeight()));

	}

	Button* GUI::addSliderPanelH(int x,int y,int w,int h)
	{
		Button *button = this->form_add("SLIDER PANEL TEST H",x,y,w,h,'L',false);
		Button *slider = this->add_slider_h(
			button->getX()+button->getWidth()/2-100,
			button->form[1]->getY()+button->form[1]->getHeight()-30-2,
			200,30,0,5);	

		button->form.push_back(slider);		

		return button;

	}

	Button* GUI::addSliderPanelV(int x,int y,int w,int h)
	{
		Button *button = this->form_add("SLIDER PANEL TEST V",x,y,w,h,'L',false);
		button->form[0]->setText("O");
		button->form[0]->setType(-999);
		button->form[0]->setClass(-999);
		button->form[0]->setID(-999);
		button->form[0]->setGUI(-999);

		Button *sliderv = this->add_slider_v(			
			button->form[1]->getX()+button->form[1]->getWidth()-30-2,
			button->form[1]->getY()+button->form[1]->getHeight()/2-((int)100/5)*5-10,
			30,200,0,5);	
		sliderv->setState(5);

		button->form.push_back(sliderv);
		for(int i=0;i<sliderv->form.size();i++)
		{
			button->form.push_back(sliderv->form[i]);
		}

		return button;

	}


}//Advanced2D