#include "Advanced2D.h"

//only one type of collision box per frame
//treba napravit test ovoga svega, u slucaju pogreasak, nepostojanja tocaka...

namespace Advanced2D
{
	eng_Collision::eng_Collision()
	{
		this->notarget=false;
	}

	eng_Collision::~eng_Collision(){}

	///////////////////////	
	//     SET DOTS      //	
	///////////////////////

	void eng_Collision::setColl_center(Sprite *a)
	{
		a->setCollisionMethod(CENTER);		

		Vector3 dot;
		dot.Set(a->cx(),a->cy(),0);
		a->coll_box.push_back(dot);		

	}//setColl_center

	void eng_Collision::setColl_line2(Sprite *a)
	{
		a->setCollisionMethod(LINE);		

		//1 c 2
		Vector3 dot1;
		dot1.Set(
			a->cx()+a->hw()*cos(a->getRotation()),
			a->cy()+a->hw()*sin(a->getRotation()),
			0
			);
		a->coll_box.push_back(dot1);

		Vector3 dot2;
		dot2.Set(
			a->cx()+a->hw()*cos(a->getRotation()+3.141592654),
			a->cy()+a->hw()*sin(a->getRotation()+3.141592654),
			0
			);
		a->coll_box.push_back(dot2);		

	}//setColl_line2

	void eng_Collision::setColl_line3(Sprite *a)
	{
		a->setCollisionMethod(LINE);		

		//1 2 3
		Vector3 dot1;
		dot1.Set(
			a->getX()+(a->w())*cos(a->getRotation()),
			a->getY()+(a->w())*sin(a->getRotation()),
			0
			);
		a->coll_box.push_back(dot1);

		Vector3 dot2;
		dot2.Set(
			a->getX()+a->hw()*cos(a->getRotation()),
			a->getY()+a->hw()*sin(a->getRotation()),
			0
			);
		a->coll_box.push_back(dot2);

		Vector3 dot3;
		dot3.Set(
			a->getX(),
			a->getY(),
			0
			);
		a->coll_box.push_back(dot3);		

	}//setColl_line3

	void eng_Collision::setColl_box(Sprite *a)
	{
		a->setCollisionMethod(BOX);		

		int width = a->w();
		int height = a->h();

		//1 2
		//4 3
		Vector3 dot1;
		dot1.Set(a->getX(),a->getY(),0);
		a->coll_box.push_back(dot1);

		Vector3 dot2;
		dot2.Set(a->getX()+width,a->getY(),0);
		a->coll_box.push_back(dot2);

		Vector3 dot3;
		dot3.Set(a->getX()+width,a->getY()+height,0);
		a->coll_box.push_back(dot3);

		Vector3 dot4;
		dot4.Set(a->getX(),a->getY()+height,0);
		a->coll_box.push_back(dot4);		

	}//setColl_box

	///////////////////////	
	//   UPDATE DOTS     //	
	///////////////////////

	void eng_Collision::collbox_update(Sprite *a)
	{
		switch (a->getCollisionMethod())
		{
		case BOX:
			updateColl_box(a);
			break;
			/*case BOX_CENTER:
			break;*/
		case BOX_RXYC:
			collbox_update_RXYCbox(a);
			break;
		case LINE:
			if (a->coll_box.size()==2) updateColl_line2(a);
			else if (a->coll_box.size()==3) updateColl_line3(a);
			break;
		case CENTER:
			updateColl_center(a);
			break;		
		}

	}//updateColl_Box

	void eng_Collision::collbox_update_RXYCbox(Sprite *a)//rotating around its position
	{
		if (a->coll_box.size()!=4) return;

		float half_diagonal = 0;
		if (a->w() < a->h()) half_diagonal = a->hw();
		else half_diagonal = a->hh();

		//1 2
		//4 3
		a->coll_box[0].Set(a->getX()-half_diagonal,a->getY()-half_diagonal,0);
		a->coll_box[1].Set(a->getX()+half_diagonal,a->getY()-half_diagonal,0);
		a->coll_box[2].Set(a->getX()+half_diagonal,a->getY()+half_diagonal,0);
		a->coll_box[3].Set(a->getX()-half_diagonal,a->getY()+half_diagonal,0);

	}//updateColl_Box

