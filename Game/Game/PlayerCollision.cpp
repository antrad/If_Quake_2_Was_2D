#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "PlayerCollision.h"

bool PlayerCollision::DotInsideSpriteBR(float dot_x, float dot_y, Sprite* a)
{
	if (a->coll_box.size()<4) return false;

	//A B
	//D C
	if (
		( (dot_x >= a->coll_box[0].getX()) && (dot_x <= a->coll_box[1].getX()) ) &&
		( (dot_y >= a->coll_box[0].getY()) && (dot_y <= a->coll_box[3].getY()) )
		)
		return true;

	return false;

}//DotInsideSprite

bool DotInsideSpriteBR_engine(float dot_x, float dot_y, Sprite* a)
{
	if (a->coll_box.size()<4) return false;

	//g_engine->collisioncheck_counter++;

	//A B
	//D C
	if (
		( (dot_x > a->coll_box[0].getX()) && (dot_x < a->coll_box[1].getX()) ) &&
		( (dot_y > a->coll_box[0].getY()) && (dot_y < a->coll_box[3].getY()) )
		)
		return true;

	return false;

}//DotInsideSprite

bool PlayerCollision::TestPlayerCollision_br(Sprite *a, Sprite *b)
{
	if (a->coll_box.size()!=4 || b->coll_box.size()!=4) return false;	

	for (int i=0;i<a->coll_box.size();i++)
	{
		g_engine->collisioncheck_counter++;
		if ( DotInsideSpriteBR(a->coll_box[i].getX(),a->coll_box[i].getY(),b) ) return true;
	}

	for (int i=0;i<b->coll_box.size();i++)
	{
		g_engine->collisioncheck_counter++;
		if ( DotInsideSpriteBR(b->coll_box[i].getX(),b->coll_box[i].getY(),a) ) return true;
	}

	return false;
}

bool PlayerCollision::setVertices_CollBox2(Sprite* a, Sprite* b, bool &A, bool &B, bool &C, bool &D,bool &A2, bool &B2, bool &C2, bool &D2)
{
	if (a->coll_box.size()!=4 || b->coll_box.size()!=4) return false;	

	if (DotInsideSpriteBR_engine(a->coll_box[0].getX(),a->coll_box[0].getY(),b)) {A=true;}
	if (DotInsideSpriteBR_engine(a->coll_box[1].getX(),a->coll_box[1].getY(),b)) {B=true;}
	if (DotInsideSpriteBR_engine(a->coll_box[2].getX(),a->coll_box[2].getY(),b)) {C=true;}
	if (DotInsideSpriteBR_engine(a->coll_box[3].getX(),a->coll_box[3].getY(),b)) {D=true;}

	if (DotInsideSpriteBR_engine(b->coll_box[0].getX(),b->coll_box[0].getY(),a)) {A2=true;}
	if (DotInsideSpriteBR_engine(b->coll_box[1].getX(),b->coll_box[1].getY(),a)) {B2=true;}
	if (DotInsideSpriteBR_engine(b->coll_box[2].getX(),b->coll_box[2].getY(),a)) {C2=true;}
	if (DotInsideSpriteBR_engine(b->coll_box[3].getX(),b->coll_box[3].getY(),a)) {D2=true;}

	return true;
}

bool PlayerCollision::setVertices_CollBox3(Sprite* a, Sprite* b, bool &A, bool &B, bool &C, bool &D,bool &A2, bool &B2, bool &C2, bool &D2)
{
	if (a->coll_box.size()!=4 || b->coll_box.size()!=4) return false;

	if (DotInsideSpriteBR(a->coll_box[0].getX(),a->coll_box[0].getY(),b)) {A=true;}
	if (DotInsideSpriteBR(a->coll_box[1].getX(),a->coll_box[1].getY(),b)) {B=true;}
	if (DotInsideSpriteBR(a->coll_box[2].getX(),a->coll_box[2].getY(),b)) {C=true;}
	if (DotInsideSpriteBR(a->coll_box[3].getX(),a->coll_box[3].getY(),b)) {D=true;}

	if (DotInsideSpriteBR(b->coll_box[0].getX(),b->coll_box[0].getY(),a)) {A2=true;}
	if (DotInsideSpriteBR(b->coll_box[1].getX(),b->coll_box[1].getY(),a)) {B2=true;}
	if (DotInsideSpriteBR(b->coll_box[2].getX(),b->coll_box[2].getY(),a)) {C2=true;}
	if (DotInsideSpriteBR(b->coll_box[3].getX(),b->coll_box[3].getY(),a)) {D2=true;}	

	return true;
}

void PlayerCollision::HitWhere_Count2(Sprite* player,Sprite* b)
{
	bool A = false;
	bool B = false;
	bool C = false;
	bool D = false;

	bool A2 = false;
	bool B2 = false;
	bool C2 = false;
	bool D2 = false;

	//player
	//A 1 B
	//4   2
	//D 3 C

	if (!setVertices_CollBox2(player,b,A,B,C,D,A2,B2,C2,D2))
	{
		g_engine->gui->messagebox_add("ERROR","setVertices_CollBox-not a rect",100);	
		return;
	}	

	this->s_A.push_back(A);
	this->s_B.push_back(B);
	this->s_C.push_back(C);
	this->s_D.push_back(D);

	this->s_A2.push_back(A2);
	this->s_B2.push_back(B2);
	this->s_C2.push_back(C2);
	this->s_D2.push_back(D2);	

}//HitWhere_Count()

bool PlayerCollision::AlreadyHit(Sprite *test_me)
{
	std::vector <Sprite*>::iterator iter = this->my_colliders.begin();

	while (iter != this->my_colliders.end())
	{
		Sprite *a = *iter;

		if (a==test_me) return true;

		iter++;
	}

	return false;

}

