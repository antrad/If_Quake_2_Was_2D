#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Biomech.h"

BiomechModel::BiomechModel()
{
	this->skeleton = NULL;
}

bool BiomechModel::Load(std::string root_directory, std::string name)
{
	this->root_directory = root_directory;
	this->name = name;

	//LOAD ANIMATIONS
	if (!this->animation_walk.Load(this->root_directory + "walk.txt","walk")) return false;
	if (!this->animation_stand.Load(this->root_directory + "stand.txt","stand")) return false;
	if (!this->animation_dead.Load(this->root_directory + "dead.txt","dead")) return false;	

	return true;
}

Biomech::Biomech(void)
{
	//misc
	this->rocket = NULL;
	this->laser = NULL;	

	this->velocity_minor = 2.25;
	this->velocity_major = 2.25;

	this->energy_minor = 10;
	this->energy_major = 50;	

}//Biomech()

bool Biomech::LoadTextures(void)
{
	//PROJECTILES
	this->rocket = g_engine->texture_manager->findTexture("Data/Textures/Enemies/Biomech/rocket.tga");
	if (this->rocket==NULL){g_engine->message("\"Data/Textures/Enemies/Biomech/rocket.tga\" not preloaded.");return false;}
	this->laser = g_engine->texture_manager->findTexture("Data/Textures/Enemies/Biomech/bm_laser.tga");
	if (this->laser==NULL){g_engine->message("\"Data/Textures/Enemies/Biomech/bm_laser.tga\" not preloaded.");return false;}

	BiomechModel *temp_model = new BiomechModel();
	if(temp_model->Load("Data/Textures/Enemies/Biomech/Minor/","name"))
	{
		//LOAD SKELETON
		temp_model->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/Biomech/Minor/biomech_minor.txt");
		if(temp_model->skeleton==NULL) return false;
		this->biomech_models.push_back(temp_model);
	}
	else
		return false;

	BiomechModel *temp_model2 = new BiomechModel();
	if(temp_model2->Load("Data/Textures/Enemies/Biomech/Major/","name"))
	{
		//LOAD SKELETON
		temp_model2->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/Biomech/Major/biomech_major.txt");

		if(temp_model2->skeleton==NULL) return false;
		this->biomech_models.push_back(temp_model2);
	}
	else
		return false;

	return true;

}//LoadTextures()

Sprite* Biomech::addBiomech(double x, double y, int id,double scale_x,double scale_y,int energy,int difficulty,std::string command)
{
	Sprite *biomech = new Sprite();
	biomech->setClass(ENEMY);
	biomech->setType(BIOMECH);	
	biomech->setID(id);
	biomech->setMovementState(FALL);
	biomech->setPosition(x,y);	
	biomech->setColor_Box(0xFFFF0000);
	biomech->fire_tick = 100 + rand()%50;
	biomech->difficulty = difficulty;
	//biomech->setScale(scale_x,scale_y);//ne moze ovo tu poremeti koliziju i sve ostalo !?!?
	g_engine->addSprite(biomech,ENEMIES);	

	if(id==BIOMECH_MINOR)
	{
		this->addSkeleton(biomech,scale_x,scale_y);
		biomech->setFrameTimer_ms(this->biomech_models[0]->animation_walk.timer/2);
		biomech->setFramesTotal(this->biomech_models[0]->animation_walk.totalframes);
		biomech->setFrameCurrent(rand()%this->biomech_models[0]->animation_walk.totalframes);
	}
	else if(id==BIOMECH_MAJOR)
	{
		this->addSkeleton(biomech,scale_x,scale_y);		
		biomech->setFrameTimer_ms(this->biomech_models[1]->animation_walk.timer);
		biomech->setFramesTotal(this->biomech_models[1]->animation_walk.totalframes);
		biomech->setFrameCurrent(rand()%this->biomech_models[1]->animation_walk.totalframes);
	}

	if(energy <= 0)
	{
		if(id==BIOMECH_MINOR) biomech->energy = this->energy_minor;
		else if(id==BIOMECH_MAJOR) biomech->energy = this->energy_major;		
	}
	else
		biomech->energy = energy;	

	//misc
	g_engine->coll_manager->setColl_box(biomech);	
	this->Update(biomech,NULL,NULL);	

	biomech->setText(command);
	return biomech;

}//addBiomech()