	void eng_Collision::updateColl_center(Sprite *a)
	{
		if (a->coll_box.empty()) return;
		if (a->coll_box.size()!=1) return;

		//1 2
		// c
		//4 3
		a->coll_box[0].Set(a->cx(),a->cy(),0);

	}//updateColl_Box

	void eng_Collision::updateColl_line2(Sprite *a)
	{
		if (a->coll_box.empty()) return;
		if (a->coll_box.size()!=2) return;

		a->coll_box[0].Set(
			a->cx()+a->hw()*cos(a->getRotation()),
			a->cy()+a->hw()*sin(a->getRotation()),
			0
			);

		a->coll_box[1].Set(
			a->cx()+a->hw()*cos(a->getRotation()+3.141592654),
			a->cy()+a->hw()*sin(a->getRotation()+3.141592654),
			0
			);

	}//updateColl_line2

	void eng_Collision::updateColl_line3(Sprite *a)
	{
		if (a->coll_box.empty()) return;
		if (a->coll_box.size()!=3) return;

		//first dot forward

		a->coll_box[0].Set(
			a->getX()+a->w()*cos(a->getRotation()),
			a->getY()+a->w()*sin(a->getRotation()),
			0
			);

		a->coll_box[1].Set(
			a->getX()+a->hw()*cos(a->getRotation()),
			a->getY()+a->hw()*sin(a->getRotation()),
			0
			);

		a->coll_box[2].Set(
			a->getX(),
			a->getY(),
			0
			);

	}//updateColl_line3

	void eng_Collision::updateColl_box(Sprite *a)
	{
		if (a->coll_box.empty()) return;
		if (a->coll_box.size()!=4) return;

		int width = a->w();
		int height = a->h();		

		//1 2
		//4 3
		a->coll_box[0].Set(a->getX(),a->getY(),0);
		a->coll_box[1].Set(a->getX()+width,a->getY(),0);
		a->coll_box[2].Set(a->getX()+width,a->getY()+height,0);
		a->coll_box[3].Set(a->getX(),a->getY()+height,0);

	}//updateColl_Box

	//////////////////////	
	// DOT COLLISION    //	
	//////////////////////

	bool eng_Collision::dot_dotrect(float dot_x, float dot_y, float x, float y, float w, float h)
	{
		g_engine->collisioncheck_counter++;

		//izbrisan = 19.3.2012
		if (
			( (dot_x > x) && (dot_x < x+w) ) &&
			( (dot_y > y) && (dot_y < y+h) )
			)
			return true;

		return false;

	}//dot_dotrect

	bool eng_Collision::dot_dotrect_eq(float dot_x, float dot_y, float x, float y, float w, float h)
	{
		g_engine->collisioncheck_counter++;

		if (
			( (dot_x >= x) && (dot_x <= x+w) ) &&
			( (dot_y >= y) && (dot_y <= y+h) )
			)
			return true;

		return false;

	}//dot_dotrect

	bool eng_Collision::dot_dotrect_nocount(float dot_x, float dot_y, float x, float y, float w, float h)
	{
		//g_engine->collisioncheck_counter_other++;

		if (
			( (dot_x >= x) && (dot_x <= x+w) ) &&
			( (dot_y >= y) && (dot_y <= y+h) )
			)
			return true;

		return false;

	}//dot_dotrect

	bool eng_Collision::DotInsideSprite(float dot_x, float dot_y, Sprite* a)
	{
		if (dot_dotrect(dot_x, dot_y, a->getX(), a->getY(), a->w(), a->h())) return true;

		return false;

	}//DotInsideSprite

