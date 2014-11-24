#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Beheaded.h"

BeheadedModel::BeheadedModel()
{
	this->skeleton = NULL;
}

bool BeheadedModel::Load(std::string root_directory, std::string name)
{
	this->root_directory = root_directory;
	this->name = name;	

	//LOAD ANIMATIONS
	if (!this->animation_hit_front_air.Load(this->root_directory + "gnaar_hit_front_hard_air.txt","gnaar_hit_front_hard_air")) return false;
	if (!this->animation_hit_front_ground.Load(this->root_directory + "gnaar_hit_front_hard_ground.txt","gnaar_hit_front_hard_ground")) return false;
	if (!this->animation_hit_weak.Load(this->root_directory + "gnaar_hit_weak.txt","gnaar_hit_weak")) return false;
	if (!this->animation_hit_weak_ground.Load(this->root_directory + "gnaar_hit_weak_ground.txt","gnaar_hit_weak_weak")) return false;
	if (!this->animation_run.Load(this->root_directory + "run.txt","run")) return false;
	if (!this->animation_walk.Load(this->root_directory + "walk.txt","run")) return false;
	if (!this->animation_stand.Load(this->root_directory + "stand.txt","stand")) return false;

	return true;

}

Beheaded::Beheaded(void)
{	
	//MISC
	this->t_bullet = NULL;	
	this->t_pistol = NULL;
	this->t_grenade = NULL;
	this->t_tongue = NULL;

	this->velocity_walk = 1.25;

	this->walk_gunner = 1;	
	this->run_gunner = 2.5;

	this->velocity_run = 3.5;
	this->velocity_run_kamikaze = 5;

}

bool Beheaded::LoadTextures(void)
{
	if (!this->enforcer_punch.Load("Data/Textures/Enemies/enforcer/punch.txt","punch")) return false;

	BeheadedModel *temp_model = new BeheadedModel();
	if(temp_model->Load("Data/Textures/Enemies/guard/light/","name"))
	{
		//LOAD SKELETON
		temp_model->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/guard/light/guard_light.txt");
		if(temp_model->skeleton==NULL) return false;
		this->beheaded_models.push_back(temp_model);
	}
	else
		return false;

	BeheadedModel *temp_model2 = new BeheadedModel();
	if(temp_model2->Load("Data/Textures/Enemies/guard/shotgun/","name"))
	{
		//LOAD SKELETON
		temp_model2->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/guard/shotgun/guard_shotgun.txt");
		if(temp_model2->skeleton==NULL) return false;
		this->beheaded_models.push_back(temp_model2);
	}
	else
		return false;

	BeheadedModel *temp_model3 = new BeheadedModel();
	if(temp_model3->Load("Data/Textures/Enemies/guard/machinegun/","name"))
	{
		//LOAD SKELETON
		temp_model3->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/guard/machinegun/guard_machinegun.txt");
		if(temp_model3->skeleton==NULL) return false;
		this->beheaded_models.push_back(temp_model3);
	}
	else
		return false;

	BeheadedModel *temp_model4 = new BeheadedModel();
	if(temp_model4->Load("Data/Textures/Enemies/enforcer/","name"))
	{
		//LOAD SKELETON
		temp_model4->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/enforcer/enforcer.txt");
		if(temp_model4->skeleton==NULL) return false;
		this->beheaded_models.push_back(temp_model4);
	}
	else
		return false;	

	BeheadedModel *temp_model5 = new BeheadedModel();
	if(temp_model5->Load("Data/Textures/Enemies/parasite/","name"))
	{
		//LOAD SKELETON
		temp_model5->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/parasite/parasite.txt");
		if(temp_model5->skeleton==NULL) return false;
		this->beheaded_models.push_back(temp_model5);
	}
	else
		return false;

	BeheadedModel *temp_model6 = new BeheadedModel();
	if(temp_model6->Load("Data/Textures/Enemies/gunner/","name"))
	{
		//LOAD SKELETON
		temp_model6->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/gunner/gunner.txt");
		if(temp_model6->skeleton==NULL) return false;
		this->beheaded_models.push_back(temp_model6);
	}
	else
		return false;

	BeheadedModel *temp_model7 = new BeheadedModel();
	if(temp_model7->Load("Data/Textures/Enemies/Beheaded/Kamikaze/","name"))
	{
		//LOAD SKELETON
		temp_model7->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/Beheaded/Kamikaze/beheaded_kamikaze.txt");
		if(temp_model7->skeleton==NULL) return false;
		this->beheaded_models.push_back(temp_model7);
	}
	else
		return false;

	//TEXTURES
	this->t_bullet = g_engine->texture_manager->findTexture("Data/Textures/Weapons/bullet.tga");
	if (this->t_bullet==NULL) return false;

	this->t_tongue = g_engine->texture_manager->findTexture("Data/Textures/Enemies/parasite/tongue.png");
	if (this->t_tongue==NULL) return false;

	this->t_sacma = g_engine->texture_manager->findTexture("Data/Textures/Weapons/sacma.tga");
	if (this->t_sacma==NULL) return false;

	this->t_grenade = g_engine->texture_manager->findTexture("Data/Textures/Weapons/Granata.tga");
	if (this->t_grenade==NULL) return false;

	return true;

}//LoadTextures

