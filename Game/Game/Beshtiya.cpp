#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Beshtiya.h"

BeshtiyaModel::BeshtiyaModel()
{
	this->skeleton = NULL;
}

bool BeshtiyaModel::Load(std::string root_directory, std::string name)
{
	this->root_directory = root_directory;
	this->name = name;	

	//LOAD ANIMATIONS
	if (!this->animation_fly.Load(this->root_directory + "fly.txt","fly")) return false;
	if (!this->animation_swim.Load(this->root_directory + "swim.txt","swim")) return false;
	if (!this->animation_attack.Load(this->root_directory + "attack.txt","attack")) return false;
	
	return true;

}
	
Beshtiya::Beshtiya(void)
{
	//misc
	this->energy_male = 2;
	this->energy_female = 4;

	this->velocity_walk = 1.5;
	this->velocity_run = 4.5;

}//Beshtiya()

bool Beshtiya::LoadTextures(void)
{
	/*BeshtiyaModel *temp_model = new BeshtiyaModel();
	if(temp_model->Load("Data/Textures/Enemies/Beshtiya/","name"))
	{
		//LOAD SKELETON
		temp_model->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/Beshtiya/beshtiya.txt");
		if(temp_model->skeleton==NULL) return false;
		this->beshtiya_models.push_back(temp_model);
	}
	else
		return false;*/

	BeshtiyaModel *temp_model1 = new BeshtiyaModel();
	if(temp_model1->Load("Data/Textures/Enemies/flyer/","name"))
	{
		//LOAD SKELETON
		temp_model1->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/flyer/flyer.txt");
		if(temp_model1->skeleton==NULL) return false;
		this->beshtiya_models.push_back(temp_model1);
	}
	else
		return false;

	BeshtiyaModel *temp_model2 = new BeshtiyaModel();
	if(temp_model2->Load("Data/Textures/Enemies/flipper/","name"))
	{
		//LOAD SKELETON
		temp_model2->skeleton = g_engine->skeleton_manager.GetSkeleton("Data/Textures/Enemies/flipper/flipper.txt");
		if(temp_model2->skeleton==NULL) return false;
		this->beshtiya_models.push_back(temp_model2);
	}
	else
		return false;

	this->laser_beam = g_engine->texture_manager->findTexture("Data/Textures/Weapons/bullet.tga");
	if (this->laser_beam==NULL) return false;

	return true;

}//LoadTextures()

Sprite* Beshtiya::addBeshtiya(double x, double y, int id,double scale_x,double scale_y,int energy,int difficulty,std::string command)
{
	Sprite *beshtiya = new Sprite();
	beshtiya->setClass(ENEMY);
	beshtiya->setType(BESHTIYA);
	beshtiya->setID(id);
	beshtiya->setState(0);//idle
	beshtiya->setPosition(x,y);
	beshtiya->rel_posy = y;
	beshtiya->setColor_Box(0xFFFF0000);
	beshtiya->setAnimationControl(MANUAL);
	beshtiya->difficulty = difficulty;
	//beshtiya->setScale(scale_x,scale_y);//ne moze ovo tu poremeti koliziju i sve ostalo !?!?
	g_engine->addSprite(beshtiya,ENEMIES);

	if(energy <= 0)
	{
		/*if(id==BESHTIYA) beshtiya->energy = 1000;
		else */
		if(id==FLIPPER) beshtiya->energy = 6;
		else if(id==FLYER) beshtiya->energy = 10;		
	}
	else
	{
		if(id==FLIPPER && energy == 10) beshtiya->energy = 6;//ne mogu sada sve minjat
		else
			beshtiya->energy = energy;	
	}

	this->addSkeleton(beshtiya,scale_x,scale_y);	

	g_engine->coll_manager->setColl_box(beshtiya);
	this->Update(beshtiya,NULL,NULL);

	beshtiya->setText(command);
	return beshtiya;

}//addBeshtiyaale()

