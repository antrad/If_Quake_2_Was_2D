#include "Advanced2D.h"

namespace Advanced2D
{
	EditLayer::EditLayer()
	{
		this->setNULL();
	}

	EditLayer::~EditLayer(){}

	void EditLayer::setNULL()
	{
		this->main_form = NULL;

		this->visible.clear();
		this->collidable.clear();

		this->shader.clear();

		this->offset_mx.clear();
		this->offset_my.clear();

		this->clear.clear();

		this->b_red = NULL;
		this->b_green = NULL;
		this->b_blue = NULL;

		this->darkness = NULL;
	}

	void EditLayer::Init()
	{
		Button *tile = new Button();
		tile->setType(FORM_AUTOHIDE);
		tile->setImage(g_engine->gui->default_t);
		tile->setText("Ly");
		tile->setColor_Texture(0xFF888888);
		tile->setSize(32,48);
		tile->setPosition(2,78);		
		g_engine->gui->addButton(tile);

		Button *el_form = g_engine->gui->form_add(
			" EDIT LAYERS AND STUFF...",
			tile->getX()+tile->getWidth(),28,
			280,g_engine->getScreenHeight()-28,
			'L',false);

		el_form->form[0]->setText("O");
		el_form->form[0]->clickable=false;
		el_form->form[0]->setCollidable(false);
		tile->child=el_form;

		//mozda ne treba ovo
		this->main_form=el_form;

		//drawing, button positions and stuff
		el_form->form[1]->row_height = g_engine->e_font->size + 2;
		el_form->form[1]->row_x = 2;
		el_form->form[1]->row_y = 2;

		int width = 40;
		int height = g_engine->e_font->size;
		int row_distance = 2;

		int posx = el_form->getX() + 2 + 50;
		int posy = el_form->form[1]->getY() + g_engine->e_font->size + 4;

		std::ostringstream text;

		el_form->form[1]->page.push_back("                  O_x   O_y      Shader");

		//el_form stuf		
		for (int i=0;i<g_engine->game_sprites.size();i++)
		{
			text.str("");
			text << "L" << i;
			el_form->form[1]->page.push_back(text.str());
			el_form->form[1]->text_positioning = 'L';

			//VISIBLE
			Button *visible = g_engine->gui->add_checkbox(
				EDITLAYER_VISIBLE,
				posx,posy,
				width/2,height,
				" V"," V",NULL,1);

			visible->setID(i);
			this->visible.push_back(visible);
			el_form->children.push_back(visible);			

			posx += visible->getWidth() + 2;

			//COLLIDABLE
			Button *collidable = g_engine->gui->add_checkbox(
				EDITLAYER_COLLIDABLE,
				posx,posy,
				width/2,height,
				" C"," C",NULL,1);

			collidable->setID(i);
			this->collidable.push_back(collidable);
			el_form->children.push_back(collidable);			

			posx += collidable->getWidth() + 5;

			Button *offsetx = new Button();
			offsetx->setType(EDITLAYER_OFFSET_MX);
			offsetx->setClass(TEXTBOX);
			offsetx->setID(i);
			offsetx->setImage(g_engine->gui->default_t);			
			offsetx->setSize(width-10,height);
			offsetx->setColor_Texture(0xFFFFFFFF);
			offsetx->setColor_Text(0xFF000000);
			offsetx->setPosition(posx,posy);
			this->offset_mx.push_back(offsetx);
			el_form->children.push_back(offsetx);
			g_engine->gui->addButton(offsetx);

			posx += offsetx->getWidth() + 2;

			Button *offsety = new Button();
			offsety->setType(EDITLAYER_OFFSET_MY);
			offsety->setClass(TEXTBOX);
			offsety->setID(i);
			offsety->setImage(g_engine->gui->default_t);			
			offsety->setSize(width-10,height);
			offsety->setColor_Texture(0xFFFFFFFF);
			offsety->setColor_Text(0xFF000000);
			offsety->setPosition(posx,posy);
			this->offset_my.push_back(offsety);
			el_form->children.push_back(offsety);
			g_engine->gui->addButton(offsety);

			posx += offsety->getWidth() + 5;

			Button *shader = new Button();
			shader->setType(EDITLAYER_SHADER);
			shader->setClass(TEXTBOX);
			shader->setID(i);
			shader->setImage(g_engine->gui->default_t);
			shader->setColor_Texture(0xFFFFFFFF);
			shader->setColor_Text(0xFF000000);
			shader->setSize(width*3,height);
			shader->setPosition(posx,posy);
			this->shader.push_back(shader);
			el_form->children.push_back(shader);
			g_engine->gui->addButton(shader);

			posx += shader->getWidth() + 5;

			Button *clear = new Button();
			clear->setType(EDITLAYER_CLEAR);
			clear->setState(0);//0 yes/no form,1 clear
			clear->setID(i);
			clear->setImage(g_engine->gui->default_t);
			clear->setText("CLR");
			clear->setSize(width-10,height);
			clear->setColor_Texture(0xFFFF0000);
			clear->setColor_Text(0xFF000000);
			clear->setPosition(posx,posy);
			this->clear.push_back(clear);
			el_form->children.push_back(clear);
			g_engine->gui->addButton(clear);			

			//next layer(row)
			posx = el_form->getX() + 2 + 50;
			posy += height + row_distance;

		}//for()

		posx = el_form->getX() + 2 + 50;
		posy += height + row_distance;
		el_form->form[1]->page.push_back("Back.");
		el_form->form[1]->page.push_back("");
		el_form->form[1]->page.push_back("");
		el_form->form[1]->page.push_back("");
		
		//BACKGROUND COLOR
		Button *red = g_engine->gui->add_slider_h(posx,posy,200,g_engine->e_font->size,0,255);
		red->form[1]->setColor_Texture(0xFFDD0000);
		red->setType(EDITLAYER_RED);		
		this->b_red = red;
		el_form->children.push_back(red);
		for(int i=0;i<b_red->form.size();i++)
		{
			el_form->children.push_back(b_red->form[i]);
		}

		posy += height + 2*row_distance;

		Button *green = g_engine->gui->add_slider_h(posx,posy,200,g_engine->e_font->size,0,255);
		green->form[1]->setColor_Texture(0xFF00DD00);
		green->setType(EDITLAYER_GREEN);		
		this->b_green = green;
		el_form->children.push_back(green);
		for(int i=0;i<b_green->form.size();i++)
		{
			el_form->children.push_back(b_green->form[i]);
		}

		posy += height + 2*row_distance;

		Button *blue = g_engine->gui->add_slider_h(posx,posy,200,g_engine->e_font->size,0,255);
		blue->form[1]->setColor_Texture(0xFF0000DD);
		blue->setType(EDITLAYER_BLUE);
		this->b_blue = blue;
		el_form->children.push_back(blue);	
		for(int i=0;i<b_blue->form.size();i++)
		{
			el_form->children.push_back(b_blue->form[i]);
		}
		//

		//DARKNESS
		posx = el_form->getX() + 2 + 40;
		posy += 2*height + row_distance;
		el_form->form[1]->page.push_back("DARK(0-100)");

		Button *b_dark = new Button();
		b_dark->setType(EDITLAYER_DARK);
		b_dark->setClass(TEXTBOX);
		b_dark->setImage(g_engine->gui->default_t);
		b_dark->setColor_Texture(0xFFFFFFFF);
		b_dark->setColor_Text(0xFF000000);
		b_dark->setSize(200,height);
		b_dark->setPosition(posx,posy);
		this->darkness = b_dark;
		g_engine->gui->addButton(b_dark);
		el_form->children.push_back(b_dark);
		//

		for (int i=0;i<el_form->children.size();i++)
		{
			el_form->children[i]->setGUI(EDITLAYER_CLASS);
		}

		//fit panel(children based)
		g_engine->gui->form_fitpanel_w(el_form);
		//g_engine->gui->form_fitpanel_h(el_form);

		//UPDATE VISIBILITY
		el_form->setVisible(false);
		for(int i=0;i<el_form->form.size();i++)
		{
			el_form->form[i]->setVisible(false);
		}
		for(int i=0;i<el_form->children.size();i++)
		{
			el_form->children[i]->setVisible(false);
		}

	}	

