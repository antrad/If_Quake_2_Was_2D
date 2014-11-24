#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Gnaar.h"

GnaarModel::GnaarModel()
{
	this->skeleton = NULL;
}

bool GnaarModel::Load(std::string root_directory, std::string name)
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
	
Gnaar::Gnaar(void)
{
	//misc
	this->energy_male = 2;
	this->energy_female = 4;

	this->velocity_walk = 1.5;
	this->velocity_run = 4.5;

}//Gnaar()

bool Gnaar::LoadTextures(void)
{
	GnaarModel *temp_model = new GnaarModel();
	if(temp_model->Load("Data/Textures/Enemies/Gnaar/Male/","name"))
	{
		//LOAD SKELETON
		temp_model->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/Gnaar/Male/gnaar_male.txt");
		if(temp_model->skeleton==NULL) return false;
		this->gnaar_models.push_back(temp_model);
	}
	else
		return false;

	GnaarModel *temp_model2 = new GnaarModel();
	if(temp_model2->Load("Data/Textures/Enemies/Gnaar/Female/","name"))
	{
		//LOAD SKELETON
		temp_model2->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/Gnaar/Female/gnaar_female.txt");
		if(temp_model2->skeleton==NULL) return false;
		this->gnaar_models.push_back(temp_model2);
	}
	else
		return false;

	return true;

}//LoadTextures()

Sprite* Gnaar::addGnaar(double x, double y, int id,double scale_x,double scale_y,int energy,int difficulty,std::string command)
{
	Sprite *gnaar = new Sprite();
	gnaar->setClass(ENEMY);
	gnaar->setType(GNAAR);
	gnaar->setID(id);	
	gnaar->setState(0);//idle
	gnaar->setPosition(x,y);
	gnaar->setColor_Box(0xFFFF0000);
	gnaar->difficulty = difficulty;
	//gnaar->setScale(scale_x,scale_y);//ne moze ovo tu poremeti koliziju i sve ostalo !?!?
	g_engine->addSprite(gnaar,ENEMIES);

	if(energy <= 0)
	{
		if(id==GNAAR_MALE) gnaar->energy = this->energy_male;
		else if(id==GNAAR_FEMALE) gnaar->energy = this->energy_female;		
	}
	else
		gnaar->energy = energy;

	this->addSkeleton(gnaar,scale_x,scale_y);	

	g_engine->coll_manager->setColl_box(gnaar);
	this->Update(gnaar,NULL,NULL);

	gnaar->setText(command);
	return gnaar;

}//addGnaarale()

void Gnaar::addSkeleton(Sprite *a,double scale_x,double scale_y)
{
	a->frame_tick[0]=0;
	
	if(a->getID()==GNAAR_MALE)
	{
		this->gnaar_models[0]->skeleton->setSkeleton(a);		
	}
	else if(a->getID()==GNAAR_FEMALE)
	{
		this->gnaar_models[1]->skeleton->setSkeleton(a);		
	}

	g_engine->skeleton_manager.Scale(a->skeleton, scale_x, scale_y);

	//set size
	int width = 24;
	if (a->getBone("$TORSO")->h()>24) width = a->getBone("$TORSO")->h();
	
	a->setSize(width,
		(int)(a->getBone("$TORSO")->att_distance[0]+
		(int)a->getBone("$LEG1_UP")->att_distance[0]+
		(int)a->getBone("$LEG1_DOWN")->att_distance[0]));

}//addSkeleton()