Sprite* Beheaded::addGuard(double x, double y, int id,double scale_x,double scale_y,int energy,int difficulty,std::string command)
{
	Sprite *freak = new Sprite();
	freak->setClass(ENEMY);
	freak->setType(BEHEADED);
	freak->setState(0);//idle
	freak->setID(id);
	freak->setPosition(x,y);
	freak->rel_posx = 200 + rand()%300;//valjda udaljenost od igraca
	freak->setColor_Box(0xFFFF0000);
	freak->difficulty = difficulty;
	//freak->setScale(scale_x,scale_y);//ne moze ovo tu poremeti koliziju i sve ostalo !?!?
	g_engine->addSprite(freak,ENEMIES);

	if(energy <= 0)
	{
		if(id==GUARD_MACHINEGUN) freak->energy = 3;
		else if(id==GUARD_SHOTGUN) freak->energy = 3;
		else if(id==ENFORCER) freak->energy = 8;
		else if(id==PARASITE) freak->energy = 15;
		else if(id==GUNNER) freak->energy = 20;
		else freak->energy = 2;
	}
	else
	{
		if(id==ENFORCER && energy == 10) freak->energy = 8;//ne mogu sada sve minjat
		else
			freak->energy = energy;	
	}

	freak->setAnimationControl(ANIMATE_MANUAL);
	freak->fire_tick = rand()%100;

	this->addSkeleton(freak,scale_x,scale_y);	

	g_engine->coll_manager->setColl_box(freak);
	this->Update(freak,NULL,NULL);

	freak->setText(command);
	return freak;

}//addBeheadedFreak()

void Beheaded::addSkeleton(Sprite *a,double scale_x,double scale_y)
{
	a->frame_tick[0] = 0;	

	if (a->getID()==GUARD_SHOTGUN)
		this->beheaded_models[1]->skeleton->setSkeleton(a);
	else if (a->getID()==GUARD_MACHINEGUN)
		this->beheaded_models[2]->skeleton->setSkeleton(a);
	else if (a->getID()==ENFORCER)
		this->beheaded_models[3]->skeleton->setSkeleton(a);	
	else if (a->getID()==PARASITE)
		this->beheaded_models[4]->skeleton->setSkeleton(a);
	else if (a->getID()==GUNNER)
		this->beheaded_models[5]->skeleton->setSkeleton(a);
	else
		this->beheaded_models[0]->skeleton->setSkeleton(a);

	g_engine->skeleton_manager.Scale(a->skeleton, scale_x, scale_y);

	//set size
	int width = 24;
	if (a->getBone("$TORSO")->h()>24) width = a->getBone("$TORSO")->h();

	if(a->getBone("$FOOT1"))
	{
		a->setSize(width,
			(int)(a->getBone("$TORSO")->att_distance[0]+
			a->getBone("$LEG1_UP")->att_distance[0]+
			a->getBone("$LEG1_DOWN")->att_distance[0]+
			a->getBone("$FOOT1")->att_distance[0]));
	}
	else
	{
		a->setSize(width,
			(int)(a->getBone("$TORSO")->att_distance[0]+
			a->getBone("$LEG1_UP")->att_distance[0]+
			a->getBone("$LEG1_DOWN")->att_distance[0]));
	}

	if(a->getBone("$HEAD")) a->setHeight((int)(a->getHeight()+a->getBone("$HEAD")->att_distance[0]));	

	if(a->getID()==PARASITE)
	{
		if(a->getBone("$TORSO")) a->setWidth(a->getBone("$TORSO")->w());
		if(a->getBone("$HEAD")) a->setWidth(a->w()+a->getBone("$HEAD")->w());

		//a->setHeight(a->h()/2);
	}

}//addSkeleton()

void Beheaded::TurnLeft(Sprite *a)
{
	a->FlipAll_H(true);	
}
void Beheaded::TurnRight(Sprite *a)
{
	a->FlipAll_H(false);	
}