	void EditLayer::Reset(int &r, int &g, int &b)
	{
		//reset offsets and shaders
		for(int i=0;i<g_engine->game_sprites.size();i++)
		{
			g_engine->game_sprites[i]->setShader("#");
			g_engine->game_sprites[i]->setOffsetMultiplierX(1);
			g_engine->game_sprites[i]->setOffsetMultiplierY(1);			
		}

		//reset map_color
		r = 0;
		g = 0;
		b = 0;
		this->b_red->setText("0");
		this->b_green->setText("0");
		this->b_blue->setText("0");
		g_engine->gui->update_slider_h_value(this->b_red,0);
		g_engine->gui->update_slider_h_value(this->b_green,0);
		g_engine->gui->update_slider_h_value(this->b_blue,0);

		//reset buttons
		for(int i=0;i<this->main_form->children.size();i++)
		{
			if (this->main_form->children[i]->getClass()==TEXTBOX)
			{
				switch(this->main_form->children[i]->getType())
				{
				case EDITLAYER_OFFSET_MX:
				case EDITLAYER_OFFSET_MY:
					this->main_form->children[i]->setText("1");
					break;

				case EDITLAYER_SHADER:
					this->main_form->children[i]->setText("#");
					break;				

				case EDITLAYER_DARK:
					this->main_form->children[i]->setText("0");
					break;
				}
			}
		}
	}