void Biomech::addSkeleton(Sprite *a,double scale_x,double scale_y)
{
	a->frame_tick[0]=0;

	if(a->getID()==BIOMECH_MINOR)
	{
		this->biomech_models[0]->skeleton->setSkeleton(a);		
	}
	else if(a->getID()==BIOMECH_MAJOR)
	{
		this->biomech_models[1]->skeleton->setSkeleton(a);		
	}

	g_engine->skeleton_manager.Scale(a->skeleton, scale_x, scale_y);

	//set size
	a->setSize(
		(int)a->getBone("$HEAD")->h()-20,
		(int)a->getBone("$HEAD")->w()+(int)a->getBone("$LEG1_UP")->att_distance[0]+(int)a->getBone("$LEG1_DOWN")->att_distance[0]+(int)a->getBone("$FOOT1")->att_distance[0]-20
		);

}//addSkeleton()

void Biomech::Update(Sprite *a, Sprite *player, Sprite *player2)
{
	if(player!=NULL) if(a->map_current!=player->map_current) player = NULL;
	if(player2!=NULL) if(a->map_current!=player2->map_current) player2 = NULL;

	this->UpdateSkeleton(a,player,player2);

}//update

void Biomech::HitPlatform(Sprite *b, int direction, Sprite *a)
{
	if (b->getMovementState()==WALK || b->getMovementState()==WALKANDFALL )
	{
		if (direction==400)
			TurnRight(b);
		else if (direction==200)
			TurnLeft(b);
	}

	if (direction==300)
	{
		if(b->energy<=0)
		{
			if(b->animation_name=="fly") b->setFrameCurrent(0);
			b->setMovementState(STANDSTILL);
			b->setVelocityX(0);
		}
		else
		{
			if (a->w()<2*b->w())
				b->setMovementState(WALKANDFALL);
			else
				b->setMovementState(WALK);
		}
	}          

}//HitPlatform()

void Biomech::TurnLeft(Sprite *a)
{
	a->FlipAll_H(true);

}//TurnLeft()

void Biomech::TurnRight(Sprite *a)
{
	a->FlipAll_H(false);

}//TurnRight()