	bool eng_Collision::DotInsideSpriteBR(float dot_x, float dot_y, Sprite* a)
	{
		if (a->coll_box.size()<4) return false;

		//increase collision check counter
		g_engine->collisioncheck_counter++;

		//izbrisan = 19.3.2012
		//1 2
		//4 3               
		if (
			( (dot_x > a->coll_box[0].getX()) && (dot_x < a->coll_box[1].getX()) ) &&
			( (dot_y > a->coll_box[0].getY()) && (dot_y < a->coll_box[3].getY()) )
			)
			return true;

		return false;

	}//DotInsideSprite

	bool eng_Collision::TestCollision_dot(Sprite* a, Sprite *b)
	{
		if (a->coll_box.empty()) return false;

		//is a inside b
		for (int i=0;i<a->coll_box.size();i++)
		{
			if ( DotInsideSprite(a->coll_box[i].getX(),a->coll_box[i].getY(),b) ) return true;
		}

		return false;

	}//DotInsideSprite

	void eng_Collision::TestForCollisions_dot(Sprite *test_me, int entity_list)
	{
		if(!g_engine->save_to_map)
		{
			Sprite *a;
			std::list<Sprite*>::iterator iter = g_engine->game_sprites[entity_list]->sprites.begin();

			while (iter != g_engine->game_sprites[entity_list]->sprites.end() && test_me->getAlive())
			{
				//temporary pointer
				a = *iter;

				//is this entity in use?
				if ( a->getAlive() && a->getCollidable() && a!=test_me)
				{
					if (g_engine->coll_manager->TestCollision_dot(test_me,a))
					{
						//notify game of collision
						if (!game_spriteCollision( test_me,a )) game_spriteCollision( a,test_me );
					}
				}

				++iter;

			} //while
		}
		else
		{			
			Sprite *a;
			std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.begin();

			while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.end() && test_me->getAlive())
			{
				//temporary pointer
				a = *iter;

				//is this entity in use?
				if ( a->getAlive() && a->getCollidable() && a!=test_me)
				{
					if (g_engine->coll_manager->TestCollision_dot(test_me,a))
					{
						//notify game of collision
						if (!game_spriteCollision( test_me,a )) game_spriteCollision( a,test_me );
					}
				}

				++iter;

			} //while
		}

	}//TestForCollisions_dot

	void eng_Collision::TestForCollisions_dot(Sprite *test_me, int map, int entity_list)
	{
		if(!g_engine->save_to_map)
		{
			Sprite *a;
			std::list<Sprite*>::iterator iter = g_engine->game_sprites[entity_list]->sprites.begin();

			while (iter != g_engine->game_sprites[entity_list]->sprites.end() && test_me->getAlive())
			{
				//temporary pointer
				a = *iter;

				//is this entity in use?
				if ( a->getAlive() && a->getCollidable() && a!=test_me)
				{
					if (g_engine->coll_manager->TestCollision_dot(test_me,a))
					{
						//notify game of collision
						if (!game_spriteCollision( test_me,a )) game_spriteCollision( a,test_me );
					}
				}

				++iter;

			} //while
		}
		else
		{			
			Sprite *a;
			std::list<Sprite*>::iterator iter = g_engine->game_maps[map]->sprite_layers[entity_list]->sprites.begin();

			while (iter != g_engine->game_maps[map]->sprite_layers[entity_list]->sprites.end() && test_me->getAlive())
			{
				//temporary pointer
				a = *iter;

				//is this entity in use?
				if ( a->getAlive() && a->getCollidable() && a!=test_me)
				{
					if (g_engine->coll_manager->TestCollision_dot(test_me,a))
					{
						//notify game of collision
						if (!game_spriteCollision( test_me,a )) game_spriteCollision( a,test_me );
					}
				}

				++iter;

			} //while
		}

	}//Collision

	bool eng_Collision::TestCollision_br(Sprite *a, Sprite *b)
	{
		if (a->coll_box.size()!=4 || b->coll_box.size()!=4) return false;

		for (int i=0;i<a->coll_box.size();i++)
		{
			if ( DotInsideSpriteBR(a->coll_box[i].getX(),a->coll_box[i].getY(),b) ) return true;
		}

		for (int i=0;i<b->coll_box.size();i++)
		{
			if ( DotInsideSpriteBR(b->coll_box[i].getX(),b->coll_box[i].getY(),a) ) return true;
		}

		return false;
	}

	void eng_Collision::TestForCollisions_br(Sprite *test_me, int entity_list)
	{
		if(!g_engine->save_to_map)
		{
			Sprite *a;
			std::list<Sprite*>::iterator iter = g_engine->game_sprites[entity_list]->sprites.begin();

			while (iter != g_engine->game_sprites[entity_list]->sprites.end() && test_me->getAlive())
			{			
				a = *iter;//temporary pointer			
				if ( a->getAlive() && a->getCollidable() && a!=test_me)//is this entity in use?
				{
					if (g_engine->coll_manager->TestCollision_br(test_me,a))
					{					
						if (!game_spriteCollision( test_me,a )) game_spriteCollision( a,test_me );//notify game of collision
					}
				}
				++iter;
			} //while
		}
		else
		{
			Sprite *a;
			std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.begin();

			while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.end() && test_me->getAlive())
			{			
				a = *iter;//temporary pointer			
				if ( a->getAlive() && a->getCollidable() && a!=test_me)//is this entity in use?
				{
					if (g_engine->coll_manager->TestCollision_br(test_me,a))
					{					
						if (!game_spriteCollision( test_me,a )) game_spriteCollision( a,test_me );//notify game of collision
					}
				}
				++iter;
			} //while
		}

	}//Collision

	void eng_Collision::TestForCollisions_br(Sprite *test_me, int map, int entity_list)
	{
		if(!g_engine->save_to_map)
		{
			Sprite *a;
			std::list<Sprite*>::iterator iter = g_engine->game_sprites[entity_list]->sprites.begin();

			while (iter != g_engine->game_sprites[entity_list]->sprites.end() && test_me->getAlive())
			{			
				a = *iter;//temporary pointer			
				if ( a->getAlive() && a->getCollidable() && a!=test_me)//is this entity in use?
				{
					if (g_engine->coll_manager->TestCollision_br(test_me,a))
					{					
						if (!game_spriteCollision( test_me,a )) game_spriteCollision( a,test_me );//notify game of collision
					}
				}
				++iter;
			} //while
		}
		else
		{
			Sprite *a;
			std::list<Sprite*>::iterator iter = g_engine->game_maps[map]->sprite_layers[entity_list]->sprites.begin();

			while (iter != g_engine->game_maps[map]->sprite_layers[entity_list]->sprites.end() && test_me->getAlive())
			{			
				a = *iter;//temporary pointer			
				if ( a->getAlive() && a->getCollidable() && a!=test_me)//is this entity in use?
				{
					if (g_engine->coll_manager->TestCollision_br(test_me,a))
					{					
						if (!game_spriteCollision( test_me,a )) game_spriteCollision( a,test_me );//notify game of collision
					}
				}
				++iter;
			} //while
		}

	}//Collision

	void eng_Collision::collision_line_rectrect(Sprite *test_me, int entity_list)
	{
		Sprite *a;
		std::list<Sprite*>::iterator iter = g_engine->game_sprites[entity_list]->sprites.begin();

		while (iter != g_engine->game_sprites[entity_list]->sprites.end() && test_me->getAlive())
		{
			//temporary pointer
			a = *iter;

			//is this entity in use?
			if ( a->getAlive() && a->getCollidable() && a!=test_me && a->coll_box.size()==4 && test_me->coll_box.size()==4)
			{
				if (g_engine->coll_manager->line_collbox_rectrect(
					a->coll_box[0].getX(),a->coll_box[0].getY(),
					a->coll_box[1].getX(),a->coll_box[1].getY(),
					a->coll_box[2].getX(),a->coll_box[2].getY(),
					a->coll_box[3].getX(),a->coll_box[3].getY(),
					test_me->coll_box[0].getX(),test_me->coll_box[0].getY(),
					test_me->coll_box[1].getX(),test_me->coll_box[1].getY(),
					test_me->coll_box[2].getX(),test_me->coll_box[2].getY(),
					test_me->coll_box[3].getX(),test_me->coll_box[3].getY()))
				{
					//notify game of collision
					if (!game_spriteCollision(test_me,a)) game_spriteCollision( a,test_me );
				}
			}
			++iter;

		} //while

	}//Collision

	void eng_Collision::collision_list_line_rect(Sprite *test_me, float x1, float y1, float x2, float y2, int entity_list)
	{
		std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.begin();
		while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.end())
		{
			//temporary pointer
			Sprite *a = *iter;

			//is this entity in use?
			if (a->getAlive() && a->getCollidable() && a->coll_box.size()==4)
			{
				if (this->line_linerect(
					x1,y1,x2,y2,
					a->coll_box[0].getX(),
					a->coll_box[0].getY(),
					fabs(a->coll_box[0].getX()-a->coll_box[1].getX()),
					fabs(a->coll_box[0].getY()-a->coll_box[3].getY())))
				{
					//notify game of collision
					if (!game_spriteCollision(test_me,a)) game_spriteCollision(a,test_me );
				}
			}
			++iter;

		}//while

	}//Collision

	///////////////////////	
	//     LINE-LINE     //	
	///////////////////////

	bool eng_Collision::line_lineline(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
	{

		g_engine->lineline_counter++;

		float epsilon = 0.01;//0.001 doesn't work properly

		float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
		// If d is zero, there is no intersection
		if (d == 0) return false;

		// Get the x and y
		float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
		float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
		float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

		// Check if the x and y coordinates are within both lines
		if ( x < min(x1, x2)-epsilon ||
			x > max(x1, x2)+epsilon ||
			x < min(x3, x4)-epsilon ||
			x > max(x3, x4)+epsilon ) return false;

		if ( y < min(y1, y2)-epsilon ||
			y > max(y1, y2)+epsilon ||
			y < min(y3, y4)-epsilon ||
			y > max(y3, y4)+epsilon ) return false;

		return true;

	}//line_lineline

	bool eng_Collision::line_linerect( float x1, float y1, float xx1, float yy1, float x2, float y2, float w2, float h2 )
	{
		// A - B
		// |   |
		// D - C

		// AB
		if (line_lineline(x1,y1,xx1,yy1,x2,y2,x2+w2,y2)) return true;
		// BC
		if (line_lineline(x1,y1,xx1,yy1,x2+w2,y2,x2+w2,y2+h2)) return true;
		// CD
		if (line_lineline(x1,y1,xx1,yy1,x2+w2,y2+h2,x2,y2+h2)) return true;
		// DA
		if (line_lineline(x1,y1,xx1,yy1,x2,y2+h2,x2,y2)) return true;

		return false;

	}//line_linerect

	bool eng_Collision::line_linerectABCD( float x1, float y1, float xx1, float yy1, float x2, float y2, float w2, float h2, bool &AB, bool &BC, bool &CD, bool &DA )
	{
		// A - B
		// |   |
		// D - C
		AB = false;
		BC = false;
		CD = false;
		DA = false;

		// AB
		if (line_lineline(x1,y1,xx1,yy1,x2,y2,x2+w2,y2)) AB = true;
		// BC
		if (line_lineline(x1,y1,xx1,yy1,x2+w2,y2,x2+w2,y2+h2)) BC = true;
		// CD
		if (line_lineline(x1,y1,xx1,yy1,x2+w2,y2+h2,x2,y2+h2)) CD = true;
		// DA
		if (line_lineline(x1,y1,xx1,yy1,x2,y2+h2,x2,y2)) DA = true;

		return false;

	}//line_linerectABCD

	bool eng_Collision::line_rectrect( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2 )
	{
		//horizontal lines only with vertical ones

		// A - B     E - F
		// |   |     |   |
		// D - C     H - G

		// AB - EH
		if (line_lineline(x1,y1,x1+w1,y1,x2,y2,x2,y2+h2)) return true;
		// AB - FG
		if (line_lineline(x1,y1,x1+w1,y1,x2+w2,y2,x2+w2,y2+h2)) return true;
		// DC - EH
		if (line_lineline(x1,y1+h1,x1+w1,y1+h1,x2,y2,x2,y2+h2)) return true;
		// DC - FG
		if (line_lineline(x1,y1+h1,x1+w1,y1+h1,x2+w2,y2,x2+w2,y2+h2)) return true;


		// AD - EF
		if (line_lineline(x1,y1,x1,y1+h1,x2,y2,x2+w2,y2)) return true;
		// AD - HG
		if (line_lineline(x1,y1,x1,y1+h1,x2,y2+h2,x2+w2,y2+h2)) return true;
		// BC - EF
		if (line_lineline(x1+w1,y1,x1+w1,y1+h1,x2,y2,x2+w2,y2)) return true;
		// BC - HG
		if (line_lineline(x1+w1,y1,x1+w1,y1+h1,x2,y2+h2,x2+w2,y2+h2)) return true;

		return false;

	}//line_rectrect

	bool eng_Collision::line_collbox_rectrect(float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy,
		float ex,float ey,float fx,float fy,float gx,float gy,float hx,float hy)
	{
		//horizontal lines only with vertical ones

		// A - B     E - F
		// |   |     |   |
		// D - C     H - G

		//A
		if (
			( (ax >= ex) && (ax <= fx) ) &&
			( (ay >= ey) && (ay <= hy) )
			)
			return true;

		//B
		if (
			( (bx >= ex) && (bx <= fx) ) &&
			( (by >= ey) && (by <= hy) )
			)
			return true;

		//C
		if (
			( (cx >= ex) && (cx <= fx) ) &&
			( (cy >= ey) && (cy <= hy) )
			)
			return true;

		//D
		if (
			( (dx >= ex) && (dx <= fx) ) &&
			( (dy >= ey) && (dy <= hy) )
			)
			return true;

		// A - B     E - F
		// |   |     |   |
		// D - C     H - G


		//E
		if (
			( (ex >= ax) && (ex <= bx) ) &&
			( (ey >= ay) && (ey <= dy) )
			)
			return true;

		//F
		if (
			( (fx >= ax) && (fx <= bx) ) &&
			( (fy >= ay) && (fy <= dy) )
			)
			return true;

		//G
		if (
			( (gx >= ax) && (gx <= bx) ) &&
			( (gy >= ay) && (gy <= dy) )
			)
			return true;

		//H
		if (
			( (hx >= ax) && (hx <= bx) ) &&
			( (hy >= ay) && (hy <= dy) )
			)
			return true;

		return false;

	}//line_rectrect

	bool eng_Collision::line_collbox_linerectABCD( float x1,float y1,float x2,float y2,float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy,bool &AB, bool &BC, bool &CD, bool &DA )
	{
		// A - B
		// |   |
		// D - C
		AB = false;
		BC = false;
		CD = false;
		DA = false;

		// AB
		if (line_lineline(x1,y1,x2,y2,ax,ay,bx,by)) AB = true;
		// BC
		if (line_lineline(x1,y1,x2,y2,bx,by,cx,cy)) BC = true;
		// CD
		if (line_lineline(x1,y1,x2,y2,cx,cy,dx,dy)) CD = true;
		// DA
		if (line_lineline(x1,y1,x2,y2,dx,dy,ax,ay)) DA = true;

		return false;

	}//line_linerectABCD

	bool eng_Collision::line_linelinexy(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float &xxx,float &yyy)
	{

		g_engine->lineline_counter++;

		float epsilon = 0.01;//0.001 doesn't work properly

		float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
		// If d is zero, there is no intersection
		if (d == 0) return false;

		// Get the x and y
		float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
		float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
		float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

		// Check if the x and y coordinates are within both lines
		if ( x < min(x1, x2)-epsilon ||
			x > max(x1, x2)+epsilon ||
			x < min(x3, x4)-epsilon ||
			x > max(x3, x4)+epsilon ) return false;

		if ( y < min(y1, y2)-epsilon ||
			y > max(y1, y2)+epsilon ||
			y < min(y3, y4)-epsilon ||
			y > max(y3, y4)+epsilon ) return false;

		xxx = x;
		yyy = y;

		return true;

	}//line_lineline

	void eng_Collision::line_linerectxy( float x1, float y1, float xx1, float yy1, float x2, float y2, float w2, float h2,
		bool &AB, bool &BC, bool &CD, bool &DA,
		float &ab_x, float &ab_y, float &bc_x, float &bc_y, float &cd_x, float &cd_y, float &da_x, float &da_y)
	{
		// A - B
		// |   |
		// D - C
		AB = false;
		BC = false;
		CD = false;
		DA = false;

		// AB
		if (line_linelinexy(x1,y1,xx1,yy1,x2,y2,x2+w2,y2,ab_x,ab_y)) AB = true;
		// BC
		if (line_linelinexy(x1,y1,xx1,yy1,x2+w2,y2,x2+w2,y2+h2,bc_x,bc_y)) BC = true;
		// CD
		if (line_linelinexy(x1,y1,xx1,yy1,x2+w2,y2+h2,x2,y2+h2,cd_x,cd_y)) CD = true;
		// DA
		if (line_linelinexy(x1,y1,xx1,yy1,x2,y2+h2,x2,y2,da_x,da_y)) DA = true;



	}//line_linerectABCD

	bool eng_Collision::raycasting_all(Sprite *a, Sprite *target)
	{
		for(int i=0;i<5;i++)
		{
			a->raycast_counter = i;
			if(this->raycasting(a,target)) return true;
		}

		return false;
	}

	bool eng_Collision::raycasting(Sprite *a, Sprite *target)
	{
		if (a==NULL || target==NULL) return false;

		if(this->notarget) return false;

		// 0 - 1
		// | 2 |
		// 4 - 3		

		a->raycast_counter++;
		if(a->raycast_counter>4) a->raycast_counter = 0;

		//source	
		float ray_x1 = a->cx();
		float ray_y1 = a->cy();	
		float ray_x2,ray_y2;//target

		switch(a->raycast_counter)
		{
		case 0:
			ray_x2 = target->getX()+2;
			ray_y2 = target->getY()+2;
			break;

		case 1:
			ray_x2 = target->getX()+target->w()-4;
			ray_y2 = target->getY()+2;
			break;

		case 2:
			ray_x2 = target->cx();
			ray_y2 = target->cy();
			break;

		case 3:
			ray_x2 = target->getX()+target->w()-4;
			ray_y2 = target->getY()+target->h()-4;
			break;

		case 4:
			ray_x2 = target->getX()+2;
			ray_y2 = target->getY()+target->h()-4;
			break;
		}
		
		if(!g_engine->save_to_map)
		{
			//check if visible via this line		
			std::list<Sprite*>::iterator iter = g_engine->game_sprites[8]->sprites.begin();
			while (iter != g_engine->game_sprites[8]->sprites.end())
			{
				Sprite *temp = *iter;
				if (temp->getAlive() && temp->getCollidable())
				{
					if (g_engine->coll_manager->line_linerect(
						ray_x1, ray_y1, ray_x2, ray_y2,
						temp->getX(), temp->getY(),
						temp->w(), temp->h()))
					{					
						if(g_engine->display_lineofsight)//create line to draw
						{
							Line *temp_line = new Line(ray_x1, ray_y1, ray_x2, ray_y2, a->getLayer(), 0xFFFF0000);				
							g_engine->line_list.push_back(temp_line);
						}
						return false;//line blocked, target not visible
					}
				}
				iter++;
			}
		}
		else
		{
			//check if visible via this line		
			std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[8]->sprites.begin();
			while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[8]->sprites.end())
			{
				Sprite *temp = *iter;
				if (temp->getAlive() && temp->getCollidable())
				{
					if (g_engine->coll_manager->line_linerect(
						ray_x1, ray_y1, ray_x2, ray_y2,
						temp->getX(), temp->getY(),
						temp->w(), temp->h()))
					{					
						if(g_engine->display_lineofsight)//create line to draw
						{
							Line *temp_line = new Line(ray_x1, ray_y1, ray_x2, ray_y2, a->getLayer(), 0xFFFF0000);				
							g_engine->line_list.push_back(temp_line);
						}
						return false;//line blocked, target not visible
					}
				}
				iter++;
			}

		}

		if(g_engine->display_lineofsight)//create line to draw
		{
			Line *temp_line = new Line(ray_x1, ray_y1, ray_x2, ray_y2, a->getLayer(), 0xFF00FF00);				
			g_engine->line_list.push_back(temp_line);
		}

		return true;//line not cut, target visible

	}//raycasting()

	Sprite* eng_Collision::collision_line_getclosest(float x1, float y1, float x2, float y2, int entity_list, float &coll_x, float &coll_y)
	{
		Sprite *collider = NULL;
		float temp_collx;
		float temp_colly;
		float distance = 100000;

		std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.begin();
		while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.end())
		{
			Sprite *a = *iter;

			if (a->getAlive() && a->getCollidable() && a->coll_box.size()==4)
			{
				//check 4 sides
				// AB
				if (this->line_linelinexy(
					x1,y1,
					x2,y2,
					a->coll_box[0].getX(),a->coll_box[0].getY(),
					a->coll_box[1].getX(),a->coll_box[1].getY(),
					temp_collx,temp_colly))
				{
					float temp_distance = g_engine->math->Distance(x1,y1,temp_collx,temp_colly);
					if(temp_distance<distance)//if closer to source
					{
						distance = temp_distance;
						collider = a;
						coll_x = temp_collx;
						coll_y = temp_colly;
					}
				}

				// BC
				if (this->line_linelinexy(
					x1,y1,
					x2,y2,
					a->coll_box[1].getX(),a->coll_box[1].getY(),
					a->coll_box[2].getX(),a->coll_box[2].getY(),
					temp_collx,temp_colly))
				{
					float temp_distance = g_engine->math->Distance(x1,y1,temp_collx,temp_colly);
					if(temp_distance<distance)//if closer to source
					{
						distance = temp_distance;
						collider = a;
						coll_x = temp_collx;
						coll_y = temp_colly;
					}
				}

				// CD
				if (this->line_linelinexy(
					x1,y1,
					x2,y2,
					a->coll_box[2].getX(),a->coll_box[2].getY(),
					a->coll_box[3].getX(),a->coll_box[3].getY(),
					temp_collx,temp_colly))
				{
					float temp_distance = g_engine->math->Distance(x1,y1,temp_collx,temp_colly);
					if(temp_distance<distance)//if closer to source
					{
						distance = temp_distance;
						collider = a;
						coll_x = temp_collx;
						coll_y = temp_colly;
					}
				}

				// DA
				if (this->line_linelinexy(
					x1,y1,
					x2,y2,
					a->coll_box[3].getX(),a->coll_box[3].getY(),
					a->coll_box[0].getX(),a->coll_box[0].getY(),
					temp_collx,temp_colly))
				{
					float temp_distance = g_engine->math->Distance(x1,y1,temp_collx,temp_colly);
					if(temp_distance<distance)//if closer to source
					{
						distance = temp_distance;
						collider = a;
						coll_x = temp_collx;
						coll_y = temp_colly;
					}
				}
			}
			iter++;

		}//while

		return collider;

	}//Collision

}//Advanced2D