	void EditLayer::LayerData_load(std::string filename, int &r, int &g, int &b)
	{
		//just checking
		if (g_engine->getEngineState()==EDITOR && this->main_form==NULL) return;	

		//load level data
		std::ifstream filein(filename.c_str());
		if(!filein.is_open())
		{
			g_engine->gui->messagebox_add("ERROR","Opening file " + filename + " failed.",10);
			this->Reset(r,g,b);
			return;
		}

		std::string line1;		

		//read first line
		while(std::getline(filein,line1))
		{
			std::stringstream stream1(line1);
			std::string line1_string;			

			if (stream1 >> line1_string)
			{
				if (line1_string=="LAYER")
				{
					//read second line
					std::string line2;
					std::getline(filein,line2);
					std::stringstream stream2(line2);	

					int layer;
					double offsetx,offsety;
					std::string shader;

					if (stream2 >> layer >> offsetx >> offsety >> shader)
					{
						if (g_engine->getEngineState()==EDITOR)
						{
							this->shader[layer]->setText(shader);
							this->offset_mx[layer]->setText(g_engine->inttostring(offsetx));
							this->offset_my[layer]->setText(g_engine->inttostring(offsety));
						}
						g_engine->game_sprites[layer]->setShader(shader);
						g_engine->game_sprites[layer]->setOffsetMultiplierX(offsetx);
						g_engine->game_sprites[layer]->setOffsetMultiplierY(offsety);
					}
				}
				else if (line1_string=="map_color")//tri broja zaredom pa mora ovdje,treba prominit
				{
					//read second line
					std::string line2;
					std::getline(filein,line2);
					std::stringstream stream2(line2);	

					int red,green,blue;

					if (stream2 >> red >> green >> blue)
					{
						if (g_engine->getEngineState()==EDITOR)
						{
							this->b_red->setText(g_engine->inttostring(red));
							g_engine->gui->update_slider_h_value(this->b_red,red);
							this->b_green->setText(g_engine->inttostring(green));
							g_engine->gui->update_slider_h_value(this->b_green,green);
							this->b_blue->setText(g_engine->inttostring(blue));
							g_engine->gui->update_slider_h_value(this->b_blue,blue);
						}
						r = red;
						g = green;
						b = blue;
					}
				}
				else if (line1_string=="dark")
				{
					//read second line
					std::string line2;
					std::getline(filein,line2);
					std::stringstream stream2(line2);

					int dark;

					if(stream2 >> dark)
					{
						if(g_engine->getEngineState()==EDITOR)
						{
							this->darkness->setText(g_engine->inttostring(dark));							
						}
						else if(g_engine->getEngineState()==GAME)
						{
							g_engine->game_maps[g_engine->map_current]->dark = dark;
						}
					}			
				}
				else if (line1_string=="objective_p")
				{
					//read second line
					std::string line2;
					std::getline(filein,line2);
					
					g_engine->game_maps[g_engine->map_current]->objective_p = line2;
				}
				else if (line1_string=="objective_s")
				{
					//read second line
					std::string line2;
					std::getline(filein,line2);
					
					g_engine->game_maps[g_engine->map_current]->objective_s = line2;
				}

			}//if

		}//getline

		filein.close();

	}//Update_form()

	void EditLayer::LayerData_update(Button *button)
	{
		switch(button->getType())
		{
		case EDITLAYER_OFFSET_MX:
			g_engine->game_sprites[button->getID()]->setOffsetMultiplierX(g_engine->stringtoint(button->getText()));
			break;

		case EDITLAYER_OFFSET_MY:
			g_engine->game_sprites[button->getID()]->setOffsetMultiplierY(g_engine->stringtoint(button->getText()));
			break;

		case EDITLAYER_SHADER:
			g_engine->game_sprites[button->getID()]->setShader(button->getText());	
			break;		
		}		

	}//Update_layers()

	void EditLayer::ButtonPress(Button *button,int mousebutton, int &r, int &g,int &b)
	{
		switch (mousebutton)
		{
		case 0:
			switch (button->getType())
			{
			case EDITLAYER_RED:
				r = (int)g_engine->stringtoint(button->getText());
				break;

			case EDITLAYER_GREEN:
				g = (int)g_engine->stringtoint(button->getText());
				break;

			case EDITLAYER_BLUE:
				b = (int)g_engine->stringtoint(button->getText());
				break;
			}
		}
	}

	void EditLayer::ButtonRelease(Button *button,int mousebutton)
	{
		switch (mousebutton)
		{
		case 0:
			switch (button->getType())
			{
			case EDITLAYER_VISIBLE:
				if (button->getState()==1)
					g_engine->game_sprites[button->getID()]->setVisible(true);
				else
					g_engine->game_sprites[button->getID()]->setVisible(false);
				break;

			case EDITLAYER_COLLIDABLE:
				if (button->getState()==1)
					g_engine->game_sprites[button->getID()]->setCollidable(true);
				else
					g_engine->game_sprites[button->getID()]->setCollidable(false);
				break;

			case EDITLAYER_CLEAR:
				if(button->getState()==0)
				{
					Button *temp_button = g_engine->gui->form_add_yesno(
						"CLEAR LAYER","Delete all sprites in layer?",250,200);
					temp_button->form[3]->setType(EDITLAYER_CLEAR);
					temp_button->form[3]->setState(1);
					temp_button->form[3]->setID(button->getID());
				}
				else
				{
					g_engine->gui->setSelectedButton(NULL);
					g_engine->gui->setSelectedSprite(NULL);
					g_engine->game_sprites[button->getID()]->ClearLayer();
					g_engine->gui->form_delete(button->parent);
				}
				break;
			}
		}
	}

}//Advanced2D