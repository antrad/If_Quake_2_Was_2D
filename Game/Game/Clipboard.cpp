#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Clipboard.h"

namespace Advanced2D
{
	Clipboard::Clipboard()
	{
		this->grid = false;
		this->grid_size = 32;
		this->working_layer = 0;
		this->mouse_pressed = false;

	}//Clipboard()

	void Clipboard::Clear()
	{
		std::vector<Sprite*>::iterator iter = this->clipboard.begin();
		while (iter != this->clipboard.end())
		{
			delete (*iter);
			iter = this->clipboard.erase(iter);			
		}
		this->clipboard.clear();

		this->clipboard_relposx.clear();
		this->clipboard_relposy.clear();

		//ovo ne smi ovdje !!!
		/*this->clipboard_box_x1.clear();
		this->clipboard_box_y1.clear();
		this->clipboard_box_x2.clear();
		this->clipboard_box_y2.clear();*/

	}//Clear()

	/////////////////////
	//  SELECTION BOX  //
	/////////////////////

	void Clipboard::SelectionBox_position(int cursor_x, int cursor_y)
	{
		if(this->mouse_pressed)
		{
			this->clipboard_box_x2.clear();
			this->clipboard_box_y2.clear();for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
			{
				this->clipboard_box_x2.push_back(cursor_x - g_engine->getLayerOffsetX(i));
				this->clipboard_box_y2.push_back(cursor_y - g_engine->getLayerOffsetY(i));
				//g_engine->audio->Play("jump1");
			}
		}
		else
		{
			this->clipboard_box_x1.clear();
			this->clipboard_box_y1.clear();
			this->clipboard_box_x2.clear();
			this->clipboard_box_y2.clear();

			for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
			{
				this->clipboard_box_x1.push_back(cursor_x - g_engine->getLayerOffsetX(i));
				this->clipboard_box_y1.push_back(cursor_y - g_engine->getLayerOffsetY(i));
				this->clipboard_box_x2.push_back(cursor_x - g_engine->getLayerOffsetX(i));
				this->clipboard_box_y2.push_back(cursor_y - g_engine->getLayerOffsetY(i));
			}
			this->mouse_pressed = true;
		}

	}//SelectionBox_position()

	bool Clipboard::SelectionBox_inside(Sprite *a, int x1, int y1, int x2, int y2)
	{
		double x = a->getX() + g_engine->getLayerOffsetX(a->getLayer())*a->getOffsetMX();
		double y = a->getY() + g_engine->getLayerOffsetY(a->getLayer())*a->getOffsetMY();
		double w = a->w();
		double h = a->h();

		if ((g_engine->coll_manager->dot_dotrect(
			x,y,
			x1+g_engine->getLayerOffsetX(this->working_layer),y1+g_engine->getLayerOffsetY(this->working_layer),
			x2-x1,y2-y1))

			&&

			(g_engine->coll_manager->dot_dotrect(
			x+w,y,
			x1+g_engine->getLayerOffsetX(this->working_layer),y1+g_engine->getLayerOffsetY(this->working_layer),
			x2-x1,y2-y1))

			&&
			(g_engine->coll_manager->dot_dotrect(
			x+w,y+h,
			x1+g_engine->getLayerOffsetX(this->working_layer),y1+g_engine->getLayerOffsetY(this->working_layer),
			x2-x1,y2-y1))

			&&

			(g_engine->coll_manager->dot_dotrect(
			x,y+h,
			x1+g_engine->getLayerOffsetX(this->working_layer),y1+g_engine->getLayerOffsetY(this->working_layer),
			x2-x1,y2-y1)))
			return true;

		return false;

	}//SelectionBox_inside()