void Beheaded::Update(Sprite *a, Sprite *player, Sprite *player2)
{	
	if(player!=NULL) if(a->map_current!=player->map_current) player = NULL;
	if(player2!=NULL) if(a->map_current!=player2->map_current) player2 = NULL;

	this->UpdateSkeleton(a,player,player2);

}//Update()

void Beheaded::HitPlatform(Sprite *b, int direction, Sprite *a)
{
	//mozda da skoci posli, neki trigger variablu ubacit
	//ne radi , desi se da gnaar propadne, gnaar skace vise od beheadeda
	
	if(b->getState()!=0 && b->energy>0)// && b->getMovementState()==WALK
	{
		//platforma ispod je bas zavrsavala gdje i ova i skaka bi malo, mada sa to bi se treba sudarit odozgo !!!
		if (direction==400 || direction==200)
		{
			if(fabs(b->getY()+b->h()-a->getY())<=70 && fabs(b->getY()+b->h()-a->getY())>40)
			{
				b->jump = 2;
				if(g_engine->display_lineofsight)
				{
					Line *temp_line0 = new Line(b->getX(), b->getY()+b->h(), a->getX(), a->getY(), b->getLayer(), 0xFFFFFF00);				
					g_engine->line_list.push_back(temp_line0);
				}
			}
			else if(fabs(b->getY()+b->h()-a->getY())<=40)
			{
				b->jump = 1;
				if(g_engine->display_lineofsight)
				{
					Line *temp_line0 = new Line(b->getX(), b->getY()+b->h(), a->getX(), a->getY(), b->getLayer(), 0xFF00FF00);				
					g_engine->line_list.push_back(temp_line0);
				}
			}
			else
			{
				if (direction==400)
					TurnRight(b);
				else if (direction==200)
					TurnLeft(b);
				else
				{
					if (!b->flip_h)
						TurnRight(b);
					else
						TurnLeft(b);
				}
			}
		}
	}
	else
	{
		if (direction==400)
			TurnRight(b);
		else if (direction==200)
			TurnLeft(b);
		else
		{
			if (!b->flip_h)
				TurnRight(b);
			else
				TurnLeft(b);
		}
	}

	if (direction==300)
	{
		if(b->energy<=0)
		{
			if(b->animation_name=="fly") b->setFrameCurrent(0);
			b->setMovementState(STANDSTILL);
			b->setVelocityX(0);
		}		
	}

	/*if (b->flip_h)
		b->setName("L");
	else
		b->setName("R");*/

}//HitPlatform()