void Biomech::UpdateSkeleton(Sprite *a, Sprite *player1, Sprite *player2)
{
	//return;

	if (a->skeleton.empty()) return;

	int model_id = 0;
	if(a->getID()==BIOMECH_MAJOR) model_id = 1;

	if (a->energy <= 0)
	{
		a->animation_name = "ground";
		a->setAnimationLoop(ANIMATE_ONCE);
		a->setFrameTimer_ms(this->biomech_models[model_id]->animation_dead.timer);
		a->setFramesTotal(this->biomech_models[model_id]->animation_dead.totalframes);
		this->biomech_models[model_id]->animation_dead.setRotation(a);
		this->UpdateSkeleton_lite(a);
		return;
	}

	bool player_visible = false;
	double timer = 100;
	int shot_num = 1;
	double speed_run = 0;	

	if(a->getID()==BIOMECH_MINOR)
	{
		timer = 200;
		shot_num = 4;
		speed_run = this->velocity_minor;		
	}
	if(a->getID()==BIOMECH_MAJOR)
	{
		timer = 300;
		shot_num = 2;
		speed_run = this->velocity_major;		
	}	

	Sprite *player = NULL;

	if(player1 != NULL || player2 != NULL)
	{
		if(a->getState()==0)//search target
		{
			if(player1!=NULL)
			{
				if(g_engine->math->Distance(a->cx(),a->cy(),player1->cx(),player1->cy())<750)
					player_visible = g_engine->coll_manager->raycasting(a,player1);
				a->target = 0;
			}

			if(player2!=NULL && player_visible == false)
			{
				if(g_engine->math->Distance(a->cx(),a->cy(),player2->cx(),player2->cy())<750)
					player_visible = g_engine->coll_manager->raycasting(a,player2);
				a->target = 1;
			}

			if(player_visible)				
			{
				a->setState(1);//attack				
			}
		}
	}

	if(a->target==0 && player1!=NULL) player = player1;
	if(a->target==1 && player2!=NULL) player = player2;

	if(player!=NULL)
	{
		if(a->getState()==1 || a->getState()==5)//lock on target
		{
			if(a->fire_tick<10000) a->fire_tick += g_engine->game_speed*(g_engine->GetDeltaFix());


			if(a->tick_vision<a->rel_posy/14)
			{
				a->tick_vision += g_engine->game_speed*(g_engine->GetDeltaFix());
				if(a->getState()==1)
				{
					if(a->getArg(0)=="dontfall") this->DontFall(a);
					else
					{
						if (a->getName()=="L")
						{
							TurnLeft(a);
							a->setVelocityX(-speed_run);
						}
						else
						{
							TurnRight(a);
							a->setVelocityX(speed_run);	
						}
					}

					a->tick_vision += g_engine->game_speed*(g_engine->GetDeltaFix());
					a->setFrameTimer_ms(this->biomech_models[model_id]->animation_walk.timer);
					a->setFramesTotal(this->biomech_models[model_id]->animation_walk.totalframes);
					a->animate();
					//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
					if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
					if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
					this->biomech_models[model_id]->animation_walk.setRotation(a);
				}
				else
				{
					if (player->cx()<a->cx()) TurnLeft(a);
					else TurnRight(a);

					//update bones
					if(a->getBone("$HEAD"))
					{
						double angle = g_engine->math->AngleToTarget(
							a->getBone("$HEAD")->getX(),a->getBone("$HEAD")->getY(),
							player->cx(),player->cy());

						if(!a->flip_h && angle<-0.7) angle = -0.7;
						if(!a->flip_h && angle>0.7) angle = 0.7;
						if(a->flip_h && angle>-2.44 && angle<0) angle = -2.44;
						if(a->flip_h && angle<2.44 && angle>0) angle = 2.44;
						a->getBone("$HEAD")->setRotation(angle);
					}

					a->setFrameTimer_ms(this->biomech_models[model_id]->animation_stand.timer);
					a->setFramesTotal(this->biomech_models[model_id]->animation_stand.totalframes);
					a->setFrameCurrent(0);//bez ovoga se rusi !!!!!
					a->animate();
					//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
					if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
					if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
					a->setVelocityX(0);
					this->biomech_models[model_id]->animation_stand.setRotation(a);
				}
			}
			else
			{
				if(g_engine->coll_manager->raycasting_all(a,player) && g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<1000)//if see him, fire
				{
					a->setState(2);
					a->tick_vision = 0;
				}
				else//dont see him, run around
				{
					a->setState(10);
					a->tick_vision = 30000;
					if (player->cx()<a->cx()) TurnLeft(a);			
					else TurnRight(a);

					if(a->getState()==1)
					{
						if (a->flip_h)
							a->setName("L");
						else
							a->setName("R");

						if (a->getName()=="L")
						{
							TurnLeft(a);
							a->setVelocityX(-speed_run);								
						}
						else
						{
							TurnRight(a);
							a->setVelocityX(speed_run);								
						}
					}
				}
			}
		}//state 1
		else if(a->getState()==10)//aware of target
		{
			if(a->fire_tick<10000) a->fire_tick += g_engine->game_speed*(g_engine->GetDeltaFix());

			if(a->tick_vision<a->rel_posy/14)
			{
				a->tick_vision += g_engine->game_speed*(g_engine->GetDeltaFix());
				if(a->getState()==10)
				{
					if(a->getArg(0)=="dontfall") this->DontFall(a);
					else
					{
						if (a->getName()=="L")
						{
							TurnLeft(a);
							a->setVelocityX(-speed_run);
						}
						else
						{
							TurnRight(a);
							a->setVelocityX(speed_run);	
						}
					}

					a->tick_vision += g_engine->game_speed*(g_engine->GetDeltaFix());
					a->setFrameTimer_ms(this->biomech_models[model_id]->animation_walk.timer);
					a->setFramesTotal(this->biomech_models[model_id]->animation_walk.totalframes);
					a->animate();
					//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
					if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
					if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
					this->biomech_models[model_id]->animation_walk.setRotation(a);
				}
				else
				{
					if (player->cx()<a->cx()) TurnLeft(a);
					else TurnRight(a);

					//update bones
					if(a->getBone("$HEAD"))
					{
						double angle = g_engine->math->AngleToTarget(
							a->getBone("$HEAD")->getX(),a->getBone("$HEAD")->getY(),
							player->cx(),player->cy());

						if(!a->flip_h && angle<-0.7) angle = -0.7;
						if(!a->flip_h && angle>0.7) angle = 0.7;
						if(a->flip_h && angle>-2.44 && angle<0) angle = -2.44;
						if(a->flip_h && angle<2.44 && angle>0) angle = 2.44;
						a->getBone("$HEAD")->setRotation(angle);
					}

					a->setFrameTimer_ms(this->biomech_models[model_id]->animation_stand.timer);
					a->setFramesTotal(this->biomech_models[model_id]->animation_stand.totalframes);
					a->setFrameCurrent(0);//bez ovoga se rusi !!!!!
					a->animate();
					//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
					if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
					if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
					a->setVelocityX(0);
					this->biomech_models[model_id]->animation_stand.setRotation(a);
				}

				if(g_engine->coll_manager->raycasting_all(a,player) && g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<1000)//if see him, fire
				{
					a->setState(2);
					a->tick_vision = 0;											
				}						
			}
			else
			{
				if(g_engine->coll_manager->raycasting_all(a,player) && g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<1000)//if see him, fire
				{
					a->setState(2);
					a->tick_vision = 0;
				}
				else//dont see him, run around
				{
					a->setVelocityAngular(a->getVelocityAngular()+1);

					//if in panic and 10 times not able to see, calm down
					if(a->getVelocityAngular() >= 10)
					{
						a->setVelocityAngular(0);
						a->setState(0);
						a->target = -999;
					}
					else
					{
						a->setState(10);
						a->tick_vision = 0;
						if (player->cx()<a->cx()) TurnLeft(a);			
						else TurnRight(a);

						if(a->getState()==10)
						{
							if (a->flip_h)
								a->setName("L");
							else
								a->setName("R");

							if (a->getName()=="L")
							{
								TurnLeft(a);
								a->setVelocityX(-speed_run);								
							}
							else
							{
								TurnRight(a);
								a->setVelocityX(speed_run);								
							}
						}
					}
				}
			}
		}//state 1		
		else if(a->getState()==2 || a->getState()==4)//fire at target 8
		{
			if(a->fire_tick<10000) a->fire_tick += g_engine->game_speed*(g_engine->GetDeltaFix());

			if (player->cx()<a->cx()) TurnLeft(a);
			else TurnRight(a);

			a->setFrameTimer_ms(this->biomech_models[model_id]->animation_stand.timer);
			a->setFramesTotal(this->biomech_models[model_id]->animation_stand.totalframes);
			a->setFrameCurrent(0);//bez ovoga se rusi !!!!!
			a->animate();
			//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			a->setVelocityX(0);
			this->biomech_models[model_id]->animation_stand.setRotation(a);

			//update bones
			double angle = g_engine->math->AngleToTarget(
				a->getBone("$GUN1")->getX(),a->getBone("$GUN1")->getY(),
				player->cx(),player->cy());


			if(a->getState()==2)
			{
				a->getBone("$GUN1")->setRotation(angle);
				a->getBone("$GUN2")->setRotation(angle);

			}

			if(a->getBone("$HEAD"))
			{
				angle = g_engine->math->AngleToTarget(
					a->getBone("$HEAD")->getX(),a->getBone("$HEAD")->getY(),
					player->cx(),player->cy());

				if(!a->flip_h && angle<-0.7) angle = -0.7;
				if(!a->flip_h && angle>0.7) angle = 0.7;
				if(a->flip_h && angle>-2.44 && angle<0) angle = -2.44;
				if(a->flip_h && angle<2.44 && angle>0) angle = 2.44;

				a->getBone("$HEAD")->setRotation(angle);
			}

			//fire			
			if(a->fire_tick>timer/14)
			{
				this->FireWeapon(a,a->getBone("$GUN1"));
				this->FireWeapon(a,a->getBone("$GUN2"));
				if(a->getID()==BIOMECH_MINOR)
					g_engine->audio->Play("bluemechfire");
				else
					g_engine->audio->Play("ROCKLF1A");				

				a->fire_tick = 0;
				a->tick_vision += 1;

				if(a->tick_vision >= shot_num)
				{
					if(g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<450)
					{
						a->setState(1);
						//novo da ne zapinje
						if(a->getState()==1)
						{
							if (a->flip_h)
								a->setName("L");
							else
								a->setName("R");
						}

						if (a->getName()=="L")
						{
							TurnLeft(a);
							a->setVelocityX(-speed_run);								
						}
						else
						{
							TurnRight(a);
							a->setVelocityX(speed_run);								
						}

					}
					else
					{
						a->setState(1);
						if (player->cx()<a->cx()) TurnLeft(a);			
						else TurnRight(a);

						if(a->getState()==1)
						{
							if (a->flip_h)
								a->setName("L");
							else
								a->setName("R");
						}

						if (a->getName()=="L")
						{
							TurnLeft(a);
							a->setVelocityX(-speed_run);								
						}
						else
						{
							TurnRight(a);
							a->setVelocityX(speed_run);								
						}
					}

					//ne znam sta bi, random koliko ce trcat
					a->rel_posy = 1500 + (rand()%15)*100;
				}
			}//firetick			


		}//state 2

	}//player!=NULL
	else	
	{
		a->setState(0);
		a->target = -999;
		if(a->sound != NULL)
		{
			a->sound->stop();
			a->sound = NULL;
		}
	}

	if(a->getState()==0)//walk around
	{
		a->setFrameTimer_ms(this->biomech_models[model_id]->animation_walk.timer);
		a->setFramesTotal(this->biomech_models[model_id]->animation_walk.totalframes);
		a->animate();
		//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
		if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
		if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);


		if(a->flip_h)
			a->setVelocityX(-speed_run);
		else
			a->setVelocityX(speed_run);


		this->biomech_models[model_id]->animation_walk.setRotation(a);			
	}

	this->UpdateSkeleton_lite(a);	

}