	void Clipboard::SelectionBox_draw()
	{
		//draw boxes of sprites inside selection box
		if(this->clipboard_box_x1.empty()) return;
		if(this->clipboard_box_y1.empty()) return;
		if(this->clipboard_box_x2.empty()) return;
		if(this->clipboard_box_y2.empty()) return;

		std::list<Sprite*>::iterator iter = g_engine->game_sprites[this->working_layer]->sprites.begin();
		while (iter != g_engine->game_sprites[this->working_layer]->sprites.end())
		{
			Sprite *a = *iter;
			if(this->SelectionBox_inside(a,this->getX1(this->working_layer),this->getY1(this->working_layer),this->getX2(this->working_layer),this->getY2(this->working_layer)))
				g_engine->ar_graphics->draw_box(
				a->getX(),a->getY(),
				a->w(),a->h(),
				g_engine->getLayerOffsetX(a->getLayer()),
				g_engine->getLayerOffsetY(a->getLayer()),
				0xFFFF8800);
			iter++;
		}

		//draw texture box
		g_engine->ar_graphics->draw_texture(
			g_engine->gui->default_t,
			this->getX1(this->working_layer)+g_engine->getLayerOffsetX(this->working_layer),
			this->getY1(this->working_layer)+g_engine->getLayerOffsetY(this->working_layer),
			this->getX2(this->working_layer)-this->getX1(this->working_layer),
			this->getY2(this->working_layer)-this->getY1(this->working_layer),
			0,0,
			this->getX2(this->working_layer)-this->getX1(this->working_layer),
			this->getY2(this->working_layer)-this->getY1(this->working_layer),
			1,1,
			0x33FFFF00);		

		//draw line box
		g_engine->ar_graphics->draw_box(
			this->getX1(this->working_layer),this->getY1(this->working_layer),
			this->getX2(this->working_layer)-this->getX1(this->working_layer),
			this->getY2(this->working_layer)-this->getY1(this->working_layer),
			g_engine->getLayerOffsetX(this->working_layer),g_engine->getLayerOffsetY(this->working_layer),
			0x66FFFF00);

	}//SelectionBox_draw()

	////////////
	//  COPY  //
	////////////

	void Clipboard::Copy_sprite(Sprite *a, std::string command)
	{
		this->Clear();

		//skeleton se ne kopira, ali nosilac da
		Sprite *clone = NULL;
		clone = a->Copy();
		if (clone==NULL)
		{
			g_engine->gui->messagebox_add("Error","Copy failed",10);
			g_engine->gui->setSelectedSprite(NULL);
			return;
		}
		else
		{
			this->clipboard.push_back(clone);
			this->clipboard_relposx.push_back(0);
			this->clipboard_relposy.push_back(0);
			g_engine->gui->setSelectedSprite(NULL);
			if(command=="cut") a->setAlive(false);
		}

		//g_engine->audio->Play("SOLDETH1");
	}	

	void Clipboard::Copy(std::string command, int layer, bool clear)
	{
		//no selection box
		if(this->clipboard_box_x1.empty()) return;

		if(clear) this->Clear();

		std::list<Sprite*>::iterator iter = g_engine->game_sprites[layer]->sprites.begin();
		while (iter != g_engine->game_sprites[layer]->sprites.end())
		{
			Sprite *a = *iter;
			if (a->getClass()!=-999 || a->getType()!=-999 || a->getState()!=-999)
			{
				if (this->SelectionBox_inside(a,this->getX1(layer),this->getY1(layer),this->getX2(layer),this->getY2(layer)))
				{
					Sprite *temp_sprite = new Sprite();
					temp_sprite = a->Copy();
					this->clipboard.push_back(temp_sprite);

					int rel_x = a->getX() - this->clipboard_box_x1[layer];
					this->clipboard_relposx.push_back(rel_x);
					int rel_y = a->getY() - this->clipboard_box_y1[layer];
					this->clipboard_relposy.push_back(rel_y);

					if(command=="cut")
					{
						a->setAlive(false);
						for(int i=0;i<a->skeleton.size();i++) a->skeleton[i]->setAlive(false);
						for(int i=0;i<a->attachments.size();i++) a->attachments[i]->setAlive(false);
					}
				}
			}
			iter++;
		}

		if(clear) //moga bi kopirat vise puta isto, ne brise
		{
			this->clipboard_box_x1.clear();
			this->clipboard_box_y1.clear();
			this->clipboard_box_x2.clear();
			this->clipboard_box_y2.clear();
		}

	}	

	void Clipboard::CopyAll(std::string command)
	{
		this->Clear();

		for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
		{
			this->Copy(command,i,false);
		}
	}