void Beshtiya::addSkeleton(Sprite *a,double scale_x,double scale_y)
{
	a->frame_tick[0] = 0;

	/*if(a->getID()==BESHTIYA)
	{
		this->beshtiya_models[0]->skeleton->setSkeleton(a);
		a->setFrameTimer_ms(this->beshtiya_models[0]->animation_fly.timer);
		a->setFramesTotal(this->beshtiya_models[0]->animation_fly.totalframes);

		g_engine->skeleton_manager.Scale(a->skeleton, scale_x, scale_y);
		if(a->getBone("$TORSO")) a->setSize(a->getBone("$TORSO")->w(),a->getBone("$TORSO")->h());		
	}
	else */
	if(a->getID()==FLIPPER)
	{
		this->beshtiya_models[1]->skeleton->setSkeleton(a);
		a->setFrameTimer_ms(this->beshtiya_models[1]->animation_fly.timer);
		a->setFramesTotal(this->beshtiya_models[1]->animation_fly.totalframes);

		g_engine->skeleton_manager.Scale(a->skeleton, scale_x, scale_y);
		if(a->getBone("$HEAD1") && a->getBone("$HEAD2") && a->getBone("$TAIL0") && a->getBone("$TAIL1"))
			a->setSize(
			a->getBone("$HEAD1")->w() + a->getBone("$TAIL0")->w() + a->getBone("$TAIL1")->w(),
			a->getBone("$HEAD1")->h() + a->getBone("$HEAD2")->h());		
	}
	else//flyer
	{
		this->beshtiya_models[0]->skeleton->setSkeleton(a);
		a->setFrameTimer_ms(this->beshtiya_models[0]->animation_fly.timer);
		a->setFramesTotal(this->beshtiya_models[0]->animation_fly.totalframes);

		g_engine->skeleton_manager.Scale(a->skeleton, scale_x, scale_y);
		if(a->getBone("$TORSO")) a->setSize(a->getBone("$TORSO")->w(),a->getBone("$TORSO")->h());		
	}

	//a->setSize(100,100);
	//a->setSize(a->getBone("$TORSO")->getWidth(),a->getBone("$TORSO")->getHeight());

}//addSkeleton()

void Beshtiya::HitPlatform(Sprite *b, int direction, Sprite *a)
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

void Beshtiya::TurnLeft(Sprite *a)
{
	a->FlipAll_H(true);
}
void Beshtiya::TurnRight(Sprite *a)
{
	a->FlipAll_H(false);
}

void FlyerConnectBones(Sprite *a)
{
	for(unsigned int i=0;i<a->skeleton.size();i++)
	{
		if(!a->skeleton[i]->parent)
		{
			if(!a->flip_h) a->skeleton[i]->setPosition(a->getX()+a->skeleton[i]->getRotateX_real(),a->getY()+a->skeleton[i]->getRotateY_real());
			else a->skeleton[i]->setPosition(a->getX()+a->w()-a->skeleton[i]->getRotateX_real(),a->getY()+a->skeleton[i]->getRotateY_real());
		}
		else
		{
			a->skeleton[i]->parent->attach(a->skeleton[i]->parent_att_dot,a->skeleton[i]);
		}
	}

}//ConnectBones()

void FlipperConnectBones(Sprite *a)
{
	for(unsigned int i=0;i<a->skeleton.size();i++)
	{
		if(!a->skeleton[i]->parent)
		{
			//if(!a->flip_h)
				a->skeleton[i]->setPosition(a->cx(),a->getY()+a->skeleton[i]->hw());
			//else a->skeleton[i]->setPosition(a->getX()+a->w()-a->skeleton[i]->getRotateX_real(),a->getY()+a->skeleton[i]->getRotateY_real());
		}
		else
		{
			a->skeleton[i]->parent->attach(a->skeleton[i]->parent_att_dot,a->skeleton[i]);
		}
	}

}//ConnectBones()