void Biomech::UpdateSkeleton_lite(Sprite *a)
{
	if(a->getID()==BIOMECH_MINOR)
		g_engine->skeleton_manager.Walk_Bipedal(a);
	else if(a->getID()==BIOMECH_MAJOR)
		g_engine->skeleton_manager.Walk_Bipedal(a);
}

void Biomech::FireWeapon(Sprite *biomech,Sprite *weapon)
{
	Sprite *bullet = new Sprite();

	int velocity;

	if(biomech->getID()==BIOMECH_MINOR)
	{
		bullet->setType(BIOMECH_LASER);
		bullet->setImage(this->laser);
		bullet->energy = 2;
		velocity = 13;
	}
	else
	{
		bullet->setType(BIOMECH_ROCKET);
		bullet->setImage(this->rocket);
		bullet->setScale(0.75,0.75);
		bullet->energy = 15;
		velocity = 10;
	}	

	bullet->setRotationAxis(0,bullet->hh());
	bullet->setRotation(weapon->getRotation());
	weapon->attach(0,bullet);	  

	bullet->setVelocity(
		g_engine->math->LinearVelocityX(g_engine->math->toDegrees(weapon->getRotation()))*velocity,
		g_engine->math->LinearVelocityY(g_engine->math->toDegrees(weapon->getRotation()))*velocity
		);

	bullet->setColor_Box(0xFFFF8800);
	g_engine->coll_manager->setColl_line3(bullet);
	g_engine->addSprite(bullet,PARTICLES_COLL);

}//FireWeapon()

void Biomech::DontFall(Sprite *test_me)
{
	if(test_me->energy <=0 ) return;

	bool turn = true;

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
				if(test_me->getVelocityX()>0)
				{
					if(g_engine->coll_manager->DotInsideSprite(
						test_me->coll_box[2].getX(),test_me->coll_box[2].getY()+5,a))
						turn = false;
				}
				else if(test_me->getVelocityX()<0)
				{
					if(g_engine->coll_manager->DotInsideSprite(
						test_me->coll_box[3].getX(),test_me->coll_box[3].getY()+5,a))
						turn = false;
				}
				iter++;
			}
		}
	}

	if(turn)	
	{
		//g_engine->audio->Play("jump1");
		test_me->setVelocityX(-test_me->getVelocityX());

		if(test_me->getVelocityX()<0)
			this->TurnLeft(test_me);
		else if(test_me->getVelocityX()>0)
			this->TurnRight(test_me);
	}
}