	void Clipboard::Paste()
	{
		int  i=0;
		std::vector<Sprite*>::iterator iter=this->clipboard.begin();
		while (iter!=this->clipboard.end())
		{
			Sprite *a = *iter;

			Sprite *b = new Sprite();
			b=a->Copy();

			if (!grid)
			{
				b->setPosition(
					g_engine->gui->cursor->getX()-g_engine->getLayerOffsetX(b->getLayer())+this->clipboard_relposx[i],
					g_engine->gui->cursor->getY()-g_engine->getLayerOffsetY(b->getLayer())+this->clipboard_relposy[i]);
			}
			else
			{
				b->setPosition(
					this->getGridX(g_engine->gui->cursor->getX()+this->clipboard_relposx[i],b->getLayer()),
					this->getGridY(g_engine->gui->cursor->getY()+this->clipboard_relposy[i],b->getLayer())
					);
			}

			g_engine->addSprite(b,b->getLayer());

			iter++;
			i++;
		}
	}	

	void Clipboard::Delete(int layer)
	{		
		std::list<Sprite*>::iterator iter = g_engine->game_sprites[layer]->sprites.begin();
		while (iter!=g_engine->game_sprites[layer]->sprites.end())
		{
			Sprite *a = *iter;
			if (a->skeleton.empty() && a->attachments.empty())
			{
				if (this->SelectionBox_inside(a,this->getX1(layer),this->getY1(layer),this->getX2(layer),this->getY2(layer)))
					a->setAlive(false);
			}
			iter++;
		}

		//srusi se
		/*this->clipboard_box_x1.clear();
		this->clipboard_box_y1.clear();
		this->clipboard_box_x2.clear();
		this->clipboard_box_y2.clear();*/

	}

	void Clipboard::DeleteAll()
	{
		for(unsigned int i=0;i<g_engine->game_sprites.size();i++) this->Delete(i);		
	}

	void Clipboard::Draw()
	{
		this->Grid_draw();

		if(g_engine->p_input->GetKeyState(DIK_LCONTROL) & 0x80)
		{
			//draw transparent entities in clipboard while paste
			if(!this->clipboard.empty() && (g_engine->p_input->GetKeyState(DIK_V) & 0x80))
			{
				int i=0;			
				std::vector<Sprite*>::iterator iter = this->clipboard.begin();
				while (iter!=this->clipboard.end())
				{
					Sprite *a = *iter;
					if(!grid)
					{
						//stavio default_t dok ne napisem za teksture
						g_engine->ar_graphics->draw_texture(
							g_engine->gui->default_t,
							g_engine->gui->cursor->getX()+this->clipboard_relposx[i],
							g_engine->gui->cursor->getY()+this->clipboard_relposy[i],
							a->w(),a->h(),
							0, 0, a->w(),a->h(),
							1,1,
							0x88FFFFFF);
					}
					else
					{
						g_engine->ar_graphics->draw_texture(
							g_engine->gui->default_t,
							this->getGridX(g_engine->gui->cursor->getX()+this->clipboard_relposx[i],
							a->getLayer())+g_engine->getLayerOffsetX(a->getLayer()),
							this->getGridY(g_engine->gui->cursor->getY()+this->clipboard_relposy[i],
							a->getLayer())+g_engine->getLayerOffsetY(a->getLayer()),
							a->w(),a->h(),
							0, 0,
							a->w(),a->h(),
							1,1,
							0x88FFFFFF);					
					}
					iter++;
					i++;
				}				
			}
			else
				this->SelectionBox_draw();//just draw the selection box
		}

		if (g_engine->gui->getSelectedSprite()!=NULL) this->Draw_parent_child(g_engine->gui->getSelectedSprite());

	}