void PlayerCollision::PlatformPlayerCollision(Sprite *test_me, int entity_list)
{
	//if(entity_list==PARTICLES_NON) g_engine->audio->Play("jump1");

	this->my_colliders.clear();
	this->done.clear();

	this->s_A.clear();
	this->s_B.clear();
	this->s_C.clear();
	this->s_D.clear();

	this->s_A2.clear();
	this->s_B2.clear();
	this->s_C2.clear();
	this->s_D2.clear();

	this->left=false;
	this->right=false;
	this->up=false;
	this->down=false;

	this->p1_velx = test_me->getVelocityX();
	this->p1_vely = test_me->getVelocityY();	

	//find colliders
	if(entity_list==1)//zbog in lift jer nije u sectorima, promini i doli
	{
		if(g_engine->save_to_map)
		{
			for(int i=0;i<g_engine->game_maps[g_engine->map_current]->sprite_layers[1]->sprites.size();i++)
			{
				//check collisions with sprites inside sector
				std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[1]->sprites.begin();
				while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[1]->sprites.end())
				{
					Sprite *a = *iter;

					if (a->getAlive() && a->getCollidable() && a->getType()==501)//&& a->getType()==501
					{
						//test collision with sprite
						if (TestPlayerCollision_br(test_me,a))
						{
							if(!this->AlreadyHit(a))
							{
								this->my_colliders.push_back(a);
								this->done.push_back(false);
								this->HitWhere_Count2(test_me,a);

								//draw
								if(g_engine->display_lineofsight)
								{
									Line *temp_line0 = new Line(a->coll_box[0].getX(), a->coll_box[0].getY(), a->coll_box[1].getX(), a->coll_box[1].getY(), a->getLayer(), 0xFFFFFFFF);				
									g_engine->line_list.push_back(temp_line0);
									Line *temp_line1 = new Line(a->coll_box[1].getX(), a->coll_box[1].getY(), a->coll_box[2].getX(), a->coll_box[2].getY(), a->getLayer(), 0xFFFFFFFF);				
									g_engine->line_list.push_back(temp_line1);
									Line *temp_line2 = new Line(a->coll_box[2].getX(), a->coll_box[2].getY(), a->coll_box[3].getX(), a->coll_box[3].getY(), a->getLayer(), 0xFFFFFFFF);				
									g_engine->line_list.push_back(temp_line2);
									Line *temp_line3 = new Line(a->coll_box[3].getX(), a->coll_box[3].getY(), a->coll_box[0].getX(), a->coll_box[0].getY(), a->getLayer(), 0xFFFFFFFF);				
									g_engine->line_list.push_back(temp_line3);
								}
							}
						}
					}
					iter++;
				}
			}
		}
	}
	else
	{
		if(g_engine->save_to_map)
		{
			for(int i=0;i<g_engine->game_maps[g_engine->map_current]->sectors.size();i++)
			{
				//entity inside sector
				if(
					g_engine->game_maps[g_engine->map_current]->sectors[i].IsInside(test_me->coll_box[0].getX(),test_me->coll_box[0].getY())
					|| g_engine->game_maps[g_engine->map_current]->sectors[i].IsInside(test_me->coll_box[1].getX(),test_me->coll_box[1].getY())
					|| g_engine->game_maps[g_engine->map_current]->sectors[i].IsInside(test_me->coll_box[2].getX(),test_me->coll_box[2].getY())
					|| g_engine->game_maps[g_engine->map_current]->sectors[i].IsInside(test_me->coll_box[3].getX(),test_me->coll_box[3].getY())
					)
				{
					//check collisions with sprites inside sector
					std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.begin();
					while (iter != g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.end())
					{
						Sprite *a = *iter;

						if(test_me->children.empty())
						{
							if (a->getAlive() && a->getCollidable() && a!=test_me)//&& a->getType()==501
							{
								//test collision with sprite
								if (TestPlayerCollision_br(test_me,a))
								{
									if(!this->AlreadyHit(a))
									{
										this->my_colliders.push_back(a);
										this->done.push_back(false);
										this->HitWhere_Count2(test_me,a);

										//draw
										if(g_engine->display_lineofsight)
										{
											Line *temp_line0 = new Line(a->coll_box[0].getX(), a->coll_box[0].getY(), a->coll_box[1].getX(), a->coll_box[1].getY(), a->getLayer(), 0xFFFFFFFF);				
											g_engine->line_list.push_back(temp_line0);
											Line *temp_line1 = new Line(a->coll_box[1].getX(), a->coll_box[1].getY(), a->coll_box[2].getX(), a->coll_box[2].getY(), a->getLayer(), 0xFFFFFFFF);				
											g_engine->line_list.push_back(temp_line1);
											Line *temp_line2 = new Line(a->coll_box[2].getX(), a->coll_box[2].getY(), a->coll_box[3].getX(), a->coll_box[3].getY(), a->getLayer(), 0xFFFFFFFF);				
											g_engine->line_list.push_back(temp_line2);
											Line *temp_line3 = new Line(a->coll_box[3].getX(), a->coll_box[3].getY(), a->coll_box[0].getX(), a->coll_box[0].getY(), a->getLayer(), 0xFFFFFFFF);				
											g_engine->line_list.push_back(temp_line3);
										}
									}
								}
							}
						}
						else
						{
							//sudaranje s child kojieg vude za sobom
							bool my_kid = false;
							for(unsigned int i=0;i<test_me->children.size();i++)
							{
								if(test_me->children[i]==a) my_kid = true; 
							}

							if(!my_kid)
							{
								if (a->getAlive() && a->getCollidable())//&& a->getType()==501
								{
									//test collision with sprite
									if (TestPlayerCollision_br(test_me,a))
									{
										if(!this->AlreadyHit(a))
										{
											this->my_colliders.push_back(a);
											this->done.push_back(false);
											this->HitWhere_Count2(test_me,a);

											//draw
											if(g_engine->display_lineofsight)
											{
												Line *temp_line0 = new Line(a->coll_box[0].getX(), a->coll_box[0].getY(), a->coll_box[1].getX(), a->coll_box[1].getY(), a->getLayer(), 0xFFFFFFFF);				
												g_engine->line_list.push_back(temp_line0);
												Line *temp_line1 = new Line(a->coll_box[1].getX(), a->coll_box[1].getY(), a->coll_box[2].getX(), a->coll_box[2].getY(), a->getLayer(), 0xFFFFFFFF);				
												g_engine->line_list.push_back(temp_line1);
												Line *temp_line2 = new Line(a->coll_box[2].getX(), a->coll_box[2].getY(), a->coll_box[3].getX(), a->coll_box[3].getY(), a->getLayer(), 0xFFFFFFFF);				
												g_engine->line_list.push_back(temp_line2);
												Line *temp_line3 = new Line(a->coll_box[3].getX(), a->coll_box[3].getY(), a->coll_box[0].getX(), a->coll_box[0].getY(), a->getLayer(), 0xFFFFFFFF);				
												g_engine->line_list.push_back(temp_line3);
											}
										}
									}
								}

							}

						}
						iter++;
					}
				}
			}
		}		
	}
	
	if(!this->my_colliders.empty())
	{
		for(unsigned int i=0;i<this->my_colliders.size();i++)
		{
			//if(my_colliders[i]->getLayer()==PARTICLES_NON) g_engine->audio->Play("jump1");
			this->TwoVertexPlayerCollision(test_me,my_colliders[i],i);				
		}

		for(unsigned int i=0;i<this->my_colliders.size();i++)		
		{
			//ne ulazi u ovo uopce
			if(this->done[i]==false)
				Bounce(test_me,my_colliders[i],i);
		}
	}

	g_engine->coll_manager->collbox_update(test_me);	

}//PlatformPlayerCollision()

