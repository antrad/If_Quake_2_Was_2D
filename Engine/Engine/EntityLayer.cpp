/* Antonio R.*/
#include "Advanced2D.h"

namespace Advanced2D
{
	bool Sector::IsInside(float x,float y)
	{
		g_engine->count_check_insector++;
		if (g_engine->coll_manager->dot_dotrect_nocount(x,y,this->x,this->y,this->w,this->h))
			return true;
		else
			return false;
	}

	void Sector::Draw()
	{
		g_engine->ar_graphics->draw_box(this->x,this->y,this->w,this->h,g_engine->offset_X,g_engine->offset_Y,0xFFFF0000);
	}

	SpriteLayer::SpriteLayer()
	{
		this->visible = true;
		this->collidable = true;
		this->offset_multiplierx = 1;
		this->offset_multipliery = 1;
		this->shader = "#";
		this->shadow = false;
	}

	void SpriteLayer::UpdateSprites()
	{
		Sprite *a;
		std::list<Sprite*>::iterator iter=this->sprites.begin();

		while (iter != this->sprites.end())
		{
			a = *iter;
			if ( a->getAlive() ) 
			{
				//move/animate entity
				if (a->just_created == true)
				{
					a->just_created = false;
					game_spriteUpdate( a );
				}
				else
				{
					a->move();
					if (a->getAnimationControl()==ANIMATE_AUTO) a->animate();
					game_spriteUpdate(a);
				}

				if (a->getCollidable()) g_engine->collidable_counter++;

				//see if this entity will auto-expire
				if (a->getLifetime()>0)
				{
					a->setAge( a->getAge() + g_engine->game_speed*g_engine->GetDeltaFix() );
					if ( a->getAge() >= a->getLifetime()) a->setAlive(false);
				}
			}
			++iter;
		} 
	}//Updatesprites

	void SpriteLayer::BurySprites()
	{
		std::list<Sprite*>::iterator iter = this->sprites.begin();
		while (iter != this->sprites.end())
		{
			if ((*iter)->getAlive() == false || (*iter)->oneframelife == true)
			{
				game_spriteErase(*iter);
				delete (*iter);
				iter = this->sprites.erase( iter );
			}
			else
				iter++;
		}

		this->sprites_render.clear();

	}//BuryEntites

	void SpriteLayer::ClearLayer()
	{
		Sprite *a;
		std::list<Sprite*>::iterator iter = this->sprites.begin();

		while (iter != this->sprites.end())
		{
			a = *iter;
			a->setAlive(false);
			iter++;
		}

	}//BuryEntites

	bool SpriteLayer::onScreen(Sprite *a)
	{
		double x = a->getX() + g_engine->getLayerOffsetX(a->getLayer())*a->getOffsetMX();
		double y = a->getY() + g_engine->getLayerOffsetY(a->getLayer())*a->getOffsetMY();
		double w = a->w();
		double h = a->h();

		g_engine->count_check_onscreen++;
		if (g_engine->coll_manager->dot_dotrect_nocount(
			x,y,
			-200,-200,g_engine->getScreenWidth()+200,g_engine->getScreenHeight()+200))
			return true;

		g_engine->count_check_onscreen++;
		if (g_engine->coll_manager->dot_dotrect_nocount(
			x+w,y,
			-200,-200,g_engine->getScreenWidth()+200,g_engine->getScreenHeight()+200))
			return true;

		g_engine->count_check_onscreen++;
		if (g_engine->coll_manager->dot_dotrect_nocount(
			x+w,y+h,
			-200,-200,g_engine->getScreenWidth()+200,g_engine->getScreenHeight()+200))
			return true;

		g_engine->count_check_onscreen++;
		if (g_engine->coll_manager->dot_dotrect_nocount(
			x,y+h,
			-200,-200,g_engine->getScreenWidth()+200,g_engine->getScreenHeight()+200))
			return true;

		return false;
	}