	void Clipboard::Grid_draw()
	{
		//start screen
		g_engine->ar_graphics->draw_box(
			0,0,g_engine->getScreenWidth(),g_engine->getScreenHeight(),
			g_engine->offset_X,g_engine->offset_Y,
			0xFFFF0000);

		if (this->grid)
		{
			//vertical
			for(int i=0;i<g_engine->getScreenWidth()/this->grid_size+1;i++)
			{				
				g_engine->ar_graphics->draw_line(
					(((int)-g_engine->getLayerOffsetX(this->working_layer)/this->grid_size)*this->grid_size)+i*this->grid_size,
					-g_engine->getLayerOffsetY(this->working_layer),
					(((int)-g_engine->getLayerOffsetX(this->working_layer)/this->grid_size)*this->grid_size)+i*this->grid_size,
					-g_engine->getLayerOffsetY(this->working_layer)+g_engine->getScreenHeight(),
					g_engine->getLayerOffsetX(this->working_layer),
					g_engine->getLayerOffsetY(this->working_layer),
					0x44FF0000
					);			
			}

			//horizontal
			for(int i=0;i<g_engine->getScreenHeight()/this->grid_size+1;i++)
			{				
				g_engine->ar_graphics->draw_line(
					-g_engine->getLayerOffsetX(this->working_layer),
					(((int)-g_engine->getLayerOffsetY(this->working_layer)/this->grid_size)*this->grid_size)+i*this->grid_size,
					-g_engine->getLayerOffsetX(this->working_layer)+g_engine->getScreenWidth(),
					(((int)-g_engine->getLayerOffsetY(this->working_layer)/this->grid_size)*this->grid_size)+i*this->grid_size,
					g_engine->getLayerOffsetX(this->working_layer),
					g_engine->getLayerOffsetY(this->working_layer),
					0x44FF0000
					);
			}

			D3DCOLOR color = 0xFFFFFFFF;

			//small grid-box
			if (this->dotcollision(
				this->getGridX(g_engine->gui->cursor->getX(),this->working_layer)+this->grid_size/2,
				this->getGridY(g_engine->gui->cursor->getY(),this->working_layer)+this->grid_size/2,
				this->working_layer))
				color=0xFF00FF00;

			g_engine->ar_graphics->draw_box(
				this->getGridX(g_engine->gui->cursor->getX(),this->working_layer),
				this->getGridY(g_engine->gui->cursor->getY(),this->working_layer),
				this->grid_size,this->grid_size,
				g_engine->getLayerOffsetX(this->working_layer),
				g_engine->getLayerOffsetY(this->working_layer),
				color);
		}
	}	

	void Clipboard::Draw_parent_child(Sprite *a)
	{
		std::stringstream stream;

		//draw selected sprites box
		g_engine->ar_graphics->draw_xbox(
			a->getX(),a->getY(),
			a->w(),a->h(),
			g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()),
			0xFFFF0000);