void PlayerCollision::DynamicCollision(Sprite *test_me, int entity_list)
{
	this->my_colliders.clear();
	this->done.clear();

	this->s_A.clear();
	this->s_B.clear();
	this->s_C.clear();
	this->s_D.clear();

	this->s_A2.clear();
	this->s_B2.clear();
	this->s_C2.clear();
	this->s_D2.clear();

	this->left=false;
	this->right=false;
	this->up=false;
	this->down=false;

	this->p1_velx = test_me->getVelocityX();
	this->p1_vely = test_me->getVelocityY();

	//find colliders
	if(g_engine->save_to_map)
	{
		for(int i=0;i<g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.size();i++)
		{
			//check collisions with sprites inside sector
			std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.begin();
			while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[entity_list]->sprites.end())
			{
				Sprite *a = *iter;

				if (a->getAlive() && a->getCollidable())//&& a->getType()==501
				{
					//test collision with sprite
					if (TestPlayerCollision_br(test_me,a))
					{
						if(!this->AlreadyHit(a))
						{
							this->my_colliders.push_back(a);
							this->done.push_back(false);
							this->HitWhere_Count2(test_me,a);

							//draw
							if(g_engine->display_lineofsight)
							{
								Line *temp_line0 = new Line(a->coll_box[0].getX(), a->coll_box[0].getY(), a->coll_box[1].getX(), a->coll_box[1].getY(), a->getLayer(), 0xFFFFFFFF);				
								g_engine->line_list.push_back(temp_line0);
								Line *temp_line1 = new Line(a->coll_box[1].getX(), a->coll_box[1].getY(), a->coll_box[2].getX(), a->coll_box[2].getY(), a->getLayer(), 0xFFFFFFFF);				
								g_engine->line_list.push_back(temp_line1);
								Line *temp_line2 = new Line(a->coll_box[2].getX(), a->coll_box[2].getY(), a->coll_box[3].getX(), a->coll_box[3].getY(), a->getLayer(), 0xFFFFFFFF);				
								g_engine->line_list.push_back(temp_line2);
								Line *temp_line3 = new Line(a->coll_box[3].getX(), a->coll_box[3].getY(), a->coll_box[0].getX(), a->coll_box[0].getY(), a->getLayer(), 0xFFFFFFFF);				
								g_engine->line_list.push_back(temp_line3);
							}
						}
					}
				}
				iter++;
			}
		}
	}

	if(!this->my_colliders.empty())
	{
		for(unsigned int i=0;i<this->my_colliders.size();i++)
		{
			//if(my_colliders[i]->getLayer()==PARTICLES_NON) g_engine->audio->Play("jump1");
			this->TwoVertexPlayerCollision(test_me,my_colliders[i],i);				
		}

		for(unsigned int i=0;i<this->my_colliders.size();i++)		
		{
			//ne ulazi u ovo uopce
			if(this->done[i]==false)
				Bounce(test_me,my_colliders[i],i);
		}
	}

	g_engine->coll_manager->collbox_update(test_me);	

}//PlatformPlayerCollision()

//problem klizanja po stropu i kad udje cijeli u spriteove (pretvori u veliki preko brzine ?)
bool PlayerCollision::TwoVertexPlayerCollision(Sprite *test_me,Sprite *b,int i)
{
	bool A = this->s_A[i];
	bool B = this->s_B[i];
	bool C = this->s_C[i];
	bool D = this->s_D[i];

	bool A2 = this->s_A2[i];
	bool B2 = this->s_B2[i];
	bool C2 = this->s_C2[i];
	bool D2 = this->s_D2[i];	

	//==
	if(!A && !B && !C && !D && !A2 && !B2 && !C2 && !D2)
	{
		setVertices_CollBox3(test_me,b,A,B,C,D,A2,B2,C2,D2);		

		if (A && B)
		{
			setDown_P(test_me,b,i);
			this->done[i]=true;
			return true;
		}
		else if (B && C)
		{
			setLeft_P(test_me,b,i);
			this->done[i]=true;
			return true;
		}
		else if (C && D)
		{
			setUp_P(test_me,b,i);
			this->done[i]=true;
			return true;
		}
		else if (D && A)
		{
			setRight_P(test_me,b,i);
			this->done[i]=true;
			return true;
		}		

		return false;
	}

	//5.7.2012
	if (A && B)
	{
		setDown_P(test_me,b,i);
		this->done[i]=true;
		return true;
	}
	else if (B && C)
	{
		setLeft_P(test_me,b,i);
		this->done[i]=true;
		return true;
	}
	else if (C && D)
	{
		setUp_P(test_me,b,i);
		this->done[i]=true;
		return true;
	}
	else if (D && A)
	{
		setRight_P(test_me,b,i);
		this->done[i]=true;
		return true;
	}		

	this->setUp(test_me);
	this->setLeft(test_me);
	this->setRight(test_me);
	this->setDown(test_me);

	return false;

}