	void SpriteLayer::draw(int rendertype)
	{
		if (!this->visible) return;              

		Sprite *a;
		std::list<Sprite*>::iterator iter = this->sprites.begin();

		while (iter != this->sprites.end())
		{
			a = *iter;

			if ( a->getAlive() && a->getVisible())
			{
				if(this->onScreen(a))
				{
					this->sprites_render.push_back(a);//darkness and stuff, treba pazit koliko se puta ovo poziva redosljed i ostalo

					if (rendertype==TEXTURE || rendertype==ALL)// || rendertype==SKELETON
					{						
						switch (a->getRenderType())
						{
						case NORMAL:
							g_engine->ar_graphics->draw_sprite(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
							break;

						case TILE:
							g_engine->ar_graphics->draw_sprite_tile(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
							break;

						case SCROLLER_X:
							g_engine->ar_graphics->draw_scroller_x(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
							break;

						case SCROLLER_Y:
							g_engine->ar_graphics->draw_scroller_y(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));			
							break;

						case TILESCROLLER_X:
							g_engine->ar_graphics->draw_tilescroller_x(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
							break;
						}

						//g_engine->font_manager.fonts[0]->PrintText(g_engine->inttostring(a->getState()),a->getX()+g_engine->getLayerOffsetX(a->getLayer()),a->getY()+g_engine->getLayerOffsetY(a->getLayer()),1,1,0xFFFFFFFF);
					}

					if(rendertype==VECTOR || rendertype==ALL)
					{
						/*for(int i=0;i<a->att_angle.size();i++)
						{
						g_engine->ar_graphics->draw_line(a->getX(),a->getY(),a->getAtt_posX(i),a->getAtt_posY(i),
						g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()),
						a->getColor_Box());
						}*/

						for(int i=0;i<a->att_angle.size();i++)
						{
							g_engine->ar_graphics->draw_line(
								a->getX(),
								a->getY(),
								a->getAtt_posX(i),
								a->getAtt_posY(i),
								g_engine->getLayerOffsetX(a->getLayer()),
								g_engine->getLayerOffsetY(a->getLayer()),
								0xFF00FF00);
						}

						if(a->rotate)
						{
							a->vector_rect.Draw(
								a->getX(),a->getY(),a->getRotation(),a->flip_h,
								g_engine->getLayerOffsetX(a->getLayer()),
								g_engine->getLayerOffsetY(a->getLayer()),
								a->getColor_Box());
						}
						else
							a->vector_rect.Draw(
							a->cx(),a->cy(),a->getRotation(),a->flip_h,
							g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()),
							a->getColor_Box());

						if (!a->coll_box.empty())//draw collision box
						{
							if (a->coll_box.size()==4)
							{
								g_engine->ar_graphics->draw_box(
									a->coll_box[0].getX(),
									a->coll_box[0].getY(),
									a->coll_box[1].getX()-a->coll_box[0].getX(),
									a->coll_box[3].getY()-a->coll_box[0].getY(),
									g_engine->getLayerOffsetX(a->getLayer()),
									g_engine->getLayerOffsetY(a->getLayer()),
									a->getColor_Box());
							}
							else if (a->coll_box.size()==3 || a->coll_box.size()==2)
							{
								g_engine->ar_graphics->draw_line(
									a->coll_box[0].getX(),
									a->coll_box[0].getY(),
									a->coll_box[a->coll_box.size()-1].getX(),
									a->coll_box[a->coll_box.size()-1].getY(),
									g_engine->getLayerOffsetX(a->getLayer()),
									g_engine->getLayerOffsetY(a->getLayer()),
									a->getColor_Box());
							}
							else if (a->coll_box.size()==1)
							{
								g_engine->ar_graphics->draw_box(
									a->getX(),a->getY(),
									a->w(),a->h(),
									g_engine->getLayerOffsetX(a->getLayer()),
									g_engine->getLayerOffsetY(a->getLayer()),
									a->getColor_Box());
							}						
						}//draw collision box

						if(g_engine->display_collisiondots)//draw collision dots
						{
							if (!a->coll_box.empty() && a->getCollisionMethod()!=NONE)
							{
								for (int i=0;i<a->coll_box.size();i++)
								{								
									g_engine->ar_graphics->draw_dot(
										a->coll_box[i].getX(),
										a->coll_box[i].getY(),
										g_engine->getLayerOffsetX(a->getLayer()),
										g_engine->getLayerOffsetY(a->getLayer()),
										0xFF00FF00);							
								}
							}
						}//draw collision dots

					}//VECTOR || ALL

				}//OnScreen()

			}//alive && visible

			game_spritePostRender(a);
			
			++iter;

		}//while

	}//draw

	void SpriteLayer::draw_checkmap(int rendertype)
	{
		if (!this->visible) return;              

		Sprite *a;
		std::list<Sprite*>::iterator iter = this->sprites.begin();

		while (iter != this->sprites.end())
		{
			a = *iter;

			if ( a->getAlive() && a->getVisible() && a->map_current == g_engine->map_current)
			{
				if(this->onScreen(a))
				{
					this->sprites_render.push_back(a);//darkness and stuff, treba pazit koliko se puta ovo poziva redosljed i ostalo

					if (rendertype==TEXTURE || rendertype==ALL)// || rendertype==SKELETON
					{						
						switch (a->getRenderType())
						{
						case NORMAL:
							g_engine->ar_graphics->draw_sprite(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
							break;

						case TILE:
							g_engine->ar_graphics->draw_sprite_tile(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
							break;

						case SCROLLER_X:
							g_engine->ar_graphics->draw_scroller_x(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
							break;

						case SCROLLER_Y:
							g_engine->ar_graphics->draw_scroller_y(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));			
							break;

						case TILESCROLLER_X:
							g_engine->ar_graphics->draw_tilescroller_x(
								a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
							break;
						}

						//g_engine->font_manager.fonts[0]->PrintText(g_engine->inttostring(a->getState()),a->getX()+g_engine->getLayerOffsetX(a->getLayer()),a->getY()+g_engine->getLayerOffsetY(a->getLayer()),1,1,0xFFFFFFFF);
					}

					if(rendertype==VECTOR || rendertype==ALL)
					{
						/*for(int i=0;i<a->att_angle.size();i++)
						{
						g_engine->ar_graphics->draw_line(a->getX(),a->getY(),a->getAtt_posX(i),a->getAtt_posY(i),
						g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()),
						a->getColor_Box());
						}*/

						for(int i=0;i<a->att_angle.size();i++)
						{
							g_engine->ar_graphics->draw_line(
								a->getX(),
								a->getY(),
								a->getAtt_posX(i),
								a->getAtt_posY(i),
								g_engine->getLayerOffsetX(a->getLayer()),
								g_engine->getLayerOffsetY(a->getLayer()),
								0xFF00FF00);
						}

						if(a->rotate)
						{
							a->vector_rect.Draw(
								a->getX(),a->getY(),a->getRotation(),a->flip_h,
								g_engine->getLayerOffsetX(a->getLayer()),
								g_engine->getLayerOffsetY(a->getLayer()),
								a->getColor_Box());
						}
						else
							a->vector_rect.Draw(
							a->cx(),a->cy(),a->getRotation(),a->flip_h,
							g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()),
							a->getColor_Box());

						if (!a->coll_box.empty())//draw collision box
						{
							if (a->coll_box.size()==4)
							{
								g_engine->ar_graphics->draw_box(
									a->coll_box[0].getX(),
									a->coll_box[0].getY(),
									a->coll_box[1].getX()-a->coll_box[0].getX(),
									a->coll_box[3].getY()-a->coll_box[0].getY(),
									g_engine->getLayerOffsetX(a->getLayer()),
									g_engine->getLayerOffsetY(a->getLayer()),
									a->getColor_Box());
							}
							else if (a->coll_box.size()==3 || a->coll_box.size()==2)
							{
								g_engine->ar_graphics->draw_line(
									a->coll_box[0].getX(),
									a->coll_box[0].getY(),
									a->coll_box[a->coll_box.size()-1].getX(),
									a->coll_box[a->coll_box.size()-1].getY(),
									g_engine->getLayerOffsetX(a->getLayer()),
									g_engine->getLayerOffsetY(a->getLayer()),
									a->getColor_Box());
							}
							else if (a->coll_box.size()==1)
							{
								g_engine->ar_graphics->draw_box(
									a->getX(),a->getY(),
									a->w(),a->h(),
									g_engine->getLayerOffsetX(a->getLayer()),
									g_engine->getLayerOffsetY(a->getLayer()),
									a->getColor_Box());
							}						
						}//draw collision box

						if(g_engine->display_collisiondots)//draw collision dots
						{
							if (!a->coll_box.empty() && a->getCollisionMethod()!=NONE)
							{
								for (int i=0;i<a->coll_box.size();i++)
								{								
									g_engine->ar_graphics->draw_dot(
										a->coll_box[i].getX(),
										a->coll_box[i].getY(),
										g_engine->getLayerOffsetX(a->getLayer()),
										g_engine->getLayerOffsetY(a->getLayer()),
										0xFF00FF00);							
								}
							}
						}//draw collision dots

					}//VECTOR || ALL

				}//OnScreen()

			}//alive && visible

			game_spritePostRender(a);
			
			++iter;

		}//while

	}//draw

	void SpriteLayer::DrawShadow()
	{
		std::list<Sprite*>::iterator iter = this->sprites_render.begin();
		while(iter != this->sprites_render.end())
		{
			Sprite *a = *iter;
			D3DCOLOR color = a->getColor_Texture();

			//black shadow
			a->setColor_Texture(D3DCOLOR_ARGB(255,0,0,0));//KAD NIJE 255 PROBLEM ZBOG PREKLAPANJA SJENA, ZBRAJA SE

			switch (a->getRenderType())
			{
			case NORMAL:
				g_engine->ar_graphics->draw_sprite(
					a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
				break;

			case TILE:
				g_engine->ar_graphics->draw_sprite_tile(
					a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
				break;

			case SCROLLER_X:
				g_engine->ar_graphics->draw_scroller_x(
					a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
				break;

			case SCROLLER_Y:
				g_engine->ar_graphics->draw_scroller_y(
					a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));			
				break;

			case TILESCROLLER_X:
				g_engine->ar_graphics->draw_tilescroller_x(
					a,g_engine->getLayerOffsetX(a->getLayer()),g_engine->getLayerOffsetY(a->getLayer()));
				break;
			}

			a->setColor_Texture(color);

			iter++;
		}

	}

}//Advanced2D