		if (a->parent!=NULL)
		{
			std::string temp1 = "child: " + a->getName() + " " + g_engine->inttostring(a->parent_att_dot);
			g_engine->e_font->write_text(temp1.c_str(),
				a->getX()+g_engine->getLayerOffsetX(a->getLayer()),
				a->getY()+g_engine->getLayerOffsetY(a->getLayer())-20,
				g_engine->getScreenWidth(),g_engine->getScreenHeight(),
				'L',0xFF0088FF);//i am a child

			std::string temp2 = "parent: " + a->parent->getName() + " " + g_engine->inttostring(a->parent->parent_att_dot);
			g_engine->e_font->write_text(
				temp2.c_str(),
				a->parent->getX()+g_engine->getLayerOffsetX(a->getLayer()),
				a->parent->getY()+g_engine->getLayerOffsetY(a->getLayer())-20,
				g_engine->getScreenWidth(),g_engine->getScreenHeight(),
				'L',0xFF0088FF);

			g_engine->ar_graphics->draw_xbox(
				a->parent->getX(),a->parent->getY(),
				a->parent->w(),a->parent->h(),
				g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()),
				0xFF0088FF);//parent
			return;
		}

		if (a->child!=NULL)
		{
			stream.str("");
			stream<<"parent:";
			g_engine->e_font->write_text(stream.str().c_str(),
				a->getX()+g_engine->getLayerOffsetX(a->getLayer()),
				a->getY()+g_engine->getLayerOffsetY(a->getLayer())-20,
				g_engine->getScreenWidth(),g_engine->getScreenHeight(),
				'L',0xFF0088FF);//i am a parent

			stream.str("");
			stream<<"child:";
			g_engine->e_font->write_text(
				stream.str().c_str(),
				a->child->getX()+g_engine->getLayerOffsetX(a->getLayer()),
				a->child->getY()+g_engine->getLayerOffsetY(a->getLayer())-20,
				g_engine->getScreenWidth(),g_engine->getScreenHeight(),
				'L',0xFF0088FF);

			g_engine->ar_graphics->draw_xbox(
				a->child->getX(),a->child->getY(),
				a->child->w(),a->child->h(),
				g_engine->getLayerOffsetX(a->getLayer()),
				g_engine->getLayerOffsetY(a->getLayer()),
				0xFF0088FF);//child
			return;
		}
	}	

	int Clipboard::getGridX(int x, int layer)
	{
		int pos_x = x - g_engine->getLayerOffsetX(layer);			
		int add_neg = 0;
		if( pos_x < 0) add_neg = 1;//if (x<0) go one more block left

		return ((int)(pos_x)/this->grid_size)*this->grid_size - add_neg*this->grid_size;
	}

	int Clipboard::getGridY(int y, int layer)
	{
		int pos_y = y - g_engine->getLayerOffsetY(layer);			
		int add_neg = 0;
		if( pos_y < 0) add_neg = 1;//if (x<0) go one more block up

		return ((int)(pos_y)/this->grid_size)*this->grid_size - add_neg*this->grid_size;		
	}

	int Clipboard::getFrame(int x,int y,int w, int h, int column_num,int frame_offsetx,int frame_offsety)
	{
		int cx = g_engine->gui->cursor->getX();
		int cy = g_engine->gui->cursor->getY();

		int column = (cx-x)/(w+frame_offsetx);
		int row = (cy-y)/(h+frame_offsety);

		int frame = column_num*row+column;

		return frame;

	}

	int Clipboard::getFramePositionX(int x,int w,int frame_offsetx)
	{
		int cx = g_engine->gui->cursor->getX();
		int column = (cx-x)/(w+frame_offsetx);
		return x+column*(w+frame_offsetx);

	}

	int Clipboard::getFramePositionY(int y,int h,int frame_offsety)
	{
		int cy = g_engine->gui->cursor->getY();

		int row = (cy-y)/(h+frame_offsety);
		return y+row*(h+frame_offsety);
	}

	bool Clipboard::dotcollision(double x, double y, int entity_list)
	{
		std::list<Sprite*>::iterator iter = g_engine->game_sprites[entity_list]->sprites.begin();
		while (iter != g_engine->game_sprites[entity_list]->sprites.end())
		{
			Sprite *a = *iter;
			if (a->getAlive())
			{
				if ( g_engine->coll_manager->DotInsideSprite(x,y,a) ) return true;
			}
			++iter;
		}
		return false;
	}

	void Clipboard::movesprite(Sprite *a)
	{
		double oldx=a->getX();
		double oldy=a->getY();

		if (!this->grid)
		{
			a->setPosition(
				g_engine->gui->cursor->getX()-g_engine->getLayerOffsetX(a->getLayer())-g_engine->gui->cursor->rel_posx,
				g_engine->gui->cursor->getY()-g_engine->getLayerOffsetY(a->getLayer())-g_engine->gui->cursor->rel_posy
				);
		}
		else
		{
			a->setPosition(
				this->getGridX(g_engine->gui->cursor->getX()-g_engine->gui->cursor->rel_posx,a->getLayer()),
				this->getGridY(g_engine->gui->cursor->getY()-g_engine->gui->cursor->rel_posy,a->getLayer()));
		}

		double movex=a->getX()-oldx;
		double movey=a->getY()-oldy;

		for (int i=0;i<a->skeleton.size();i++) a->skeleton[i]->setPosition(
			a->skeleton[i]->getX()+movex,
			a->skeleton[i]->getY()+movey
			);
		for (int i=0;i<a->attachments.size();i++) a->attachments[i]->setPosition(
			a->attachments[i]->getX()+movex,
			a->attachments[i]->getY()+movey
			);
	}

	void Clipboard::keyPress(int key){}
	void Clipboard::keyRelease(int key){}
	void Clipboard::mousePress(int button){}
	void Clipboard::mouseRelease(int button){}
	void Clipboard::mouseWheel(int wheel){}
	void Clipboard::buttonPress(Button *button, int mousebutton){}
	void Clipboard::buttonRelease(Button *button, int mousebutton){}

}//namespace