void PlayerCollision::setUp(Sprite *test_me)//zapne kada nema pomaka pri setup_p
{
	double C_y=0;
	double D_y=0;
	double A2_y=0;
	double B2_y=0;

	//C i D, A2 i B2
	for(unsigned double i=0;i<this->my_colliders.size();i++)
	{
		bool C = this->s_C[i];
		bool D = this->s_D[i];

		bool A2 = this->s_A2[i];
		bool B2 = this->s_B2[i];		

		//C - B2
		if(C) C_y=fabs(test_me->coll_box[2].getY() - my_colliders[i]->coll_box[1].getY());
		//D - A2
		if(D) D_y=fabs(test_me->coll_box[3].getY() - my_colliders[i]->coll_box[0].getY());

		///A2 - D
		if(A2) A2_y=fabs(test_me->coll_box[2].getY() - my_colliders[i]->coll_box[1].getY());
		//B2 - C
		if(B2) B2_y=fabs(test_me->coll_box[2].getY() - my_colliders[i]->coll_box[1].getY());
	}

	//what's hit?
	bool CD = false;
	bool A2B2 = false;
	if(C_y==D_y && C_y!=0 && D_y!=0) CD=true;	
	if(A2_y==B2_y && A2_y!=0 && B2_y!=0) A2B2=true;

	if(CD)
	{
		for(unsigned double i=0;i<this->my_colliders.size();i++)
		{
			bool C = this->s_C[i];
			bool D = this->s_D[i];

			if(C || D)
			{			
				double tC_y=0;
				double tD_y=0;

				if(C)//C - B2
				{
					tC_y=fabs(test_me->coll_box[2].getY() - my_colliders[i]->coll_box[1].getY());
					if(tC_y==C_y)
					{
						this->setUp_P(test_me,my_colliders[i],i);
						this->done[i]=true;						
					}

				}
				else if(D)//D - A2
				{
					tD_y=fabs(test_me->coll_box[3].getY() - my_colliders[i]->coll_box[0].getY());
					if(tD_y==D_y)
					{
						this->setUp_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}					
				}				
			}
		}
	}

	if(A2B2)
	{
		for(unsigned double i=0;i<this->my_colliders.size();i++)
		{
			bool A2 = this->s_A2[i];
			bool B2 = this->s_B2[i];

			if(A2 || B2)
			{			
				double tA2_y=0;
				double tB2_y=0;

				if(A2)//C - B2
				{
					tA2_y=fabs(test_me->coll_box[2].getY() - my_colliders[i]->coll_box[1].getY());
					if(tA2_y==A2_y)
					{
						this->setUp_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
				else if(B2)//D - A2
				{
					tB2_y=fabs(test_me->coll_box[3].getY() - my_colliders[i]->coll_box[0].getY());
					if(tB2_y==B2_y)
					{
						this->setUp_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}				
			}
		}
	}	
}

void PlayerCollision::setDown(Sprite *test_me)
{
	double A_y=0;
	double B_y=0;
	double C2_y=0;
	double D2_y=0;

	//C i D, A2 i B2
	for(unsigned double i=0;i<this->my_colliders.size();i++)
	{
		bool A = this->s_A[i];
		bool B = this->s_B[i];		

		bool C2 = this->s_C2[i];
		bool D2 = this->s_D2[i];

		//A - D2
		if(A) A_y=fabs(test_me->coll_box[0].getY() - my_colliders[i]->coll_box[3].getY());
		//B - C2
		if(B) B_y=fabs(test_me->coll_box[1].getY() - my_colliders[i]->coll_box[2].getY());

		//C2 - B
		if(C2) C2_y=fabs(test_me->coll_box[1].getY() - my_colliders[i]->coll_box[2].getY());
		//D2 - A
		if(D2) D2_y=fabs(test_me->coll_box[0].getY() - my_colliders[i]->coll_box[3].getY());
	}

	bool AB = false;
	bool C2D2 = false;
	if(A_y==B_y  && A_y!=0 && B_y!=0) AB=true;
	if(C2_y==D2_y && C2_y!=0 && D2_y!=0) C2D2=true;

	if(AB)
	{
		for(unsigned double i=0;i<this->my_colliders.size();i++)
		{
			bool A = this->s_A[i];
			bool B = this->s_B[i];

			if(A || B)
			{			
				double tA_y=0;
				double tB_y=0;

				if(A)//A - D2
				{
					tA_y=fabs(test_me->coll_box[0].getY() - my_colliders[i]->coll_box[3].getY());
					if(tA_y==A_y)
					{
						this->setDown_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
				else if(B)//B - C2
				{
					tB_y=fabs(test_me->coll_box[1].getY() - my_colliders[i]->coll_box[2].getY());
					if(tB_y==B_y)
					{
						this->setDown_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
			}
		}
	}

	if(C2D2)
	{
		for(unsigned double i=0;i<this->my_colliders.size();i++)
		{
			bool C2 = this->s_C2[i];
			bool D2 = this->s_D2[i];

			if(C2 || D2)
			{			
				double tC2_y=0;
				double tD2_y=0;

				if(C2)//C2 - D
				{
					tC2_y=fabs(test_me->coll_box[1].getY() - my_colliders[i]->coll_box[2].getY());
					if(tC2_y==C2_y)
					{
						this->setDown_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
				else if(D2)//D2 - A
				{
					tD2_y=fabs(test_me->coll_box[0].getY() - my_colliders[i]->coll_box[3].getY());
					if(tD2_y==D2_y)
					{
						this->setDown_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
			}
		}
	}
}

void PlayerCollision::setLeft(Sprite *test_me)
{
	double B_x=0;
	double C_x=0;
	double A2_x=0;
	double D2_x=0;

	//C i D, A2 i B2
	for(unsigned double i=0;i<this->my_colliders.size();i++)
	{
		bool A = this->s_A[i];
		bool B = this->s_B[i];
		bool C = this->s_C[i];
		bool D = this->s_D[i];

		bool A2 = this->s_A2[i];
		bool B2 = this->s_B2[i];
		bool C2 = this->s_C2[i];
		bool D2 = this->s_D2[i];

		//B - A2
		if(B) B_x=fabs(test_me->coll_box[1].getX() - my_colliders[i]->coll_box[0].getX());
		//C - D2
		if(C) C_x=fabs(test_me->coll_box[2].getX() - my_colliders[i]->coll_box[3].getX());

		///A2 - B
		if(A2) A2_x=fabs(test_me->coll_box[1].getX() - my_colliders[i]->coll_box[0].getX());
		//D2 - C
		if(D2) D2_x=fabs(test_me->coll_box[2].getX() - my_colliders[i]->coll_box[3].getX());
	}

	bool BC = false;
	bool A2D2 = false;
	if(B_x==C_x && B_x!=0 && C_x!=0) BC=true;
	if(A2_x==D2_x && A2_x!=0 && D2_x!=0) A2D2=true;

	if(BC)
	{
		for(unsigned double i=0;i<this->my_colliders.size();i++)
		{
			bool B = this->s_B[i];
			bool C = this->s_C[i];

			if(B || C)
			{			
				double tB_x=0;
				double tC_x=0;

				if(B)//B - A2
				{
					tB_x=fabs(test_me->coll_box[1].getX() - my_colliders[i]->coll_box[0].getX());
					if(tB_x==B_x)
					{
						this->setLeft_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
				else if(C)//C - D2
				{
					tC_x=fabs(test_me->coll_box[2].getX() - my_colliders[i]->coll_box[3].getX());
					if(tC_x==C_x)
					{
						this->setLeft_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
			}
		}
	}

	if(A2D2)
	{
		for(unsigned double i=0;i<this->my_colliders.size();i++)
		{
			bool A2 = this->s_A2[i];
			bool D2 = this->s_D2[i];

			if(A2 || D2)
			{			
				double tA2_x=0;
				double tD2_x=0;

				if(A2)//A2 - B
				{
					tA2_x=fabs(test_me->coll_box[1].getX() - my_colliders[i]->coll_box[0].getX());
					if(tA2_x==A2_x)
					{
						this->setLeft_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
				else if(D2)//D2 - C
				{
					tD2_x=fabs(test_me->coll_box[2].getX() - my_colliders[i]->coll_box[3].getX());
					if(tD2_x==D2_x)
					{
						this->setLeft_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
			}
		}
	}
}

void PlayerCollision::setRight(Sprite *test_me)
{
	double A_x=0;
	double D_x=0;
	double B2_x=0;
	double C2_x=0;

	//calulate penetration
	//C i D, A2 i B2
	for(unsigned double i=0;i<this->my_colliders.size();i++)
	{
		bool A = this->s_A[i];
		bool D = this->s_D[i];

		bool B2 = this->s_B2[i];
		bool C2 = this->s_C2[i];		

		//A - B2
		if(A) A_x=fabs(test_me->coll_box[0].getX() - my_colliders[i]->coll_box[1].getX());
		//D - C2
		if(D) D_x=fabs(test_me->coll_box[3].getX() - my_colliders[i]->coll_box[2].getX());

		//B2 - A
		if(B2) B2_x=fabs(test_me->coll_box[0].getX() - my_colliders[i]->coll_box[1].getX());
		//C2 - D
		if(C2) C2_x=fabs(test_me->coll_box[3].getX() - my_colliders[i]->coll_box[2].getX());
	}

	bool AD = false;
	bool B2C2 = false;
	if(A_x==D_x && A_x!=0 && D_x!=0) AD=true;
	if(B2_x==C2_x && B2_x!=0 && C2_x!=0) B2C2=true;

	if(AD)
	{
		for(unsigned double i=0;i<this->my_colliders.size();i++)
		{
			bool A = this->s_A[i];
			bool D = this->s_D[i];

			if(A || D)
			{			
				double tA_x=0;
				double tD_x=0;

				if(A)//A - B2
				{
					tA_x=fabs(test_me->coll_box[0].getX() - my_colliders[i]->coll_box[1].getX());
					if(tA_x==A_x)
					{
						this->setRight_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
				else if(D)//D - C2
				{
					tD_x=fabs(test_me->coll_box[3].getX() - my_colliders[i]->coll_box[2].getX());
					if(tD_x==D_x)
					{
						this->setRight_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
			}
		}
	}

	if(B2C2)
	{
		for(unsigned double i=0;i<this->my_colliders.size();i++)
		{
			bool B2 = this->s_B2[i];
			bool C2 = this->s_C2[i];

			if(B2 || C2)
			{			
				double tB2_x=0;
				double tC2_x=0;

				if(B2)//B2 - A
				{
					tB2_x=fabs(test_me->coll_box[0].getX() - my_colliders[i]->coll_box[1].getX());
					if(tB2_x==B2_x)
					{
						this->setRight_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
				else if(C2)//C2 - D
				{
					tC2_x=fabs(test_me->coll_box[3].getX() - my_colliders[i]->coll_box[2].getX());
					if(tC2_x==C2_x)
					{
						this->setRight_P(test_me,my_colliders[i],i);
						this->done[i]=true;
					}
				}
			}
		}
	}
}

int PlayerCollision::Bounce_Dot(Sprite* player , Sprite* b,int i)
{
	double cx,cy;

	if (player->coll_box.size()==1)//dot
	{
		cx=player->coll_box[0].getX();
		cy=player->coll_box[0].getY();
	}
	else//box inside sprite
	{
		cx=player->cx();
		cy=player->cy();
	}

	//ne smije ovdje biti pomaka unaprid jer se provjerava svaka stranica

	//AB
	if (g_engine->coll_manager->line_lineline(
		cx,cy,
		cx-10*p1_velx,cy-10*p1_vely,
		b->coll_box[0].getX(),b->coll_box[0].getY(),
		b->coll_box[1].getX(),b->coll_box[1].getY()))
	{
		setUp_P(player,b,i);
		this->done[i]=true;
		return 300;		
	}

	//BC
	if (g_engine->coll_manager->line_lineline(
		cx,cy,
		cx-10*p1_velx,cy-10*p1_vely,
		b->coll_box[1].getX(),b->coll_box[1].getY(),
		b->coll_box[2].getX(),b->coll_box[2].getY()))
	{
		setRight_P(player,b,i);
		this->done[i]=true;
		return 400;		
	}

	//CD
	if (g_engine->coll_manager->line_lineline(
		cx,cy,
		cx-10*p1_velx,cy-10*p1_vely,
		b->coll_box[2].getX(),b->coll_box[2].getY(),
		b->coll_box[3].getX(),b->coll_box[3].getY()))
	{
		setDown_P(player,b,i);
		this->done[i]=true;
		return 100;		
	}

	//DA
	if (g_engine->coll_manager->line_lineline(
		cx,cy,
		cx-10*p1_velx,cy-10*p1_vely,
		b->coll_box[3].getX(),b->coll_box[3].getY(),
		b->coll_box[0].getX(),b->coll_box[0].getY()))
	{
		setLeft_P(player,b,i);
		this->done[i]=true;
		return 200;		
	}

	//g_engine->gui->messagebox_add("ERROR","Bounce_Dot() - no collision",100);//ovo treba vratiti
	return 0;

}

int PlayerCollision::Bounce(Sprite* player , Sprite* b,int i)
{
	//player bounces of b
	//A 1 B
	//4   2
	//D 3 C
	bool A = this->s_A[i];
	bool B = this->s_B[i];
	bool C = this->s_C[i];
	bool D = this->s_D[i];

	bool A2 = this->s_A2[i];
	bool B2 = this->s_B2[i];
	bool C2 = this->s_C2[i];
	bool D2 = this->s_D2[i];	

	if (A && B && C && D)
		if(player->coll_box.size()==1) return Bounce_Dot(player,b,i);//whole sprite inside

	if (A==true)
	{
		//dot MUST NOT go forward, backward is OK 24.9.
		if(g_engine->display_lineofsight)
		{
			Line *temp_line0 = new Line(player->coll_box[0].getX(),player->coll_box[0].getY(),
				player->coll_box[0].getX()-1000*p1_velx,player->coll_box[0].getY()-1000*p1_vely, b->getLayer(), 0xFFFF0000);
			g_engine->line_list.push_back(temp_line0);
		}

		if (g_engine->coll_manager->line_lineline(
			player->coll_box[0].getX(),player->coll_box[0].getY(),
			player->coll_box[0].getX()-1000*p1_velx,player->coll_box[0].getY()-1000*p1_vely,	
			b->coll_box[2].getX(),b->coll_box[2].getY(),
			b->coll_box[3].getX(),b->coll_box[3].getY()))
		{
			setDown_P(player,b,i);
			//g_engine->audio->Play("BLASTF1A");
			this->done[i]=true;
			return 100;
		}
		else
		{			
			setRight_P(player,b,i);
			this->done[i]=true;
			return 400;
		}
	}
	else if (B==true)
	{	
		if(g_engine->display_lineofsight)
		{
			Line *temp_line0 = new Line(player->coll_box[1].getX(),player->coll_box[1].getY(),
				player->coll_box[1].getX()-1000*p1_velx,player->coll_box[1].getY()-1000*p1_vely, b->getLayer(), 0xFFFF0000);				
			g_engine->line_list.push_back(temp_line0);
		}

		if (g_engine->coll_manager->line_lineline(
			player->coll_box[1].getX(),player->coll_box[1].getY(),
			player->coll_box[1].getX()-1000*p1_velx,player->coll_box[1].getY()-1000*p1_vely,	
			b->coll_box[2].getX(),b->coll_box[2].getY(),
			b->coll_box[3].getX(),b->coll_box[3].getY()))
		{
			setDown_P(player,b,i);
			//g_engine->audio->Play("BLASTF1A");
			this->done[i]=true;
			return 100;
		}
		else
		{
			setLeft_P(player,b,i);
			this->done[i]=true;
			return 200;
		}
	}
	else if (C==true)
	{
		if(g_engine->display_lineofsight)
		{
			Line *temp_line0 = new Line(player->coll_box[2].getX(),player->coll_box[2].getY(),
				player->coll_box[2].getX()-1000*p1_velx,player->coll_box[2].getY()-1000*p1_vely, b->getLayer(), 0xFFFF0000);				
			g_engine->line_list.push_back(temp_line0);
		}

		if (g_engine->coll_manager->line_lineline(
			player->coll_box[2].getX(),player->coll_box[2].getY(),
			player->coll_box[2].getX()-1000*p1_velx,player->coll_box[2].getY()-1000*p1_vely,	
			b->coll_box[0].getX(),b->coll_box[0].getY(),
			b->coll_box[1].getX(),b->coll_box[1].getY()))
		{
			setUp_P(player,b,i);
			this->done[i]=true;
			//g_engine->audio->Play("BLASTF1A");
			return 300;
		}
		else
		{
			setLeft_P(player,b,i);
			//g_engine->audio->Play("Bfg__f1y");
			this->done[i]=true;
			return 200;
		}
	}
	else if (D==true)
	{
		if(g_engine->display_lineofsight)
		{
			Line *temp_line0 = new Line(player->coll_box[3].getX(),player->coll_box[3].getY(),
				player->coll_box[3].getX()-1000*p1_velx,player->coll_box[3].getY()-1000*p1_vely, b->getLayer(), 0xFFFF0000);				
			g_engine->line_list.push_back(temp_line0);
		}

		if (g_engine->coll_manager->line_lineline(
			player->coll_box[3].getX(),player->coll_box[3].getY(),
			player->coll_box[3].getX()-1000*p1_velx,player->coll_box[3].getY()-1000*p1_vely,	
			b->coll_box[0].getX(),b->coll_box[0].getY(),
			b->coll_box[1].getX(),b->coll_box[1].getY()))
		{
			setUp_P(player,b,i);
			this->done[i]=true;
			//g_engine->audio->Play("BLASTF1A");
			return 300;
		}
		else
		{
			setRight_P(player,b,i);
			this->done[i]=true;
			return 400;
		}
	}//ne znam jel ovo uopce triba, set down je bilo setup, nisam uopce provjerava jel u redu
	else if (A2==true)
	{
		if(g_engine->display_lineofsight)
		{
			Line *temp_line0 = new Line(b->coll_box[0].getX(),b->coll_box[0].getY(),
				b->coll_box[0].getX()+1000*p1_velx,b->coll_box[0].getY()+1000*p1_vely, b->getLayer(), 0xFFFF0000);				
			g_engine->line_list.push_back(temp_line0);
		}

		if (g_engine->coll_manager->line_lineline(
			b->coll_box[0].getX(),b->coll_box[0].getY(),
			b->coll_box[0].getX()+1000*p1_velx,b->coll_box[0].getY()+1000*p1_vely,	
			player->coll_box[2].getX(),player->coll_box[2].getY(),
			player->coll_box[3].getX(),player->coll_box[3].getY()))
		{			
			setUp_P(player,b,i);
			//g_engine->audio->Play("BLASTF1A");
			this->done[i]=true;
			return 100;
		}
		else
		{			
			setLeft_P(player,b,i);
			this->done[i]=true;
			return 400;
		}
	}
	else if (B2==true)
	{		
		if(g_engine->display_lineofsight)
		{
			Line *temp_line0 = new Line(b->coll_box[1].getX(),b->coll_box[1].getY(),
				b->coll_box[1].getX()+1000*p1_velx,b->coll_box[1].getY()+1000*p1_vely, b->getLayer(), 0xFFFF0000);				
			g_engine->line_list.push_back(temp_line0);
		}

		if (g_engine->coll_manager->line_lineline(
			b->coll_box[1].getX(),b->coll_box[1].getY(),
			b->coll_box[1].getX()+1000*p1_velx,b->coll_box[1].getY()+1000*p1_vely,	
			player->coll_box[2].getX(),player->coll_box[2].getY(),
			player->coll_box[3].getX(),player->coll_box[3].getY()))
		{
			setUp_P(player,b,i);
			//g_engine->audio->Play("BLASTF1A");
			this->done[i]=true;
			return 100;
		}
		else
		{
			setRight_P(player,b,i);
			this->done[i]=true;
			return 200;
		}
	}
	else if (C2==true)
	{
		if(g_engine->display_lineofsight)
		{
			Line *temp_line0 = new Line(b->coll_box[2].getX(),b->coll_box[2].getY(),
				b->coll_box[2].getX()-1000*p1_velx,b->coll_box[2].getY()-1000*p1_vely, b->getLayer(), 0xFFFF0000);				
			g_engine->line_list.push_back(temp_line0);
		}

		if (g_engine->coll_manager->line_lineline(
			b->coll_box[2].getX(),b->coll_box[2].getY(),
			b->coll_box[2].getX()-1000*p1_velx,b->coll_box[2].getY()-1000*p1_vely,	
			player->coll_box[0].getX(),player->coll_box[0].getY(),
			player->coll_box[1].getX(),player->coll_box[1].getY()))
		{
			setDown_P(player,b,i);
			//g_engine->audio->Play("BLASTF1A");
			this->done[i]=true;
			return 300;
		}
		else
		{
			setLeft_P(player,b,i);
			this->done[i]=true;
			return 200;
		}
	}
	else if (D2==true)
	{
		if(g_engine->display_lineofsight)
		{
			Line *temp_line0 = new Line(b->coll_box[3].getX(),b->coll_box[3].getY(),
				b->coll_box[3].getX()-1000*p1_velx,b->coll_box[3].getY()-1000*p1_vely, b->getLayer(), 0xFFFF0000);				
			g_engine->line_list.push_back(temp_line0);
		}

		if (g_engine->coll_manager->line_lineline(
			b->coll_box[3].getX(),b->coll_box[3].getY(),
			b->coll_box[3].getX()-1000*p1_velx,b->coll_box[3].getY()-1000*p1_vely,	
			player->coll_box[0].getX(),player->coll_box[0].getY(),
			player->coll_box[1].getX(),player->coll_box[1].getY()))
		{
			setDown_P(player,b,i);
			this->done[i]=true;
			return 300;
		}
		else
		{
			setRight_P(player,b,i);
			this->done[i]=true;
			return 400;
		}
	}	

	return 1234;
}

void PlayerCollision::setLeft_P(Sprite *player, Sprite *b,int i)
{
	if(this->left==false) this->left=true;
	else return;

	//draw
	if(g_engine->display_lineofsight)
	{
		Line *temp_line0 = new Line(b->coll_box[0].getX(), b->coll_box[0].getY(), b->coll_box[1].getX(), b->coll_box[1].getY(), b->getLayer(), 0xFF00FF00);				
		g_engine->line_list.push_back(temp_line0);
		Line *temp_line1 = new Line(b->coll_box[1].getX(), b->coll_box[1].getY(), b->coll_box[2].getX(), b->coll_box[2].getY(), b->getLayer(), 0xFF00FF00);				
		g_engine->line_list.push_back(temp_line1);
		Line *temp_line2 = new Line(b->coll_box[2].getX(), b->coll_box[2].getY(), b->coll_box[3].getX(), b->coll_box[3].getY(), b->getLayer(), 0xFF00FF00);				
		g_engine->line_list.push_back(temp_line2);
		Line *temp_line3 = new Line(b->coll_box[3].getX(), b->coll_box[3].getY(), b->coll_box[0].getX(), b->coll_box[0].getY(), b->getLayer(), 0xFF00FF00);				
		g_engine->line_list.push_back(temp_line3);
	}

	if(player->mass > b->mass && player->getType()==IGRAC_1)
	{
		this->setRight_P(b,player,i);

		g_engine->coll_manager->updateColl_box(b);
		g_engine->coll_manager->updateColl_box(player);

		b->setVelocityX(player->getVelocityX());
		PlayerCollision *player_collision = new PlayerCollision();	
		player_collision->PlatformPlayerCollision(b,b->getLayer());
		delete player_collision;

		g_engine->coll_manager->updateColl_box(b);
		g_engine->coll_manager->updateColl_box(player);

		b->mass = 100;
		PlayerCollision *player_collision2 = new PlayerCollision();	
		player_collision2->PlatformPlayerCollision(player,b->getLayer());
		delete player_collision2;
		b->mass = 50;

		g_engine->coll_manager->updateColl_box(b);
		g_engine->coll_manager->updateColl_box(player);

		return;
	}

	if(player->getType()!=IGRAC_1 && player->getClass()!=ENEMY)//inace skace :)
	{
		setLeft_B(player,b);
		return;
	}

	player->setVelocityX(0);
	player->setX(b->getX()-player->w());

	if(!IHitAPlatform(player,b,200)) IHitAPlatform(b,player,200);
}

void PlayerCollision::setRight_P(Sprite *player, Sprite *b,int i)
{
	if(this->right==false) this->right=true;
	else return;

	//draw
	if(g_engine->display_lineofsight)
	{
		Line *temp_line0 = new Line(b->coll_box[0].getX(), b->coll_box[0].getY(), b->coll_box[1].getX(), b->coll_box[1].getY(), b->getLayer(), 0xFF0000FF);				
		g_engine->line_list.push_back(temp_line0);
		Line *temp_line1 = new Line(b->coll_box[1].getX(), b->coll_box[1].getY(), b->coll_box[2].getX(), b->coll_box[2].getY(), b->getLayer(), 0xFF0000FF);				
		g_engine->line_list.push_back(temp_line1);
		Line *temp_line2 = new Line(b->coll_box[2].getX(), b->coll_box[2].getY(), b->coll_box[3].getX(), b->coll_box[3].getY(), b->getLayer(), 0xFF0000FF);				
		g_engine->line_list.push_back(temp_line2);
		Line *temp_line3 = new Line(b->coll_box[3].getX(), b->coll_box[3].getY(), b->coll_box[0].getX(), b->coll_box[0].getY(), b->getLayer(), 0xFF0000FF);				
		g_engine->line_list.push_back(temp_line3);
	}

	if(player->mass > b->mass && player->getType()==IGRAC_1)
	{
		this->setLeft_P(b,player,i);

		g_engine->coll_manager->updateColl_box(b);
		g_engine->coll_manager->updateColl_box(player);

		b->setVelocityX(player->getVelocityX());
		PlayerCollision *player_collision = new PlayerCollision();	
		player_collision->PlatformPlayerCollision(b,b->getLayer());
		delete player_collision;

		g_engine->coll_manager->updateColl_box(b);
		g_engine->coll_manager->updateColl_box(player);

		b->mass = 100;
		PlayerCollision *player_collision2 = new PlayerCollision();	
		player_collision2->PlatformPlayerCollision(player,b->getLayer());
		delete player_collision2;
		b->mass = 50;
		
		g_engine->coll_manager->updateColl_box(b);
		g_engine->coll_manager->updateColl_box(player);

		return;
	}

	if(player->getType()!=IGRAC_1 && player->getClass()!=ENEMY)
	{
		setRight_B(player,b);		
		return;
	}

	player->setVelocityX(0);
	player->setX(b->getX()+b->w());	

	if(!IHitAPlatform(player,b,400)) IHitAPlatform(b,player,400);

}

void PlayerCollision::setUp_P(Sprite *player, Sprite *b,int i)
{
	if(this->up==false) this->up=true;
	else return;

	//draw
	if(g_engine->display_lineofsight)
	{
		Line *temp_line0 = new Line(b->coll_box[0].getX(), b->coll_box[0].getY(), b->coll_box[1].getX(), b->coll_box[1].getY(), b->getLayer(), 0xFFFF0000);				
		g_engine->line_list.push_back(temp_line0);
		Line *temp_line1 = new Line(b->coll_box[1].getX(), b->coll_box[1].getY(), b->coll_box[2].getX(), b->coll_box[2].getY(), b->getLayer(), 0xFFFF0000);				
		g_engine->line_list.push_back(temp_line1);
		Line *temp_line2 = new Line(b->coll_box[2].getX(), b->coll_box[2].getY(), b->coll_box[3].getX(), b->coll_box[3].getY(), b->getLayer(), 0xFFFF0000);				
		g_engine->line_list.push_back(temp_line2);
		Line *temp_line3 = new Line(b->coll_box[3].getX(), b->coll_box[3].getY(), b->coll_box[0].getX(), b->coll_box[0].getY(), b->getLayer(), 0xFFFF0000);				
		g_engine->line_list.push_back(temp_line3);
	}	

	if(player->getType()!=IGRAC_1 && player->getClass()!=ENEMY)
	{
		setUp_B(player,b);
		return;
	}

	//zasto ne radi ?	
	if(player->getMovementState()==OUTOFCONTROL) player->setVelocityX(0);
	player->setVelocityY(0);
	player->setMovementState(WALK);
	player->setY(b->getY()-player->h());

	//micanje kad stoji na njoj
	player->setX(player->getX()+b->getVelocityX()*g_engine->game_speed*g_engine->GetDeltaFix());
	if(b->getVelocityY()>0)
		player->setVelocityY(b->getVelocityY());

	player->setMovementState(WALK);
	if(!IHitAPlatform(player,b,300)) IHitAPlatform(b,player,300);
}

void PlayerCollision::setDown_P(Sprite *player, Sprite *b,int i)
{
	if(this->down==false) this->down=true;
	else return;

	//draw
	if(g_engine->display_lineofsight)
	{
		Line *temp_line0 = new Line(b->coll_box[0].getX(), b->coll_box[0].getY(), b->coll_box[1].getX(), b->coll_box[1].getY(), b->getLayer(), 0xFFFFFF00);				
		g_engine->line_list.push_back(temp_line0);
		Line *temp_line1 = new Line(b->coll_box[1].getX(), b->coll_box[1].getY(), b->coll_box[2].getX(), b->coll_box[2].getY(), b->getLayer(), 0xFFFFFF00);				
		g_engine->line_list.push_back(temp_line1);
		Line *temp_line2 = new Line(b->coll_box[2].getX(), b->coll_box[2].getY(), b->coll_box[3].getX(), b->coll_box[3].getY(), b->getLayer(), 0xFFFFFF00);				
		g_engine->line_list.push_back(temp_line2);
		Line *temp_line3 = new Line(b->coll_box[3].getX(), b->coll_box[3].getY(), b->coll_box[0].getX(), b->coll_box[0].getY(), b->getLayer(), 0xFFFFFF00);				
		g_engine->line_list.push_back(temp_line3);
	}	

	if(player->getType()!=IGRAC_1 && player->getClass()!=ENEMY)
	{
		setDown_B(player,b);
		return;
	}

	player->setVelocityY(0);	
	player->setY(b->getY()+b->h());

	if(!IHitAPlatform(player,b,100)) IHitAPlatform(b,player,100);
}

void PlayerCollision::setLeft_B(Sprite *player, Sprite *b)
{
	if(player->rotate)//ne radi kako treba, ne odbije se
	{
		double half_diagonal = 0;

		if (player->w() < player->h())
			half_diagonal = player->hw();
		else
			half_diagonal = player->hh();

		player->setX(b->getX()-half_diagonal);	
		//player->setState(player->getState()+1);
	}
	else
	{
		player->setX(b->getX()-player->w());
	}

	player->setVelocityX(-player->getVelocityX()*player->elastic);	

	if (player->getVelocityY()<0)
		player->setVelocityY(player->getVelocityY()*player->elastic);//slow down if going up
	else
		player->setVelocityY(player->getVelocityY());//keep initial speed

	if(!IHitAPlatform(player,b,200)) IHitAPlatform(b,player,200);

}

void PlayerCollision::setRight_B(Sprite *player, Sprite *b)
{
	if(player->rotate)//ne radi kako treba, ne odbije se
	{
		double half_diagonal = 0;

		if (player->w() < player->h())
			half_diagonal = player->hw();
		else
			half_diagonal = player->hh();

		player->setX(b->getX()+b->w()+half_diagonal);		
	}
	else
	{
		player->setX(b->getX()+b->w());
	}

	player->setVelocityX(-player->getVelocityX()*player->elastic);	

	if (player->getVelocityY()<0)
		player->setVelocityY(player->getVelocityY()*player->elastic);//slow down if going up
	else
		player->setVelocityY(player->getVelocityY());//keep initial speed

	if(!IHitAPlatform(player,b,400)) IHitAPlatform(b,player,400);
}

void PlayerCollision::setUp_B(Sprite *player, Sprite *b)
{
	if(player->rotate)//ne radi kako treba, ne odbije se
	{
		double half_diagonal = 0;

		if (player->w() < player->h())
			half_diagonal = player->hw();
		else
			half_diagonal = player->hh();

		player->setY(b->getY()-half_diagonal);		
	}
	else
	{
		player->setY(b->getY()-player->h());		
	}

	player->setVelocityY(-player->getVelocityY()*player->elastic);
	player->setVelocityX(player->getVelocityX()*player->elastic);

	//micanje kad stoji na njoj
	player->setX(player->getX()+b->getVelocityX()*g_engine->game_speed*g_engine->GetDeltaFix());

	player->setMovementState(WALK);//zbog spin in air
	if(!IHitAPlatform(player,b,300)) IHitAPlatform(b,player,300);

}

void PlayerCollision::setDown_B(Sprite *player, Sprite *b)
{
	if(player->rotate)//ne radi kako treba, ne odbije se
	{
		double half_diagonal = 0;

		if (player->w() < player->h())
			half_diagonal = player->hw();
		else
			half_diagonal = player->hh();

		player->setY(b->getY()+b->h()+half_diagonal);		
	}
	else
	{
		player->setY(b->getY()+b->h());
	}

	player->setVelocityY(-player->getVelocityY()*player->elastic);

	if(!IHitAPlatform(player,b,100)) IHitAPlatform(b,player,100);

}