void Beheaded::UpdateSkeleton(Sprite *a, Sprite *player1, Sprite *player2)
{
	if (a->skeleton.empty()) return;

	int model_id = 0;
	if (a->getID()==KAMIKAZE)
		model_id = 0;	
	else if (a->getID()==GUARD_LIGHT || a->getID()==GUARD_SHOTGUN || a->getID()==GUARD_MACHINEGUN)
		model_id = 0;
	else if (a->getID()==ENFORCER)
		model_id = 3;
	else if (a->getID()==PARASITE)
		model_id = 4;
	else if (a->getID()==GUNNER)
		model_id = 5;	

	if (a->energy <= 0)//dead
	{
		if(a->getName()=="front")
		{
			if(a->getMovementState()==FALL)
			{
				a->setAnimationControl(ANIMATE_AUTO);
				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_hit_front_air.timer);
				a->setFramesTotal(this->beheaded_models[model_id]->animation_hit_front_air.totalframes);
				this->beheaded_models[model_id]->animation_hit_front_air.setRotation(a);
				this->UpdateSkeleton_lite(a);
				a->setHeight(fabs(a->getY()-a->getBone("$TORSO")->getY()));//da se smanji
			}
			else if(a->getMovementState()==STANDSTILL)
			{
				a->animation_name = "ground";
				a->setAnimationControl(ANIMATE_AUTO);
				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_hit_front_ground.timer);
				a->setFramesTotal(this->beheaded_models[model_id]->animation_hit_front_ground.totalframes);
				this->beheaded_models[model_id]->animation_hit_front_ground.setRotation(a);
				this->UpdateSkeleton_lite(a);
				a->setHeight(fabs(a->getY()-a->getBone("$TORSO")->getY()));//da se smanji
			}
		}
		else if(a->getName()=="back")
		{
			if(a->getMovementState()==FALL)
			{
				a->setAnimationControl(ANIMATE_AUTO);
				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_hit_weak.timer);
				a->setFramesTotal(this->beheaded_models[model_id]->animation_hit_weak.totalframes);
				this->beheaded_models[model_id]->animation_hit_weak.setRotation(a);
				this->UpdateSkeleton_lite(a);
				a->setHeight(fabs(a->getY()-a->getBone("$TORSO")->getY()));//da se smanji
			}
			else if(a->getMovementState()==STANDSTILL)
			{
				a->animation_name = "ground";
				a->setAnimationControl(ANIMATE_AUTO);
				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_hit_weak_ground.timer);
				a->setFramesTotal(this->beheaded_models[model_id]->animation_hit_weak_ground.totalframes);
				this->beheaded_models[model_id]->animation_hit_weak_ground.setRotation(a);
				this->UpdateSkeleton_lite(a);
				a->setHeight(fabs(a->getY()-a->getBone("$TORSO")->getY()));//da se smanji
			}
		}

		return;

	}//dead

	bool player_visible = false;
	double timer = 100;
	int shot_num = 1;
	double speed_run = 2;
	double speed_walk = 1;

	if(a->getID()==GUARD_LIGHT)
	{
		timer = 1250;
		shot_num = 2;
		speed_run = this->velocity_run;
		speed_walk = this->velocity_walk;
	}
	else if(a->getID()==GUARD_SHOTGUN)
	{
		timer = 2000;
		shot_num = 1;
		speed_run = this->velocity_run;
		speed_walk = this->velocity_walk;
	}
	else if(a->getID()==GUARD_MACHINEGUN)
	{
		timer = 80;
		shot_num = 4;
		speed_run = this->velocity_run;
		speed_walk = this->velocity_walk;
	}
	else if(a->getID()==ENFORCER)
	{
		timer = 60;
		shot_num = 6;
		speed_run = this->velocity_run-0.5;
		speed_walk = this->velocity_walk-0.25;
	}
	else if(a->getID()==PARASITE)
	{
		timer = 60;
		shot_num = 25;
		speed_run = this->velocity_run;
		speed_walk = this->velocity_walk;
	}
	else if(a->getID()==GUNNER)
	{		
		if(a->getState()==2)
		{
			timer = 80;
			shot_num = 6;
		}
		else
		{
			timer = 650;
			shot_num = 3;
		}
		speed_run = this->run_gunner;
		speed_walk = this->walk_gunner;
	}

	Sprite *player = NULL;

	if(a->getID()==PARASITE ||a->getID()==GUNNER || a->getID()==ENFORCER || a->getID()==GUARD_LIGHT || a->getID()==GUARD_SHOTGUN || a->getID()==GUARD_MACHINEGUN)
	{
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
					if(a->getID()==ENFORCER)
						g_engine->audio->Play("INFSGHT1");
					else if(a->getID()==PARASITE)
						g_engine->audio->Play("Parsght1");
					else if(a->getID()==GUNNER)
						g_engine->audio->Play("sight1");
					else
					{
						if(rand()%2)
							g_engine->audio->Play("SOLSGHT1");
						else
							g_engine->audio->Play("SOLSRCH1");
					}
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

				if(a->getID()==KAMIKAZE)
				{			
					if (a->flip_h)
						a->setVelocityX(-this->velocity_run_kamikaze);
					else
						a->setVelocityX(this->velocity_run_kamikaze);

					if(a->sound==NULL) a->sound = g_engine->audio->Play("kamikaze");
				}

				//kad sam dalje treba dulje trcat
				if (a->getID()==GUARD_LIGHT
					|| a->getID()==GUARD_SHOTGUN
					|| a->getID()==GUARD_MACHINEGUN
					|| a->getID()==ENFORCER
					|| a->getID()==GUNNER
					|| a->getID()==PARASITE)
				{
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
							a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_run.timer);
							a->setFramesTotal(this->beheaded_models[model_id]->animation_run.totalframes);
							a->animate();
							//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
							if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
							if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
							this->beheaded_models[model_id]->animation_run.setRotation(a);
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

							a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_stand.timer);
							a->setFramesTotal(this->beheaded_models[model_id]->animation_stand.totalframes);
							a->setFrameCurrent(0);//bez ovoga se rusi !!!!!
							a->animate();
							//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
							if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
							if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
							a->setVelocityX(0);
							this->beheaded_models[model_id]->animation_stand.setRotation(a);
						}
					}
					else
					{
						if(g_engine->coll_manager->raycasting_all(a,player) && g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<1000)//if see him, fire
						{
							if(a->getID()==GUNNER)
							{
								if(rand()%2)
									a->setState(2);
								else
									a->setState(4);
								a->tick_vision = 0;
							}
							else
							{
								a->setState(2);
								a->tick_vision = 0;
							}
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
				}
			}//state 1
			else if(a->getState()==10)//aware of target
			{
				if(a->fire_tick<10000) a->fire_tick += g_engine->game_speed*(g_engine->GetDeltaFix());				

				if(a->getID()==KAMIKAZE)
				{			
					if (a->flip_h)
						a->setVelocityX(-this->velocity_run_kamikaze);
					else
						a->setVelocityX(this->velocity_run_kamikaze);

					if(a->sound==NULL) a->sound = g_engine->audio->Play("kamikaze");
				}

				//kad sam dalje treba dulje trcat
				if (a->getID()==GUARD_LIGHT
					|| a->getID()==GUARD_SHOTGUN
					|| a->getID()==GUARD_MACHINEGUN
					|| a->getID()==ENFORCER
					|| a->getID()==GUNNER
					|| a->getID()==PARASITE)
				{
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
							a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_run.timer);
							a->setFramesTotal(this->beheaded_models[model_id]->animation_run.totalframes);
							a->animate();
							//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
							if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
							if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
							this->beheaded_models[model_id]->animation_run.setRotation(a);
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

							a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_stand.timer);
							a->setFramesTotal(this->beheaded_models[model_id]->animation_stand.totalframes);
							a->setFrameCurrent(0);//bez ovoga se rusi !!!!!
							a->animate();
							//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
							if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
							if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
							a->setVelocityX(0);
							this->beheaded_models[model_id]->animation_stand.setRotation(a);
						}

						if(g_engine->coll_manager->raycasting_all(a,player) && g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<1000)//if see him, fire
						{
							if(a->getID()==GUNNER)
							{
								if(rand()%2)
									a->setState(2);
								else
									a->setState(4);
								a->tick_vision = 0;
							}
							else
							{
								a->setState(2);
								a->tick_vision = 0;
							}							
						}						
					}
					else
					{
						if(g_engine->coll_manager->raycasting_all(a,player) && g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<1000)//if see him, fire
						{
							if(a->getID()==GUNNER)
							{
								if(rand()%2)
									a->setState(2);
								else
									a->setState(4);
								a->tick_vision = 0;
							}
							else
							{
								a->setState(2);
								a->tick_vision = 0;
							}
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
				}
			}//state 1
			else if(a->getState()==7)//punch
			{
				if (player->cx()<a->cx()) TurnLeft(a);
				else TurnRight(a);

				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->enforcer_punch.timer);
				a->setFramesTotal(this->enforcer_punch.totalframes);
				a->animate();
				//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
				if(a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
				if(a->getFrameCurrent() >= a->getFramesTotal()-1)
				{
					if(g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())>60)
					{
						a->setState(1);
						a->setAnimationLoop(ANIMATE_LOOP);
					}
					else
					{
						player->energy -= 5;
						a->setFrameCurrent(0);
						if(player->energy<=0)
						{
							a->setState(0);
							a->target = -999;
						}
					}
				}
				this->enforcer_punch.setRotation(a);			
			}
			else if(a->getState()==2 || a->getState()==4)//fire at target 8
			{
				if(a->fire_tick<10000) a->fire_tick += g_engine->game_speed*(g_engine->GetDeltaFix());

				if (player->cx()<a->cx()) TurnLeft(a);
				else TurnRight(a);

				a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_stand.timer);
				a->setFramesTotal(this->beheaded_models[model_id]->animation_stand.totalframes);
				a->setFrameCurrent(0);//bez ovoga se rusi !!!!!
				a->animate();
				//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
				if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
				if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
				a->setVelocityX(0);
				this->beheaded_models[model_id]->animation_stand.setRotation(a);

				//update bones
				double angle = g_engine->math->AngleToTarget(
					a->getBone("$ARM1_UP")->getX(),a->getBone("$ARM1_UP")->getY(),
					player->cx(),player->cy());

				if(a->getID()!=PARASITE)
				{
					if(a->getID()!=GUNNER)
					{
						a->getBone("$ARM1_UP")->setRotation(angle);
						a->getBone("$ARM1_DOWN")->setRotation(angle);
						a->getBone("$ARM1_UP")->attach(0,a->getBone("$ARM1_DOWN"));
					}
					else
					{
						if(a->getState()==2)
						{
							a->getBone("$ARM1_UP")->setRotation(angle);
							a->getBone("$ARM1_DOWN")->setRotation(angle);
							a->getBone("$ARM1_UP")->attach(0,a->getBone("$ARM1_DOWN"));
						}
						else
						{
							a->getBone("$ARM2_UP")->setRotation(angle);
							a->getBone("$ARM2_DOWN")->setRotation(angle);
							a->getBone("$ARM2_UP")->attach(0,a->getBone("$ARM2_DOWN"));
						}
					}
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
				if(a->getID()==PARASITE)
				{			
					if (g_engine->coll_manager->raycasting_all(a,player))
					{
						this->Fire_Parasite(a->getBone("$HEAD"),player);
					}

					a->tick_vision += 1;
					if(a->tick_vision >= shot_num)
					{
						if(fabs(a->cx()-player->cx())<200)
						{
							if(a->getID()!=GUNNER && a->getID()!=ENFORCER && a->getID()!=GUARD_MACHINEGUN)
							{
								a->setState(2);
								a->tick_vision = 0;
							}
							else
								a->setState(1);

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

						//ne znam sta bi, random koliko ce trcat
						a->rel_posy = 1500 + (rand()%15)*100;
					}
				}
				else if(a->fire_tick>timer/14)
				{
					if(a->tick_vision == 0)
					{
						if(a->getID()==GUNNER)
						{
							g_engine->audio->Play("Gunatck1");
						}
					}

					if(a->getID()==GUARD_LIGHT)
						this->Fire_Light(a->getBone("$ARM1_DOWN"));
					else if(a->getID()==GUARD_SHOTGUN)
					{
						this->Fire_Shotgun(a->getBone("$ARM1_DOWN"),4);
						g_engine->audio->Play("Solatck1");
					}
					else if(a->getID()==GUARD_MACHINEGUN)
					{
						this->Fire_Machinegun(a->getBone("$ARM1_DOWN"));
						g_engine->audio->Play("Solatck3");
					}
					else if(a->getID()==ENFORCER)
					{
						this->Fire_Machinegun(a->getBone("$ARM1_DOWN"));
						g_engine->audio->Play("Infatck1");
					}
					else if(a->getID()==GUNNER)
					{
						if(a->getState()==2)
						{
							this->Fire_Machinegun(a->getBone("$ARM1_DOWN"));
							g_engine->audio->Play("Gunatck2");
						}
						else
						{
							this->addGrenade(a->getBone("$ARM2_DOWN"));
							g_engine->audio->Play("Gunatck3");
						}						
					}					

					a->fire_tick = 0;
					a->tick_vision += 1;

					if(a->tick_vision >= shot_num)
					{
						if(g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<60)
						{
							if(a->getID()==ENFORCER)
							{
								a->setFrameCurrent(0);
								a->setState(7);
								a->tick_vision = 0;
							}							
						}
						else if(g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())<200)
						{
							if(a->getID()!=GUNNER && a->getID()!=ENFORCER && a->getID()!=GUARD_MACHINEGUN)
							{
								a->setState(2);
								a->tick_vision = 0;
							}
							else
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
			a->setFrameTimer_ms(this->beheaded_models[model_id]->animation_walk.timer);
			a->setFramesTotal(this->beheaded_models[model_id]->animation_walk.totalframes);
			a->animate();
			//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);

			if(a->getID()!=PARASITE)
			{
				if(a->flip_h)
					a->setVelocityX(-speed_walk);
				else
					a->setVelocityX(speed_walk);
			}
			else
			{
				a->setVelocityX(0);
			}

			this->beheaded_models[model_id]->animation_walk.setRotation(a);			
		}

		this->UpdateSkeleton_lite(a);
		
	}	
}

void Beheaded::Fire_Light(Sprite *weapon)
{
	double velocity;	

	Sprite *bullet = new Sprite();	
	bullet->setType(BIOMECH_LASER);
	bullet->setID(1);
	bullet->setImage(t_bullet);			
	bullet->energy = 2;
	bullet->setColor_Texture(D3DCOLOR_ARGB(255,255,192,0));
	velocity = 12;	

	bullet->setRotationAxis(0,bullet->hh());
	bullet->setRotation(weapon->getRotation());
	weapon->attach(0,bullet);	  

	bullet->setVelocity(
		g_engine->math->LinearVelocityX(g_engine->math->toDegrees(weapon->getRotation()))*velocity,
		g_engine->math->LinearVelocityY(g_engine->math->toDegrees(weapon->getRotation()))*velocity
		);

	bullet->setText("orangetrail");
	g_engine->coll_manager->setColl_line3(bullet);
	g_engine->addSprite(bullet,PARTICLES_COLL);

	g_engine->audio->Play("Solatck2");

}//fire

void Beheaded::Fire_Shotgun(Sprite *weapon, int amount)
{
	for(int i=0;i<amount;i++)
	{
		Sprite *bullet = new Sprite();	
		bullet->setType(BIOMECH_LASER);
		bullet->setImage(t_sacma);
		bullet->setWidth(25+(rand()%8));
		bullet->energy = 1;
		bullet->setLifetime(1000+rand()%150);

		//randomized angle
		double randomangle2 = 1;
		if (rand()%2) randomangle2 = -1;
		double randomize_angle = weapon->getRotation() + g_engine->math->toRadians(rand()%5*randomangle2);

		//randomized velocity
		double velocity = 12 + (rand()%20)*0.1;//12
		bullet->setVelocity(
			g_engine->math->LinearVelocityX(g_engine->math->toDegrees(randomize_angle))*velocity,
			g_engine->math->LinearVelocityY(g_engine->math->toDegrees(randomize_angle))*velocity
			);

		//rotation and position
		bullet->setRotation(randomize_angle);
		bullet->setRotationAxis(0,bullet->hh());
		weapon->attach(0,bullet);

		g_engine->coll_manager->setColl_line3(bullet);
		g_engine->addSprite(bullet,PARTICLES_COLL);		
	}

}//Fire_Shotgun()

void Beheaded::Fire_Machinegun(Sprite *weapon)
{
	double velocity = 15;	//15

	Sprite *bullet = new Sprite();	
	bullet->setType(BIOMECH_LASER);
	bullet->setImage(t_sacma);			
	bullet->energy = 1;	

	bullet->setRotationAxis(0,bullet->hh());
	bullet->setRotation(weapon->getRotation());
	weapon->attach(0,bullet);	  

	bullet->setVelocity(
		g_engine->math->LinearVelocityX(g_engine->math->toDegrees(weapon->getRotation()))*velocity,
		g_engine->math->LinearVelocityY(g_engine->math->toDegrees(weapon->getRotation()))*velocity
		);

	g_engine->coll_manager->setColl_line3(bullet);
	g_engine->addSprite(bullet,PARTICLES_COLL);	

}//fire

void Beheaded::Fire_Parasite(Sprite *head, Sprite *player)
{
	if(head==NULL || player==NULL) return;

	float distance = g_engine->math->Distance(head->getX(),head->getY(),player->cx(),player->cy());
	if(distance > 500) return;

	Sprite *bullet_start = new Sprite();
	bullet_start->setName("tongue");
	bullet_start->setType(BIOMECH_LASER);
	bullet_start->setImage(t_tongue);
	bullet_start->oneframelife = true;
	bullet_start->energy = 0.2*g_engine->game_speed*g_engine->GetDeltaFix();	

	bullet_start->setRotationAxis(0,bullet_start->hh());
	bullet_start->setRotation(g_engine->math->AngleToTarget(head->getX(),head->getY(),player->cx(),player->cy()));
	bullet_start->setPosition(head->getAtt_posX(0),head->getAtt_posY(0));
	bullet_start->setAtt_num(1);
	bullet_start->setAtt_pos(0,bullet_start->getImage()->getWidth(),bullet_start->getImage()->getHeight()/2);
	g_engine->addSprite(bullet_start,PARTICLES_COLL);
	
	if(distance<100) bullet_start->setWidth(100);
	else bullet_start->setWidth(distance);

	g_engine->coll_manager->setColl_line3(bullet_start);	

	float coll_xp;
	float coll_yp;
	Sprite *temp_sprite_p = g_engine->coll_manager->collision_line_getclosest(
		bullet_start->coll_box[2].getX(),bullet_start->coll_box[2].getY(),
		bullet_start->coll_box[0].getX(),bullet_start->coll_box[0].getY(),
		WORLD_FRONT_COLL,
		coll_xp,coll_yp);

	float coll_xe;
	float coll_ye;
	int temp_map = g_engine->map_current;//jer player je iz mape 0
	g_engine->map_current = 0;
	Sprite *temp_sprite_e = g_engine->coll_manager->collision_line_getclosest(
		bullet_start->coll_box[2].getX(),bullet_start->coll_box[2].getY(),
		bullet_start->coll_box[0].getX(),bullet_start->coll_box[0].getY(),
		PLAYER,
		coll_xe,coll_ye);
	g_engine->map_current = temp_map;

	float xf = 0;
	float yf = 0;

	if(temp_sprite_p && temp_sprite_e)
	{
		float distance_p = (float)g_engine->math->Distance(bullet_start->coll_box[2].getX(),bullet_start->coll_box[2].getY(),coll_xp,coll_yp);
		float distance_e = (float)g_engine->math->Distance(bullet_start->coll_box[2].getX(),bullet_start->coll_box[2].getY(),coll_xe,coll_ye);
		if(distance_p < distance_e)
		{
			bullet_start->setWidth(distance_p);
			g_engine->coll_manager->collbox_update(bullet_start);
			if(!game_spriteCollision(bullet_start,temp_sprite_p)) game_spriteCollision(temp_sprite_p,bullet_start);
			distance = distance_p;
		}
		else
		{
			bullet_start->setWidth(distance_e);
			g_engine->coll_manager->collbox_update(bullet_start);
			if(!game_spriteCollision(bullet_start,temp_sprite_e)) game_spriteCollision(temp_sprite_e,bullet_start);
			distance = distance_e;
		}
	}
	else if(temp_sprite_p)
	{
		float distance_p = (float)g_engine->math->Distance(bullet_start->coll_box[2].getX(),bullet_start->coll_box[2].getY(),coll_xp,coll_yp);
		bullet_start->setWidth(distance_p);
		g_engine->coll_manager->collbox_update(bullet_start);
		if(!game_spriteCollision(bullet_start,temp_sprite_p)) game_spriteCollision(temp_sprite_p,bullet_start);
		distance = distance_p;
	}
	else if(temp_sprite_e)
	{
		float distance_e = (float)g_engine->math->Distance(bullet_start->coll_box[2].getX(),bullet_start->coll_box[2].getY(),coll_xe,coll_ye);
		bullet_start->setWidth(distance_e);
		g_engine->coll_manager->collbox_update(bullet_start);
		if(!game_spriteCollision(bullet_start,temp_sprite_e)) game_spriteCollision(temp_sprite_e,bullet_start);
		distance = distance_e;
	}

	bullet_start->setImage(t_tongue);
	bullet_start->coll_box.clear();
	bullet_start->setAlive(true);	

	float d = bullet_start->w();	

	while(d<distance)
	{
		Sprite *bullet = new Sprite();	
		bullet->setType(BIOMECH_LASER);
		bullet->setImage(t_tongue);		
		bullet->oneframelife = true;

		bullet->setRotationAxis(0,bullet->hh());
		bullet->setRotation(g_engine->math->AngleToTarget(head->getX(),head->getY(),player->cx(),player->cy()));
		bullet->setAtt_num(1);
		bullet->setAtt_pos(0,bullet->getImage()->getWidth(),bullet->getImage()->getHeight()/2);
		bullet->setCollidable(false);
		g_engine->addSprite(bullet,PARTICLES_COLL);

		//next
		bullet_start->attach(0,bullet);
		bullet_start = bullet;
		d += bullet->w();		
	}	

}//Fire_Parasite()

void Beheaded::addGrenade(Sprite *weapon)
{
	Sprite *s_grenade = new Sprite();
	s_grenade->setType(GRENADE_E);
	s_grenade->setImage(this->t_grenade);
	s_grenade->setScale(0.25,0.25);
	s_grenade->setPosition(weapon->getAtt_posX(0)-s_grenade->hw(),weapon->getAtt_posY(0)-s_grenade->hh());

	//LV degrees
	s_grenade->setVelocity(
		g_engine->math->LinearVelocityX(g_engine->math->toDegrees(weapon->getRotation()))*16,
		g_engine->math->LinearVelocityY(g_engine->math->toDegrees(weapon->getRotation()))*16);

	s_grenade->setMovementState(FALL);
	s_grenade->elastic = 0.5;
	s_grenade->energy = 5;//ekslpozija bi trebala oduzet energiju
	s_grenade->setColor_Box(0xFF00FFFF);
	g_engine->addSprite( s_grenade,PARTICLES_COLL );
	g_engine->audio->Play("HGRENT1A");
	
	g_engine->coll_manager->setColl_box(s_grenade);//dot collision

}//addGrenade()

void Beheaded::UpdateSkeleton_lite(Sprite *a)
{
	int model_id = 0;
	if (a->getID()==KAMIKAZE)
		model_id = 0;	

	if(a->energy>0)
	{
		if(a->getID()==PARASITE)
		{
			if(a->getVelocityX()!=0) g_engine->skeleton_manager.ConnectBones(a);
			else g_engine->skeleton_manager.Walk_Bipedal(a);
		}
		else g_engine->skeleton_manager.Walk_Bipedal(a);
	}
	else
		g_engine->skeleton_manager.ConnectBones(a);
}

void Beheaded::DontFall(Sprite *test_me)
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