void Beshtiya::Update(Sprite *a, Sprite *player1, Sprite *player2)
{
	if(player1!=NULL) if(a->map_current!=player1->map_current) player1 = NULL;
	if(player2!=NULL) if(a->map_current!=player2->map_current) player2 = NULL;

	this->UpdateSkeleton(a,player1,player2);

	bool player_visible = false;

	Sprite *player = NULL;
	
	if(a->getID()==FLYER)
	{
		if(player1 != NULL || player2 != NULL)
		{
			if(a->getState()==0)//search target
			{
				if(player1!=NULL)
				{
					if(g_engine->math->Distance(a->cx(),a->cy(),player1->cx(),player1->cy())<800)
						player_visible = g_engine->coll_manager->raycasting(a,player1);
					a->target = 0;
				}
				
				if(player2!=NULL && player_visible == false)
				{
					if(g_engine->math->Distance(a->cx(),a->cy(),player2->cx(),player2->cy())<800)
						player_visible = g_engine->coll_manager->raycasting(a,player2);
					a->target = 1;
				}				

				if(player_visible)				
				{
					a->setState(1);//attack
					g_engine->audio->Play("Flysght1");
				}
			}

			if(a->target==0 && player1!=NULL) player = player1;
			if(a->target==1 && player2!=NULL) player = player2;

			if(a->getState()==1 && player)//lock on target
			{
				//move
				if(g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())>250)
				a->setVelocity(
				g_engine->math->LinearVelocityX(g_engine->math->toDegrees(g_engine->math->AngleToTarget(a->cx(),a->cy(),player->cx(),player->cy())))*2.5,
				g_engine->math->LinearVelocityY(g_engine->math->toDegrees(g_engine->math->AngleToTarget(a->cx(),a->cy(),player->cx(),player->cy())))*2.5);

				if(a->tick_vision<2000/14)
					a->tick_vision += g_engine->game_speed*g_engine->GetDeltaFix();
				else
				{
					player_visible = g_engine->coll_manager->raycasting_all(a,player);
					if(player_visible)
					{
						a->setMovementState(0);					
						a->tick_vision = 0;
						a->setState(2);//idle
					}
				}
			}
			else if(a->getState()==2)//fire at target 8
			{
				a->setVelocity(0,0);

				if(a->fire_tick<100/14)
					a->fire_tick += g_engine->game_speed*(g_engine->GetDeltaFix());
				else
				{
					if((int)a->tick_vision%2==0)
						this->FireFlyer(a->getBone("$GUN1")->getAtt_posX(0),a->getBone("$GUN1")->getAtt_posY(0),a->getBone("$GUN1")->getRotation());
					else
						this->FireFlyer(a->getBone("$GUN2")->getAtt_posX(0),a->getBone("$GUN2")->getAtt_posY(0),a->getBone("$GUN2")->getRotation());

					g_engine->audio->Play("Flyatck3");
					a->fire_tick = 0;
					a->tick_vision += 1;
					if(a->tick_vision>=8) a->setState(1);
				}			
			}
		}
		else
		{
			a->setState(0);//idle
			a->target = -1;
		}
	}
	else if(a->getID()==FLIPPER)
	{
		if(a->getState()==0)//search target
		{
			if(player1 != NULL || player2 != NULL)
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
			}

			if(a->target==0 && player1!=NULL) player = player1;
			if(a->target==1 && player2!=NULL) player = player2;

			if(player_visible)				
			{
				a->setState(1);//attack
				g_engine->audio->Play("Flpsght1");
			}
		}
	}

	if(a->target==0 && player1!=NULL) player = player1;
	if(a->target==1 && player2!=NULL) player = player2;

	if(a->getState()==1 && player)
	{
		if(a->getID()==FLIPPER)
		{
			if(player)
			{
				if (player->cx()<a->cx()) a->FlipAll_H(true);
				else  a->FlipAll_H(false);

				if(g_engine->math->Distance(a->cx(),a->cy(),player->cx(),player->cy())>50)
				{
					a->setVelocity(
						g_engine->math->LinearVelocityX(g_engine->math->toDegrees(g_engine->math->AngleToTarget(a->cx(),a->cy(),player->cx(),player->cy())))*2,
						g_engine->math->LinearVelocityY(g_engine->math->toDegrees(g_engine->math->AngleToTarget(a->cx(),a->cy(),player->cx(),player->cy())))*2);

					a->setFrameTimer(this->beshtiya_models[1]->animation_swim.timer);
					this->beshtiya_models[1]->animation_swim.setRotation(a);
					FlipperConnectBones(a);
				}
				else
				{
					if(a->getFrameCurrent()==0)
					{
						if(rand()%6==0) g_engine->audio->Play("Flpatck1");
						player->energy -= 1;
					}
					a->setFrameTimer(this->beshtiya_models[1]->animation_attack.timer);
					this->beshtiya_models[1]->animation_attack.setRotation(a);
					FlipperConnectBones(a);
				}
			}			
		}			
	}
	
	if(!player || a->getState()==0)
	{
		if(a->getID()==FLYER)
		{
			a->setVelocity(0,0);
		}
		else if(a->getID()==FLIPPER)
		{
			a->setVelocity(0,0);
			this->beshtiya_models[1]->animation_swim.setRotation(a);
			FlipperConnectBones(a);				
		}
		
		a->setState(0);
		a->target = -1;
	}

	a->animate();	

	if(a->getID()==FLYER)
	{
		if(player)
		{
			a->setRotation(g_engine->math->AngleToTarget(a->cx(),a->cy(),player->cx(),player->cy()));
			for(unsigned int i=0;i<a->skeleton.size();i++)
			{
				a->skeleton[i]->setRotation(g_engine->math->AngleToTarget(a->cx(),a->cy(),player->cx(),player->cy()));
			}
		}
		FlyerConnectBones(a);
	}	

}//Update()

void Beshtiya::UpdateSkeleton(Sprite *a, Sprite *player1, Sprite *player2)
{
	if (a->skeleton.empty()) return;
	
}

void Beshtiya::FireFlyer(float x, float y, float angle)
{
	Sprite *red_ball = new Sprite();
	red_ball->setType(BIOMECH_LASER);	
	red_ball->setImage(this->laser_beam);
	red_ball->energy = 1;
	red_ball->setText("orangetrail");

	red_ball->setVelocity(
		g_engine->math->LinearVelocityX(g_engine->math->toDegrees(angle))*12,
		g_engine->math->LinearVelocityY(g_engine->math->toDegrees(angle))*12);

	red_ball->setRotation(angle);
	red_ball->setPosition(x,y);
	red_ball->setRotationAxis(red_ball->hw(),red_ball->hh());	
	red_ball->setColor_Texture(D3DCOLOR_ARGB(255,255,192,0));

	g_engine->coll_manager->setColl_line3(red_ball);
	g_engine->addSprite(red_ball,PARTICLES_COLL);
}