void Gnaar::HitPlatform(Sprite *b, int direction, Sprite *a)
{
	if(b->getState()==1 && b->getMovementState()==WALK && b->energy>0)
	{
		if (direction==400 || direction==200)
		{
			if(abs((b->getY()+b->h())-(a->getY()))<=40) b->setVelocityY(-4);
			if(abs((b->getY()+b->h())-(a->getY()))<=70) b->setVelocityY(-6.5);
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
		else
		{
			if (a->w()<2*b->w())
				b->setMovementState(WALKANDFALL);
			else
				b->setMovementState(WALK);
		}
	}

}//HitPlatform()

void Gnaar::TurnLeft(Sprite *a)
{
	a->FlipAll_H(true);
}
void Gnaar::TurnRight(Sprite *a)
{
	a->FlipAll_H(false);
}

void Gnaar::Update(Sprite *a, Sprite *player, Sprite *player2)
{
	this->UpdateSkeleton(a,player,player2);

}//Update()

void Gnaar::UpdateSkeleton_lite(Sprite *a)
{
	if(a->energy>0)
	{
		if(a->getID()==GNAAR_MALE)
			g_engine->skeleton_manager.Walk_Bipedal(a);
		else if(a->getID()==GNAAR_FEMALE)
			g_engine->skeleton_manager.Walk_Bipedal(a);
	}
	else
	{
		if(a->getID()==GNAAR_MALE)
			g_engine->skeleton_manager.ConnectBones(a);
		else if(a->getID()==GNAAR_FEMALE)
			g_engine->skeleton_manager.ConnectBones(a);
	}
}

void Gnaar::UpdateSkeleton(Sprite *a, Sprite *player1, Sprite *player2)
{
	if (a->skeleton.empty()) return;

	int model_id = 0;
	if(a->getID()==GNAAR_FEMALE) model_id = 1;

	if (a->energy <= 0)
	{
		if(a->getName()=="front")
		{
			if(a->getMovementState()==FALL)
			{
				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->gnaar_models[model_id]->animation_hit_front_air.timer);
				a->setFramesTotal(this->gnaar_models[model_id]->animation_hit_front_air.totalframes);
				this->gnaar_models[model_id]->animation_hit_front_air.setRotation(a);
				this->UpdateSkeleton_lite(a);
				a->setHeight(fabs(a->getY()-a->getBone("$TORSO")->getY()));//da se smanji
			}
			else if(a->getMovementState()==STANDSTILL)
			{
				a->animation_name = "ground";
				a->setAnimationControl(ANIMATE_AUTO);
				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->gnaar_models[model_id]->animation_hit_front_ground.timer);
				a->setFramesTotal(this->gnaar_models[model_id]->animation_hit_front_ground.totalframes);
				this->gnaar_models[model_id]->animation_hit_front_ground.setRotation(a);
				this->UpdateSkeleton_lite(a);
				a->setHeight(fabs(a->getY()-a->getBone("$TORSO")->getY()));//da se smanji
			}
		}
		else if(a->getName()=="back")
		{
			if(a->getMovementState()==FALL)
			{
				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->gnaar_models[model_id]->animation_hit_weak.timer);
				a->setFramesTotal(this->gnaar_models[model_id]->animation_hit_weak.totalframes);
				this->gnaar_models[model_id]->animation_hit_weak.setRotation(a);
				this->UpdateSkeleton_lite(a);
				a->setHeight(fabs(a->getY()-a->getBone("$TORSO")->getY()));//da se smanji
			}
			else if(a->getMovementState()==STANDSTILL)
			{
				a->animation_name = "ground";
				a->setAnimationControl(ANIMATE_AUTO);
				a->setAnimationLoop(ANIMATE_ONCE);
				a->setFrameTimer_ms(this->gnaar_models[model_id]->animation_hit_weak_ground.timer);
				a->setFramesTotal(this->gnaar_models[model_id]->animation_hit_weak_ground.totalframes);
				this->gnaar_models[model_id]->animation_hit_weak_ground.setRotation(a);
				this->UpdateSkeleton_lite(a);
				a->setHeight(fabs(a->getY()-a->getBone("$TORSO")->getY()));//da se smanji
			}
		}

		return;

	}

	//alive

	bool player_visible = false;

	Sprite *player = NULL;

	if (player!=NULL)
	{
		if (a->getState()==0)
		{
			//1000 vidno polje
			if (fabs(a->cx()-player->cx()) < 1000)
				player_visible = g_engine->coll_manager->raycasting(a,player);	

			if(player_visible)
			{
				a->setState(1);//attack
				/*if(rand()%2)
					g_engine->audio->Play("SOLSGHT1");
				else
					g_engine->audio->Play("SOLSRCH1");*/
			}
		}
		else
		{
			if(a->tick_vision<5000/14)
				a->tick_vision += g_engine->game_speed*g_engine->GetDeltaFix();
			else
			{
				player_visible = g_engine->coll_manager->raycasting(a,player);
				if(!player_visible)
				{
					a->tick_vision = 0;
					a->setState(0);//idle
				}
			}
		}
	}
	else
	{
		a->setState(0);//idle
	}

	if (a->getState()==1)//attack run
	{
		if (player->cx()<a->cx()) TurnLeft(a);			
		else TurnRight(a);

		if (a->flip_h)
			a->setVelocityX(-this->velocity_run);
		else
			a->setVelocityX(this->velocity_run);

		if (abs(player->cx()-a->cx()) <= player->w()+a->hw())//stand still
		{
			//PUNCH
			double timer = 22;
			if (a->fire_tick<timer) a->fire_tick += g_engine->game_speed*g_engine->GetDeltaFix();
			else
			{
				if(a->getID()==GNAAR_MALE) player->energy -= 2;
				else  player->energy -= 3;
				a->fire_tick = 0;
			}

			a->setFrameTimer_ms(this->gnaar_models[model_id]->animation_stand.timer);
			a->setFramesTotal(this->gnaar_models[model_id]->animation_stand.totalframes);
			//a->setFrameCurrent(0);//bez ovoga se rusi !!!!!
			//if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			a->animate();
			a->setVelocityX(0);
			this->gnaar_models[model_id]->animation_stand.setRotation(a);
		}
		else
		{
			a->setFrameTimer_ms(this->gnaar_models[model_id]->animation_run.timer);
			a->setFramesTotal(this->gnaar_models[model_id]->animation_run.totalframes);
			a->animate();
			this->gnaar_models[model_id]->animation_run.setRotation(a);
		}
		
		this->UpdateSkeleton_lite(a);
	}
	else//idle walk
	{
		a->setFrameTimer_ms(this->gnaar_models[model_id]->animation_walk.timer);
		a->setFramesTotal(this->gnaar_models[model_id]->animation_walk.totalframes);
		a->animate();
		
		if(a->flip_h)
			a->setVelocityX(-this->velocity_walk);
		else
			a->setVelocityX(this->velocity_walk);

		this->gnaar_models[model_id]->animation_walk.setRotation(a);
		this->UpdateSkeleton_lite(a);		
	}

}