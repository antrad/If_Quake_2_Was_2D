#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Player.h"
#include <cmath>

//////////////
//  MODELS  //
//////////////

PlayerModel::PlayerModel()
{
	this->skeleton = NULL;	
}

bool PlayerModel::Load(std::string root_directory, std::string name)
{
	this->root_directory = root_directory;
	this->name = name;

	//LOAD ANIMATIONS
	if (!this->animation_run.Load(this->root_directory + "run.txt","run")) return false;
	if (!this->animation_stand.Load(this->root_directory + "stand.txt","stand")) return false;
	if (!this->animation_jump.Load(this->root_directory + "jump.txt","jump")) return false;
	if (!this->animation_kneel.Load(this->root_directory + "kneel.txt","kneel")) return false;
	if (!this->animation_walkcrouch.Load(this->root_directory + "walk_crouch.txt","walk_crouch")) return false;
	if (!this->animation_swim.Load(this->root_directory + "swim.txt","swim")) return false;
	if (!this->jump_crouch.Load(this->root_directory + "jump_crouch.txt","jump_crouch")) return false;

	if(!g_engine->audio->LoadSound(this->root_directory + "jump1.wav","#",false)) return false;
	if(!g_engine->audio->LoadSound(this->root_directory + "death1.wav","#",false)) return false;
	if(!g_engine->audio->LoadSound(this->root_directory + "death2.wav","#",false)) return false;
	if(!g_engine->audio->LoadSound(this->root_directory + "death3.wav","#",false)) return false;
	if(!g_engine->audio->LoadSound(this->root_directory + "death4.wav","#",false)) return false;

	//LOAD SKELETON
	this->skeleton = new Skeleton();
	if (!this->skeleton->Load_BoneData(this->root_directory + "info.txt")) return false;	

	return true;

}

Animation PlayerModel::getAnimation(std::string name)
{
	/*g_engine->audio->Play("jump1");
	g_engine->log_text.push_back(name);
	g_engine->save_log();*/

	if(name=="run") return this->animation_run;
	else if(name=="stand") return this->animation_stand;
	else if(name=="jump") return this->animation_jump;
	else if(name=="kneel") return this->animation_kneel;
	else if(name=="walk_crouch") return this->animation_walkcrouch;
	else if(name=="swim") return this->animation_swim;
	else if(name=="jump_crouch") return this->jump_crouch;
}

bool Player::LoadModels()
{
	bool directory = true;
	int i = 1;

	while(directory)
	{
		std::string temp_filename = "Data/Textures/Players/Player" + g_engine->inttostring(i) + "/info.txt";
		std::ifstream filein(temp_filename.c_str());
		if(filein.is_open())
		{
			PlayerModel *temp_model = new PlayerModel();
			if(temp_model->Load("Data/Textures/Players/Player" + g_engine->inttostring(i) + "/","name"))
			{
				this->player_models.push_back(temp_model);
				g_engine->skeleton_manager.skeletons.push_back(temp_model->skeleton);
			}
			else
			{
				g_engine->log_text.push_back("ERROR#PLAYER MODEL - Error loading player model Data/Textures/Players/Player" + g_engine->inttostring(i));
				return false;
			}
			i++;
			filein.close();
		}
		else
		{
			g_engine->log_text.push_back("PLAYER MODEL - No skeleton file in \"Data/Textures/Players/Player" + g_engine->inttostring(i) + "\"");
			directory = false;
		}

		g_engine->save_log();
	}

	return true;
}

//////////////
//  PLAYER  //
//////////////

Player::Player(std::string root_directory, int id)
{	
	this->kneel = false;
	this->god = false;
	this->root_directory = root_directory;
	this->player_control = KEYBOARD_MOUSE;
	this->player_id = id;
	this->active = false;
	this->model_id = id;
	this->change_weapon = false;
	this->in_lift = false;
	this->draw_inventory = false;
	this->nocontrol = false;

	this->map_id = 0;
	this->armor = 0;
	this->control_action_prev = false;
	this->control_action = false;

	rotation = 0;
	player_speed = 3.5;
	speed_crouch = 2;
	player_fire = false;

	p_player = NULL;
	weapons = NULL;
	this->hud = NULL;
	this->weapon_manager = NULL;	

	light = NULL;
	bulletflame = NULL;
	sacma = NULL;
	shell_shotgun = NULL;
	Bullet = NULL;
	raketah = NULL;
	Granata = NULL;
	cahura = NULL;
	flamethrower_flame = NULL;
	flamethrower_flame_light = NULL;
	cannon_ball = NULL;
	laser_beam = NULL;
	laser_beam_light = NULL;
	laser_beam2 = NULL;
	bfg_ball = NULL;	
	minigun_trail = NULL;
	minigun_trail_light = NULL;

	//controls
	this->LEFT = DIK_A;
	this->RIGHT = DIK_D;
	this->UP = DIK_W;
	this->DOWN = DIK_S;
	this->JUMP = DIK_SPACE;
	this->FIRE1 = 0;
	this->FIRE2 = -1;
	this->C_FLY = DIK_LCONTROL;
	this->BULLETTIME = DIK_LSHIFT;
	this->PREV_WEAPON = -1200;
	this->NEXT_WEAPON = 1200;
	this->WEAPON0 = DIK_1;
	this->WEAPON1 = DIK_2;
	this->WEAPON2 = DIK_3;
	this->WEAPON3 = DIK_4;
	this->WEAPON4 = DIK_5;
	this->WEAPON5 = DIK_6;
	this->WEAPON6 = DIK_7;
	this->WEAPON7 = DIK_8;
	this->WEAPON8 = DIK_9;
	this->WEAPON9 = DIK_0;

	this->screen = new Screen();

}//Player()

void Player::screen_create(int x,int y,int w,int h)
{
	this->screen->Init(x,y,w,h);	
	this->hud->setHUDpositions(x,y,w,h);
}

bool Player::LoadTextures()
{
	////////////
	//  MISC  //
	////////////
	if(!this->LoadModels()) return false;

	this->hud = new HUD_Game();
	if (!this->hud->load_textures()) return false;

	this->hud->HUD_weapons();

	///////////////
	//  WEAPONS  //
	///////////////
	this->weapon_manager = new AR_WeaponManager();
	if (!this->weapon_manager->Load_WeaponSettings(this->root_directory + "weapon_settings.txt")) return false;

	weapons = g_engine->texture_manager->findTexture("Data/Textures/Weapons/weapons.png");
	if (weapons==NULL){g_engine->message("\"Data/Textures/Weapons/weapons.png\" not preloaded.");return false;}	

	//PROJECTILES

	//light = g_engine->texture_manager->findTexture("flashlight.png");
	//if (light==NULL){g_engine->message("\"flashlight.png\" not preloaded.");return false;}

	shell_shotgun = g_engine->texture_manager->findTexture("Data/Textures/Weapons/shell_shotgun.tga");
	if (shell_shotgun==NULL){g_engine->message("\"Data/Textures/Weapons/shell_shotgun.tga\" not preloaded.");return false;}

	laser_beam = g_engine->texture_manager->findTexture("Data/Textures/Weapons/laser_beam.tga");
	if (laser_beam==NULL){g_engine->message("\"Data/Textures/Weapons/laser_beam.tga\" not preloaded.");return false;}

	laser_beam_light = g_engine->texture_manager->findTexture("Data/Textures/Weapons/laser_beam_light.tga");
	if (laser_beam_light==NULL){g_engine->message("\"Data/Textures/Weapons/laser_beam_light.tga\" not preloaded.");return false;}

	laser_beam2 = g_engine->texture_manager->findTexture("Data/Textures/Weapons/laser_beam2.tga");
	if (laser_beam2==NULL){g_engine->message("\"Data/Textures/Weapons/laser_beam2.tga\" not preloaded.");return false;}

	cannon_ball = g_engine->texture_manager->findTexture("Data/Textures/Weapons/cannon_ball.tga");
	if (cannon_ball==NULL){g_engine->message("\"Data/Textures/Weapons/cannon_ball.tga\" not preloaded.");return false;}

	flamethrower_flame = g_engine->texture_manager->findTexture("Data/Textures/Weapons/flamethrower_flame.tga");
	if (flamethrower_flame==NULL){g_engine->message("\"Data/Textures/Weapons/flamethrower_flame.tga\" not preloaded.");return false;}

	flamethrower_flame_light = g_engine->texture_manager->findTexture("Data/Textures/Weapons/flamethrower_flame_light.tga");
	if (flamethrower_flame_light==NULL){g_engine->message("\"Data/Textures/Weapons/flamethrower_flame_light.tga\" not preloaded.");return false;}

	bulletflame = g_engine->texture_manager->findTexture("Data/Textures/Weapons/bulletflame.tga");
	if (bulletflame==NULL){g_engine->message("\"Data/Textures/Weapons/bulletflame.tga\" not preloaded.");return false;}

	sacma = g_engine->texture_manager->findTexture("Data/Textures/Weapons/sacma.tga");
	if (sacma==NULL){g_engine->message("\"Data/Textures/Weapons/sacma.tga\" not preloaded.");return false;}

	Bullet = g_engine->texture_manager->findTexture("Data/Textures/Weapons/bullet.tga");
	if (Bullet==NULL){g_engine->message("\"Data/Textures/Weapons/bullet.tga\" not preloaded.");return false;}

	raketah = g_engine->texture_manager->findTexture("Data/Textures/Weapons/raketah.tga");
	if (raketah==NULL){g_engine->message("\"Data/Textures/Weapons/raketah.tga\" not preloaded.");return false;}

	Granata = g_engine->texture_manager->findTexture("Data/Textures/Weapons/Granata.tga");
	if (Granata==NULL){g_engine->message("\"Data/Textures/Weapons/Granata.tga\" not preloaded.");return false;}

	cahura = g_engine->texture_manager->findTexture("Data/Textures/Weapons/cahura.tga");
	if (cahura==NULL){g_engine->message("\"Data/Textures/Weapons/cahura.tga\" not preloaded.");return false;}

	bfg_ball = g_engine->texture_manager->findTexture("Data/Textures/Weapons/bfg_ball.tga");
	if (bfg_ball==NULL){g_engine->message("\"Data/Textures/Weapons/bfg_ball.tga\" not preloaded.");return false;}	

	minigun_trail = g_engine->texture_manager->findTexture("Data/Textures/Weapons/minigun_trail.png");
	if (minigun_trail==NULL){g_engine->message("\"Data/Textures/Weapons/minigun_trail.png\" not preloaded.");return false;}	

	minigun_trail_light = g_engine->texture_manager->findTexture("Data/Textures/Weapons/minigun_trail_light.png");
	if (minigun_trail_light==NULL){g_engine->message("\"Data/Textures/Weapons/minigun_trail_light.png\" not preloaded.");return false;}	

	return true;

}//LoadTextures

Sprite* Player::addPlayer(double x,double y)
{
	/*this->flashlight = NULL;
	flashlight = new Sprite();
	flashlight->setImage(this->light);
	flashlight->setCollidable(false);
	flashlight->setScale(2,1.5);
	flashlight->setRotationAxis(50*2,100*1.5);
	g_engine->addSprite(flashlight,g_engine->game_sprites.size()-1);*/

	this->kneel = false;
	Sprite *temp_player = new Sprite();
	temp_player->setType(IGRAC_1);
	temp_player->setID(this->player_id);
	this->active = true;
	temp_player->movement_type = MOVE_MANUAL;
	temp_player->setAnimationControl(ANIMATE_MANUAL);	
	temp_player->setMovementState(FALL);
	temp_player->animation_name.clear();
	temp_player->animation_name = "jump";
	temp_player->setPosition(x,y);
	temp_player->setColor_Box(0xFFFF0000);
	g_engine->addSprite( temp_player,PLAYER );
	this->map_id = 0;
	this->in_lift = false;

	this->addSkeleton(temp_player);	
	temp_player->getBone("$GUN2")->setVisible(false);
	temp_player->getBone("$GUN1")->setAtt_num(4);//zasto se ovdje nije srusilo prije nego sam doda ovo
	temp_player->getBone("$GUN2")->setAtt_num(4);

	//reset
	temp_player->energy = 100;
	this->armor = 0;
	this->player_weapon = 0;
	this->hud->weapon_group = 0;	
	for(int i=0;i<10;i++) hud->ammo_amount[i] = 0;
	for(int i=0;i<10;i++) hud->weapon_unlocked[i] = false;	
	hud->weapon_unlocked[0] = true;
	this->control_action_prev = false;
	this->control_action = false;
	this->god = false;
	this->draw_inventory = false;
	this->inventory.clear();
	this->inventory.push_back("blaster");
	//

	this->player_fire = false;	

	this->hud->selected_weapon->setFrameXY(
		this->hud->hud_weapons[this->player_weapon]->getFrameX(),
		this->hud->hud_weapons[this->player_weapon]->getFrameY());

	this->hud->hudweapons_tick = 0;
	this->hud->player_weapon = player_weapon;	

	this->time_lastshot = timeGetTime();
	this->change_weapon = false;

	g_engine->coll_manager->setColl_box(temp_player);	

	return temp_player;

}//addPlayer()

void Player::addSkeleton(Sprite *a)
{
	a->frame_tick[0] = 0;
	for(unsigned int i=0;i<a->skeleton.size();i++) a->skeleton[i]->setAlive(false);	

	this->player_models[model_id]->skeleton->setSkeleton(a);	

	//set size
	int width = 24;
	if (a->getBone("$TORSO")->h()>24) width = a->getBone("$TORSO")->h();

	a->setSize(width,
		(int)(a->getBone("$TORSO")->att_distance[0]+
		a->getBone("$LEG1_UP")->att_distance[0]+
		a->getBone("$LEG1_DOWN")->att_distance[0]));


	if(a->getBone("$HEAD")) a->setHeight((int)(a->getHeight()+a->getBone("$HEAD")->att_distance[0]));
	if(a->getBone("$FOOT1")) a->setHeight((int)(a->getHeight()+a->getBone("$FOOT1")->att_distance[0]));

}//addSkeleton()

bool SwimP(Sprite *test_me, int &h)
{
	std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_NON]->sprites.begin();
	while (iter != g_engine->game_maps[g_engine->map_current]->sprite_layers[WORLD_FRONT_NON]->sprites.end())
	{			
		Sprite *a = *iter;//temporary pointer			
		if ( a->getAlive() && a->getCollidable() && a->getID()==451)//is this entity in use?
		{
			if(g_engine->coll_manager->TestCollision_br(test_me,a))
			{
				//jebeni negativni y
				h = test_me->coll_box[2].getY() - a->coll_box[0].getY();
				return true;
			}
		}
		++iter;
	}

	return false;
}

void Player::UpdatePlayer()
{
	if (this->p_player==NULL || this->p_player->energy<=0 ) return;

	Sprite *a = this->p_player;	

	a->setY(a->getY()+a->getVelocityY()*g_engine->game_speed*g_engine->GetDeltaFix());	
	a->setX(a->getX()+a->getVelocityX()*g_engine->game_speed*g_engine->GetDeltaFix());

	int h;
	if(!SwimP(a,h))
	{
		//dynamic collbox (expect problems)
		float pos_y = a->getY()+a->h();

		if(a->getBone("$FOOT1"))
		{
			if(a->getBone("$FOOT1")->getAtt_posY(0) > a->getBone("$FOOT2")->getAtt_posY(0))
				a->setHeight((int)fabs(a->getBone("$TORSO")->getAtt_posY(0)-a->getBone("$FOOT1")->getAtt_posY(0)));
			else
				a->setHeight((int)fabs(a->getBone("$TORSO")->getAtt_posY(0)-a->getBone("$FOOT2")->getAtt_posY(0)));
		}
		else
		{
			if(a->getBone("$LEG1_DOWN")->getAtt_posY(0) > a->getBone("$LEG2_DOWN")->getAtt_posY(0))
				a->setHeight((int)fabs(a->getBone("$TORSO")->getAtt_posY(0)-a->getBone("$LEG1_DOWN")->getAtt_posY(0)));
			else
				a->setHeight((int)fabs(a->getBone("$TORSO")->getAtt_posY(0)-a->getBone("$LEG2_DOWN")->getAtt_posY(0)));
		}

		if(a->getBone("$HEAD")) a->setHeight((int)(a->getHeight()+a->getBone("$HEAD")->att_distance[0]));

		a->setY(pos_y-a->h());
	}
	else
	{
		//dynamic collbox (expect problems)
		float pos_y = a->getY()+a->h();
		a->setHeight(a->getBone("$TORSO")->w());		
		a->setY(pos_y-a->h());
	}

	//FIRE WEAPON
	if(!this->change_weapon)
	{
		if (a->fire_tick < 1000) a->fire_tick += g_engine->game_speed*g_engine->GetDeltaFix();
	}

} //UpdatePlayer

void Player::UpdateSkeleton()
{
	if (this->p_player==NULL) return;
	if (this->p_player->energy<=0) return;
	if (this->p_player->skeleton.empty()) return;

	if (this->player_control==JOYSTICK)
	{
		this->UpdateSkeleton_js();
		return;
	}

	Sprite *a = this->p_player;	

	if (g_engine->gui->cursor->cx()-g_engine->offset_X < a->cx())
		a->FlipAll_H(true);//left
	else
		a->FlipAll_H(false);//right

	a->animate();

	//ne ce sada ici niz skale
	//stavio sam ovdje jer zbog provjere kolizije (nakon inputa) ga iz kneel stavi u walk stanje
	if (this->p_player->getMovementState()==WALK && (this->kneel || !this->StandUp()))
	{
		this->p_player->setMovementState(KNEEL);		
	}

	int h;
	if(SwimP(a,h))
	{
		a->setFramesTotal(this->player_models[this->model_id]->animation_swim.totalframes);
		a->setFrameTimer(this->player_models[this->model_id]->animation_swim.timer);
		//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
		if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
		if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
		this->player_models[this->model_id]->animation_swim.setRotation(a);
	}
	else if(a->getMovementState()==WALK)
	{
		if(a->getVelocityX()!=0)
		{
			a->setFramesTotal(this->player_models[this->model_id]->animation_run.totalframes);
			a->setFrameTimer(this->player_models[this->model_id]->animation_run.timer);
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			this->player_models[this->model_id]->animation_run.setRotation(a);
		}
		else
		{
			a->setFrameCurrent(0);
			a->setFramesTotal(1);
			a->setFrameTimer(0);
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			this->player_models[this->model_id]->animation_stand.setRotation(a);
		}
	}
	else if(a->getMovementState()==KNEEL)
	{
		if(a->getVelocityX()==0)
		{
			a->setFrameCurrent(0);
			a->setFramesTotal(1);
			a->setFrameTimer(0);
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			this->player_models[this->model_id]->animation_kneel.setRotation(a);
		}
		else
		{
			a->setFramesTotal(this->player_models[this->model_id]->animation_walkcrouch.totalframes);
			a->setFrameTimer(this->player_models[this->model_id]->animation_walkcrouch.timer);
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			this->player_models[this->model_id]->animation_walkcrouch.setRotation(a);
		}
	}
	else//jump
	{
		a->setFrameCurrent(0);
		a->setFramesTotal(1);
		a->setFrameTimer(0);
		if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
		if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
		this->player_models[this->model_id]->getAnimation(a->animation_name).setRotation(a);
	}

	//rotate head(if there)
	Sprite *temp = a->getBone("$HEAD");
	if(temp)
		temp->setRotation(
		g_engine->math->AngleToTarget(
		temp->getX(),temp->getY(),
		g_engine->gui->cursor->cx()-g_engine->offset_X,g_engine->gui->cursor->cy()-g_engine->offset_Y));	

	if(a->getBone("$HEAD"))
	{
		float angle =  a->getBone("$HEAD")->getRotation();
		if(!a->flip_h && angle<-0.7) angle = -0.7;
		if(!a->flip_h && angle>0.7) angle = 0.7;
		if(a->flip_h && angle>-2.44 && angle<0) angle = -2.44;
		if(a->flip_h && angle<2.44 && angle>0) angle = 2.44;
		a->getBone("$HEAD")->setRotation(angle);
	}

	g_engine->skeleton_manager.Walk_Bipedal(a);
	this->HoldWeapon(a);

	//this->flashlight->flip_h = a->getBone("$HEAD")->flip_h;
	//this->flashlight->setRotation(a->getBone("$HEAD")->getRotation());
	//a->getBone("$HEAD")->attach(0,this->flashlight);

}//UpdateSkeleton()

void Player::UpdateSkeleton_js()
{
	Sprite *a = this->p_player;

	a->FlipAll_H(a->flip_h);
	a->animate();

	//ne ce sada ici niz skale
	//stavio sam ovdje jer zbog provjere kolizije (nakon inputa) ga iz kneel stavi u walk stanje
	if (this->p_player->getMovementState()==WALK && (this->kneel || !this->StandUp()))
	{
		this->p_player->setMovementState(KNEEL);		
	}	

	int h;
	if(SwimP(a,h))
	{
		a->setFramesTotal(this->player_models[this->model_id]->animation_swim.totalframes);
		a->setFrameTimer(this->player_models[this->model_id]->animation_swim.timer);
		//da se ne srusi, ako ne prijedje tick nece se ni osvjezit ovaj frame
		if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
		if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
		this->player_models[this->model_id]->animation_swim.setRotation(a);
	}
	else if(a->getMovementState()==WALK)
	{
		if(a->getVelocityX()!=0)
		{
			a->setFramesTotal(this->player_models[this->model_id]->animation_run.totalframes);
			a->setFrameTimer(this->player_models[this->model_id]->animation_run.timer);
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			this->player_models[this->model_id]->animation_run.setRotation(a);
		}
		else
		{
			a->setFrameCurrent(0);
			a->setFramesTotal(1);
			a->setFrameTimer(0);
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			this->player_models[this->model_id]->animation_stand.setRotation(a);
		}
	}
	else if(a->getMovementState()==KNEEL)
	{
		if(a->getVelocityX()==0)
		{
			a->setFrameCurrent(0);
			a->setFramesTotal(1);
			a->setFrameTimer(0);
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			this->player_models[this->model_id]->animation_kneel.setRotation(a);
		}
		else
		{
			a->setFramesTotal(this->player_models[this->model_id]->animation_walkcrouch.totalframes);
			a->setFrameTimer(this->player_models[this->model_id]->animation_walkcrouch.timer);
			if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
			if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
			this->player_models[this->model_id]->animation_walkcrouch.setRotation(a);
		}
	}
	else//jump
	{
		a->setFrameCurrent(0);
		a->setFramesTotal(1);
		a->setFrameTimer(0);
		if (a->getFrameCurrent() < 0) a->setFrameCurrent(a->getFramesTotal()-1);
		if (a->getFrameCurrent() > a->getFramesTotal()-1) a->setFrameCurrent(0);
		this->player_models[this->model_id]->getAnimation(a->animation_name).setRotation(a);
	}

	//rotate head(if there)
	Sprite *temp = a->getBone("$HEAD");
	if(temp) temp->setRotation(g_engine->math->toRadians(this->fire_angle));

	g_engine->skeleton_manager.Walk_Bipedal(a);
	this->HoldWeapon_js(a);	

}//UpdateSkeleton()

void Player::HoldWeapon(Sprite *a)
{
	if (this->player_control==JOYSTICK)
	{
		this->HoldWeapon_js(a);
		return;
	}

	int sign = 1;
	double pi = 0;
	if (a->flip_h)//left
	{
		sign=-1;pi=180;
	}
	else//right
	{
		sign=1;	pi=0;
	}

	double aa=1;
	double bb=1;
	double cc=1;
	double A=1;
	double B=1;
	double C=1;

	double angle = 0;

	if(this->change_weapon)
	{
		fire_angle -= 5*g_engine->game_speed*g_engine->GetDeltaFix();
		if(a->flip_h) angle = g_engine->math->toRadians(180-fire_angle);
		else angle = g_engine->math->toRadians(fire_angle);
		if(fire_angle<0) this->change_weapon = false;
	}

	if(!this->change_weapon)
	{
		//odvojio da ne zapuca prije nego se ruka stvarno postavi na misto
		angle = g_engine->math->AngleToTarget(
			a->getBone("$ARM1_UP")->getX(),
			a->getBone("$ARM1_UP")->getY(),
			g_engine->gui->cursor->cx()-g_engine->offset_X,
			g_engine->gui->cursor->cy()-g_engine->offset_Y
			);
	}

	if (rotation<0)
		rotation+=2*g_engine->game_speed*(g_engine->GetDeltaFix());
	else
		rotation=0;	

	if (this->player_weapon==BLASTER)
	{
		//aim
		if((DWORD)(timeGetTime() - this->time_lastshot) < 5000 || this->player_fire)
		{
			a->getBone("$ARM1_UP")->setRotation(angle-sign*g_engine->math->toRadians(rotation)*0.8);		
			a->getBone("$ARM1_DOWN")->setRotation(a->getBone("$ARM1_UP")->getRotation()+sign*g_engine->math->toRadians(rotation)*1.5);
		}
		a->getBone("$GUN1")->setRotation(a->getBone("$ARM1_DOWN")->getRotation());

		a->getBone("$ARM1_UP")->attach(0,a->getBone("$ARM1_DOWN"));
		a->getBone("$ARM1_DOWN")->attach(0,a->getBone("$GUN1"));
	}	
	else if (this->player_weapon==BFG || this->player_weapon==CHAINGUN )
	{
		a->getBone("$ARM1_UP")->setRotation(angle+sign*g_engine->math->toRadians(145)-sign*g_engine->math->toRadians(rotation)*0.8);

		if(!this->change_weapon)
		{
			angle = g_engine->math->AngleToTarget(
				a->getBone("$ARM1_DOWN")->getX(),
				a->getBone("$ARM1_DOWN")->getY(),
				g_engine->gui->cursor->cx()-g_engine->offset_X,
				g_engine->gui->cursor->cy()-g_engine->offset_Y
				);
		}

		a->getBone("$ARM1_DOWN")->setRotation(angle+sign*g_engine->math->toRadians(65)+sign*g_engine->math->toRadians(rotation)*0.8);
		a->getBone("$GUN1")->setRotation(angle+sign*g_engine->math->toRadians(0)+sign*g_engine->math->toRadians(rotation)*0.6);

		a->getBone("$ARM1_UP")->attach(0,a->getBone("$ARM1_DOWN"));
		a->getBone("$ARM1_DOWN")->attach(0,a->getBone("$GUN1"));

	}	
	else
	{
		a->getBone("$ARM1_UP")->setRotation(angle+sign*g_engine->math->toRadians(115)-sign*g_engine->math->toRadians(rotation)*1.4);

		if(!this->change_weapon)
		{
			angle = g_engine->math->AngleToTarget(
				a->getBone("$ARM1_DOWN")->getX(),
				a->getBone("$ARM1_DOWN")->getY(),
				g_engine->gui->cursor->cx()-g_engine->offset_X,
				g_engine->gui->cursor->cy()-g_engine->offset_Y
				);
		}

		a->getBone("$ARM1_DOWN")->setRotation(angle-sign*g_engine->math->toRadians(0)+sign*g_engine->math->toRadians(rotation)*0.6);	
		a->getBone("$GUN1")->setRotation(a->getBone("$ARM1_DOWN")->getRotation()+sign*g_engine->math->toRadians(rotation)*0.6);

		a->getBone("$ARM1_UP")->attach(0,a->getBone("$ARM1_DOWN"));
		a->getBone("$ARM1_DOWN")->attach(0,a->getBone("$GUN1"));
	}

	if (this->player_weapon!=BLASTER)//arm2
	{
		//g_engine->audio->Play("jump1");

		aa = a->getBone("$ARM2_UP")->att_distance[0];
		bb = a->getBone("$ARM2_DOWN")->att_distance[0];
		cc = g_engine->math->Distance(a->getBone("$ARM2_UP")->getX(),a->getBone("$ARM2_UP")->getY(),
			a->getBone("$GUN1")->getAtt_posX(2),a->getBone("$GUN1")->getAtt_posY(2));		

		if ((aa+bb)>cc)
		{
			B = std::acos( (aa*aa+cc*cc-bb*bb) / (2*aa*cc) );

			angle = g_engine->math->AngleToTarget(a->getBone("$ARM2_UP")->getX(),a->getBone("$ARM2_UP")->getY(),
				a->getBone("$GUN1")->getAtt_posX(2),a->getBone("$GUN1")->getAtt_posY(2));
			a->getBone("$ARM2_UP")->setRotation(angle+sign*B);

			angle = g_engine->math->AngleToTarget(a->getBone("$ARM2_UP")->getAtt_posX(0),a->getBone("$ARM2_UP")->getAtt_posY(0),
				a->getBone("$GUN1")->getAtt_posX(2),a->getBone("$GUN1")->getAtt_posY(2));			
			a->getBone("$ARM2_DOWN")->setRotation(angle);
		}
		else
		{
			angle = g_engine->math->AngleToTarget(a->getBone("$ARM2_UP")->getX(),a->getBone("$ARM2_UP")->getY(),
				a->getBone("$GUN1")->getAtt_posX(2),a->getBone("$GUN1")->getAtt_posY(2));
			a->getBone("$ARM2_UP")->setRotation(angle);
			a->getBone("$ARM2_DOWN")->setRotation(angle);
		}

		a->getBone("$ARM2_UP")->attach(0,a->getBone("$ARM2_DOWN"));
	}

}//HoldWeapon()

void Player::HoldWeapon_js(Sprite *a)
{
	int sign = 1;
	double pi = 0;
	if (a->flip_h)//left
	{
		sign=-1;pi=180;
	}
	else//right
	{
		sign=1;	pi=0;
	}

	double aa=1;
	double bb=1;
	double cc=1;
	double A=1;
	double B=1;
	double C=1;

	double angle = g_engine->math->toRadians(fire_angle);

	if(this->change_weapon)
	{
		fire_angle -= 5*g_engine->game_speed*g_engine->GetDeltaFix();
		if(a->flip_h) angle = g_engine->math->toRadians(180-fire_angle);
		else angle = g_engine->math->toRadians(fire_angle);
		if(fire_angle<0) this->change_weapon = false;
	}

	if (rotation<0)
		rotation+=2*g_engine->game_speed*(g_engine->GetDeltaFix());
	else
		rotation=0;	

	if (this->player_weapon==BLASTER)
	{		
		a->getBone("$ARM1_UP")->setRotation(angle-sign*g_engine->math->toRadians(rotation)*0.8);		
		a->getBone("$ARM1_DOWN")->setRotation(a->getBone("$ARM1_UP")->getRotation()+sign*g_engine->math->toRadians(rotation)*1.5);
		a->getBone("$GUN1")->setRotation(a->getBone("$ARM1_DOWN")->getRotation());

		a->getBone("$ARM1_UP")->attach(0,a->getBone("$ARM1_DOWN"));
		a->getBone("$ARM1_DOWN")->attach(0,a->getBone("$GUN1"));		
	}	
	else if (this->player_weapon==BFG || this->player_weapon==CHAINGUN )
	{
		a->getBone("$ARM1_UP")->setRotation(angle+sign*g_engine->math->toRadians(145)-sign*g_engine->math->toRadians(rotation)*0.8);

		a->getBone("$ARM1_DOWN")->setRotation(angle+sign*g_engine->math->toRadians(65)+sign*g_engine->math->toRadians(rotation)*0.8);
		a->getBone("$GUN1")->setRotation(angle+sign*g_engine->math->toRadians(0)+sign*g_engine->math->toRadians(rotation)*0.6);

		a->getBone("$ARM1_UP")->attach(0,a->getBone("$ARM1_DOWN"));
		a->getBone("$ARM1_DOWN")->attach(0,a->getBone("$GUN1"));

	}	
	else
	{
		a->getBone("$ARM1_UP")->setRotation(angle+sign*g_engine->math->toRadians(115)-sign*g_engine->math->toRadians(rotation)*1.4);

		a->getBone("$ARM1_DOWN")->setRotation(angle-sign*g_engine->math->toRadians(0)+sign*g_engine->math->toRadians(rotation)*0.6);	
		a->getBone("$GUN1")->setRotation(a->getBone("$ARM1_DOWN")->getRotation()+sign*g_engine->math->toRadians(rotation)*0.6);

		a->getBone("$ARM1_UP")->attach(0,a->getBone("$ARM1_DOWN"));
		a->getBone("$ARM1_DOWN")->attach(0,a->getBone("$GUN1"));
	}

	if (this->player_weapon!=BLASTER)//arm2
	{
		//pojavilo se crvenilo i nakon svega ovoga kad sam puca iz topa
		//-- jer je za top att_pos nedefiniran, javi se samo ako isprva ispalim top, mozda je to i prije bio problem ?		

		aa = a->getBone("$ARM2_UP")->att_distance[0];
		bb = a->getBone("$ARM2_DOWN")->att_distance[0];
		cc = g_engine->math->Distance(a->getBone("$ARM2_UP")->getX(),a->getBone("$ARM2_UP")->getY(),
			a->getBone("$GUN1")->getAtt_posX(2),a->getBone("$GUN1")->getAtt_posY(2));		

		if ((aa+bb)>cc)
		{
			B = std::acos( (aa*aa+cc*cc-bb*bb) / (2*aa*cc) );

			angle = g_engine->math->AngleToTarget(a->getBone("$ARM2_UP")->getX(),a->getBone("$ARM2_UP")->getY(),
				a->getBone("$GUN1")->getAtt_posX(2),a->getBone("$GUN1")->getAtt_posY(2));
			a->getBone("$ARM2_UP")->setRotation(angle+sign*B);

			angle = g_engine->math->AngleToTarget(a->getBone("$ARM2_UP")->getAtt_posX(0),a->getBone("$ARM2_UP")->getAtt_posY(0),
				a->getBone("$GUN1")->getAtt_posX(2),a->getBone("$GUN1")->getAtt_posY(2));			
			a->getBone("$ARM2_DOWN")->setRotation(angle);
		}
		else
		{
			angle = g_engine->math->AngleToTarget(a->getBone("$ARM2_UP")->getX(),a->getBone("$ARM2_UP")->getY(),
				a->getBone("$GUN1")->getAtt_posX(2),a->getBone("$GUN1")->getAtt_posY(2));
			a->getBone("$ARM2_UP")->setRotation(angle);
			a->getBone("$ARM2_DOWN")->setRotation(angle);
		}

		a->getBone("$ARM2_UP")->attach(0,a->getBone("$ARM2_DOWN"));
	}

}//HoldWeapon_js()

void Player::setWeaponTexture()
{
	if(this->p_player==NULL) return;

	Sprite *weapon = p_player->getBone("$GUN1");

	this->p_player->fire_tick = 200;

	switch (this->player_weapon)
	{
	case BLASTER:
		this->weapon_manager->Switch_WeaponTexture(weapon,"BLASTER");
		this->hud->weapon_group = 0;
		this->hud->ammo_current = 0;
		break;	

	case SHOTGUN:
		this->weapon_manager->Switch_WeaponTexture(weapon,"SHOTGUN");
		this->hud->weapon_group = 1;
		this->hud->ammo_current = 1;
		break;

	case S_SHOTGUN:
		this->weapon_manager->Switch_WeaponTexture(weapon,"S_SHOTGUN");
		this->hud->weapon_group = 2;
		this->hud->ammo_current = 1;
		break;

	case MACHINEGUN:
		this->weapon_manager->Switch_WeaponTexture(weapon,"MACHINEGUN");
		this->hud->weapon_group = 3;
		this->hud->ammo_current = 2;
		break;

	case CHAINGUN:
		this->weapon_manager->Switch_WeaponTexture(weapon,"CHAINGUN");
		this->hud->weapon_group = 4;
		this->hud->ammo_current = 2;
		break;	

	case G_LAUNCHER:
		this->weapon_manager->Switch_WeaponTexture(weapon,"G_LAUNCHER");
		this->hud->weapon_group = 5;
		this->hud->ammo_current = 3;
		break;	

	case R_LAUNCHER:
		this->weapon_manager->Switch_WeaponTexture(weapon,"R_LAUNCHER");
		this->hud->weapon_group = 6;
		this->hud->ammo_current = 4;
		break;

	case HYPERBLASTER:
		this->weapon_manager->Switch_WeaponTexture(weapon,"HYPERBLASTER");
		this->hud->weapon_group = 7;
		this->hud->ammo_current = 5;
		break;

	case RAILGUN:
		this->weapon_manager->Switch_WeaponTexture(weapon,"RAILGUN");
		this->hud->weapon_group = 8;
		this->hud->ammo_current = 6;
		break;	

	case BFG:
		this->weapon_manager->Switch_WeaponTexture(weapon,"BFG");
		this->hud->weapon_group = 9;
		this->hud->ammo_current = 7;
		break;

	} //switch(player_weapon)

	//change weapon icon
	this->hud->selected_weapon->setFrameXY(
		this->hud->hud_weapons[this->player_weapon]->getFrameX(),
		this->hud->hud_weapons[this->player_weapon]->getFrameY());

	//change ammo icon
	this->hud->ammo_s->setFrameXY(
		this->hud->hud_weapons[this->hud->ammo_current]->getFrameX(),
		this->hud->hud_weapons[this->hud->ammo_current]->getFrameY());

	this->hud->hudweapons_tick = 255;
	this->hud->player_weapon = player_weapon;
	this->change_weapon = true;
	this->fire_angle = 90;

}//setWeaponTexture()

//////////////////////////
////////////////////////// FIRE WEAPON
//////////////////////////

void Player::firebullet(int tip, int time, Sprite *IGRAC1)
{
	if (IGRAC1==NULL) return;
	if (g_engine->getGameState()!=PLAY) return;

	this->time_lastshot = timeGetTime();

	Sprite *weapon1 = IGRAC1->getBone("$GUN1");

	double velocity=0;
	double randomangle=0;
	double bulletangle;
	double randomangle2=1;

	if (rand()%2) randomangle2=-1;

	//add bullet
	Sprite *bullet = new Sprite();
	bullet->setClass(PLAYER_WEAPON);

	if (tip==S_SHOTGUN || tip==SHOTGUN)
		bullet->setLifetime(time);
	else if (tip!=G_LAUNCHER)
		bullet->setLifetime(3000);

	//bullet type
	if (tip == BLASTER)
	{
		bullet->setType(BULLET);
		bullet->setImage(Bullet);
		velocity = 15;//15		
		bullet->energy = 1;	
		bullet->setColor_Texture(D3DCOLOR_ARGB(255,255,192,0));
		g_engine->audio->Play("BLASTF1A");
	}
	else if (tip == SHOTGUN)
	{
		bullet->setType(SACMA);
		bullet->setImage(sacma);
		bullet->energy = 0.5;
		velocity=28+(rand()%30)*0.1;//29
		bullet->setWidth(25+(rand()%8));
		randomangle=rand()%5*randomangle2;
		rotation=-10;
	}
	else if (tip == S_SHOTGUN)
	{
		bullet->setType(SACMA);
		bullet->setImage(sacma);
		bullet->setState(666);
		bullet->energy = 0.5;
		velocity=28+(rand()%30)*0.1;//29
		bullet->setWidth(25+(rand()%8));
		randomangle=rand()%10*randomangle2;
		rotation=-20;
	}
	else if (tip == MACHINEGUN)
	{
		bullet->setType(BULLET);
		bullet->setImage(Bullet);
		bullet->energy=1;
		velocity=30;
		randomangle=rand()%3;
		this->hud->ammo_amount[2]--; 
		rotation=-5;
	}
	else if (tip == CHAINGUN)
	{
		bullet->setType(BULLET);
		bullet->setImage(Bullet);
		bullet->energy=1;
		velocity=32;
		randomangle=rand()%5;
		this->hud->ammo_amount[2]--; 
		rotation=-4;
	}
	else if (tip == G_LAUNCHER)
	{
		bullet->setType(GRANATA);
		bullet->elastic=0.5;//0.5
		bullet->setMovementState(FALL);
		bullet->setImage(Granata);
		bullet->setState(0);
		velocity=15;//16
		g_engine->audio->Play("GRENLF1A");
		bullet->energy=10;
		this->hud->ammo_amount[3]--;
		rotation=-10;
		bullet->setScale(0.25,0.25);
	}
	else if (tip == R_LAUNCHER)
	{
		bullet->setType(ROCKET);
		bullet->setImage(raketah);
		velocity = 10;
		randomangle=rand()%2;
		g_engine->audio->Play("ROCKLF1A");
		bullet->energy=10;
		this->hud->ammo_amount[4]--;		
	}	
	else if (tip == HYPERBLASTER)
	{
		bullet->setType(BULLET);
		bullet->setImage(laser_beam);
		bullet->energy=2.5;
		velocity=28;
		g_engine->audio->Play("Hyprbf1a");
		bullet->energy=2;
		this->hud->ammo_amount[5]--; 
		rotation=-4;
	}
	else if (tip == BFG)
	{
		bullet->setClass(-999);
		bullet->setType(BFGBALL);
		//bullet->setScale(1);
		bullet->setImage(bfg_ball);
		velocity=15;
		bullet->energy=100;
		this->hud->ammo_amount[7] -= 50; 
		rotation=-20;
	}

	if (tip == CHAINGUN || tip == MACHINEGUN)
	{
		switch(rand()%5)
		{
		case 0:
			g_engine->audio->Play("Machgf1b");
			break;
		case 1:
			g_engine->audio->Play("Machgf2b");
			break;
		case 2:
			g_engine->audio->Play("Machgf3b");
			break;
		case 3:
			g_engine->audio->Play("Machgf4b");
			break;
		case 4:
			g_engine->audio->Play("Machgf5b");
			break;
		}
	}

	bulletangle = g_engine->math->toDegrees(IGRAC1->getBone("$GUN1")->getRotation());	

	//L-V degrees
	bullet->setVelocity(
		g_engine->math->LinearVelocityX(bulletangle+randomangle)*(velocity),
		g_engine->math->LinearVelocityY(bulletangle+randomangle)*(velocity)
		);
	//R radians
	bullet->setRotation(g_engine->math->toRadians(bulletangle));

	if (this->player_weapon==G_LAUNCHER || this->player_weapon==BFG)
	{
		bullet->setRotation(g_engine->math->toRadians(rand()%360));
		bullet->setPosition(weapon1->getAtt_posX(0)-bullet->hw(),weapon1->getAtt_posY(0)-bullet->hh());
	}
	else
	{
		bullet->rotate=true;
		bullet->setRotationAxis(0,bullet->hh());//problemi zbog scale ???	
		bullet->setPosition(weapon1->getAtt_posX(0),weapon1->getAtt_posY(0));
	}

	bullet->setColor_Box(0xFF00FFFF);
	g_engine->addSprite(bullet,PARTICLES_COLL);

	//dot collision
	if (tip==G_LAUNCHER || tip==BFG)
	{
		g_engine->coll_manager->setColl_box(bullet);
		/*bullet->setRotationAxis(bullet->hw(),bullet->hh());
		bullet->setCollisionMethod(BOX_RXYC);*/
	}
	else
		g_engine->coll_manager->setColl_line3(bullet);

	addMuzzle(IGRAC1->getBone("$GUN1"),g_engine->math->toRadians(bulletangle));
	bullet->setID(tip);

	if(tip==R_LAUNCHER || tip==BLASTER || tip==HYPERBLASTER)
	{
		Sprite *light = new Sprite();
		light->setImage(flamethrower_flame_light);
		light->setCollidable(false);
		//light->oneframelife = true;
		light->setPosition(bullet->getX()-light->hw(),bullet->getY()-light->hh());
		light->setColor_Texture(0xFF00FF00);
		bullet->child = light;
		light->parent = bullet;
		g_engine->addSprite(light,SHADERSPRITES);
	}

	this->p_player->fire_tick = 0;	

}//firebullet()

void Player::addMuzzle( Sprite *weapon , double angle)
{
	Sprite *muzzle = new Sprite();
	muzzle->setType(OBJECT_EXP);
	muzzle->setImage(bulletflame);
	muzzle->setCollidable(false);
	muzzle->setRotation(angle);
	muzzle->oneframelife=true;	
	muzzle->flip_h = weapon->flip_h;
	muzzle->rotate = true;
	muzzle->setRotationAxis(0,muzzle->hh());//problemi zbog scale ???	
	muzzle->setColor_Box(0x88888888);
	g_engine->addSprite( muzzle,PARTICLES_NON );

	weapon->attach(0,muzzle);

	Sprite *light = new Sprite();
	light->setImage(flamethrower_flame_light);
	light->setCollidable(false);
	light->setRotation(angle);
	light->oneframelife = true;
	light->setPosition(muzzle->getX()-light->hw(),muzzle->getY()-light->hh());	
	light->setColor_Texture(0xFF00FF00);	
	g_engine->addSprite(light,SHADERSPRITES);

}//addMuzzle()

void Player::addGrenade( Sprite *player)
{
	if (player==NULL) return;
	if (g_engine->getGameState()!=PLAY) return;

	Sprite *s_grenade = new Sprite();
	s_grenade->setClass(PLAYER_WEAPON);
	s_grenade->setType(GRANATA);
	s_grenade->setImage(Granata);
	s_grenade->setScale(0.25,0.25);
	s_grenade->setPosition(player->cx(),player->getY());

	//LV degrees
	s_grenade->setVelocity(
		g_engine->math->LinearVelocityX(g_engine->math->toDegrees(player->getBone("$GUN1")->getRotation()))*10,
		g_engine->math->LinearVelocityY(g_engine->math->toDegrees(player->getBone("$GUN1")->getRotation()))*10
		);

	s_grenade->setMovementState(FALL);
	s_grenade->elastic = 0.5;
	s_grenade->energy = 5;
	s_grenade->setColor_Box(0xFF00FFFF);
	g_engine->addSprite( s_grenade,PARTICLES_COLL );
	g_engine->audio->Play("HGRENT1A");

	//dot collision
	g_engine->coll_manager->setColl_box(s_grenade);	

}//addGrenade()

void Player::FireWeapon()
{
	if(this->p_player==NULL) return;
	if(this->p_player->energy<=0) return;
	if(this->change_weapon) return;

	//g_engine->audio->Play("jump1");

	Sprite *weapon = this->p_player->getBone("$GUN1");

	if(this->weapon_manager->Check_FireTick(weapon->getText(),this->p_player->fire_tick))
	{
		if (this->player_weapon==BLASTER) 
		{
			this->firebullet(this->player_weapon,999,this->p_player);		
		}
		else if (this->player_weapon==SHOTGUN && this->hud->ammo_amount[1]>0) 
		{
			for(int i=0;i<6;i++) firebullet(this->player_weapon,200+(i%4)*100,this->p_player);
			g_engine->audio->Play("Shotgf1b");
			this->addShell(weapon);
			this->hud->ammo_amount[1]--;
			g_engine->audio->Play("Shotgr1b");
		}
		else if (this->player_weapon==S_SHOTGUN && this->hud->ammo_amount[1]>1)
		{
			for(int i=0;i<24;i++) firebullet(this->player_weapon,200+(i%4)*100,this->p_player);
			g_engine->audio->Play("Sshotf1b");
			this->addShell(weapon);
			this->addShell(weapon);
			this->hud->ammo_amount[1]--;
			this->hud->ammo_amount[1]--;
			g_engine->audio->Play("Sshotr1b");
		}
		else if (this->hud->ammo_amount[2]>0 && this->player_weapon==MACHINEGUN) 
		{
			this->firebullet(this->player_weapon,999,this->p_player);
			this->addShell(weapon);
		}
		else if (this->hud->ammo_amount[2]>0 && this->player_weapon==CHAINGUN)
		{
			this->fireminigun(this->p_player);
			this->hud->ammo_amount[2]--; 
			switch(rand()%5)
			{
			case 0:
				g_engine->audio->Play("Machgf1b");
				break;
			case 1:
				g_engine->audio->Play("Machgf2b");
				break;
			case 2:
				g_engine->audio->Play("Machgf3b");
				break;
			case 3:
				g_engine->audio->Play("Machgf4b");
				break;
			case 4:
				g_engine->audio->Play("Machgf5b");
				break;
			}			
		}
		else if (this->hud->ammo_amount[3]>0 && this->player_weapon==G_LAUNCHER)
			firebullet(this->player_weapon,999,this->p_player);
		else if (this->hud->ammo_amount[4]>0 && this->player_weapon==R_LAUNCHER)
			firebullet(this->player_weapon,999,this->p_player);	
		else if (this->hud->ammo_amount[5]>0 && this->player_weapon==HYPERBLASTER)
			this->firebullet(this->player_weapon,999,this->p_player);
		else if (this->hud->ammo_amount[6]>0 && this->player_weapon==RAILGUN)
		{
			this->firelaser(this->p_player);
			g_engine->audio->Play("RAILGF1A");		
			this->hud->ammo_amount[6]--; 
		}		
		else if (this->hud->ammo_amount[7]>0 && this->player_weapon==BFG)
			this->firebullet(this->player_weapon,999,this->p_player);
	}

}//FireWeapon()

void Player::addShell(Sprite *weapon)
{
	return;

	/*Sprite *shell = new Sprite();
	shell->setType(SHELL);

	if (player_weapon==2 || player_weapon==3) shell->setImage(shell_shotgun);
	else
	{
	shell->setImage(cahura);
	if (this->player_weapon==UZI) shell->setWidth(4);
	else if (this->player_weapon==MACHINEGUN) shell->setWidth(5);
	else if (this->player_weapon==CHAINGUN || this->player_weapon==SNIPER) shell->setWidth(6);
	}

	shell->setPosition(
	weapon->getAtt_posX(1)-shell->hw(),
	weapon->getAtt_posY(1)-shell->hh());	

	shell->setRotation(rand()%360);

	//leti u suprotnom smjeru od gledanja
	double angle;
	if (!weapon->flip_h) angle=135;
	else angle=45;
	angle = (rand()%((int)(angle+15)-(int)(angle-15)))+(angle-15);//random angle

	shell->setVelocity(
	g_engine->math->LinearVelocityX(-angle)*4,
	g_engine->math->LinearVelocityY(-angle)*4
	);

	shell->setMovementState(FALL);
	shell->elastic=0.35+(rand()%10*0.01);
	shell->setColor_Box(0xFFFFFF00);

	g_engine->coll_manager->setColl_center(shell);
	shell->setCollidable(false);
	g_engine->addSprite(shell,PARTICLES_COLL);

	g_engine->coll_manager->TestForCollisions_dot(shell,WORLD_FRONT_COLL);	
	*/
}//addShell

void Player::firelaser(Sprite *player)
{
	Sprite *weapon = player->getBone("$GUN1");
	double angle = weapon->getRotation();

	std::vector <Vector3> collision_dots;

	Sprite *laser = new Sprite();
	laser->setClass(PLAYER_WEAPON);
	laser->setType(BULLET);
	laser->setID(RAILGUN);
	laser->setImage(laser_beam2);
	laser->energy = 50;
	laser->setWidth(1500);
	laser->setRotation(angle);
	//onda ne radi odbacivanje dead
	//laser->setVelocity(
	//g_engine->math->LinearVelocityX(g_engine->math->toDegrees(angle))*100,
	//g_engine->math->LinearVelocityY(g_engine->math->toDegrees(angle))*100);//radi bullet hiit skeleton
	laser->setRotationAxis(0,laser->hh());
	laser->setText("railgun");
	weapon->attach(0,laser);
	g_engine->coll_manager->setColl_line3(laser);

	float coll_x;
	float coll_y;

	Sprite *temp_sprite = g_engine->coll_manager->collision_line_getclosest(
		laser->coll_box[2].getX(),laser->coll_box[2].getY(),
		laser->coll_box[0].getX(),laser->coll_box[0].getY(),
		WORLD_FRONT_COLL,
		coll_x,coll_y);

	if(temp_sprite!=NULL)
	{
		laser->setWidth((float)g_engine->math->Distance(laser->coll_box[2].getX(),laser->coll_box[2].getY(),coll_x,coll_y));
		g_engine->coll_manager->collbox_update(laser);

		if(g_engine->display_lineofsight)
		{
			Line *temp_line = new Line(
				laser->coll_box[2].getX(),laser->coll_box[2].getY(),			
				coll_x,coll_y,
				WORLD_FRONT_COLL,0xFF0000FF);
			g_engine->line_list.push_back(temp_line);
		}

		if(!game_spriteCollision(laser,temp_sprite)) game_spriteCollision(temp_sprite,laser);
	}

	laser->setAlive(true);
	laser->setCollidable(true);

	g_engine->coll_manager->collision_list_line_rect(
		laser,
		laser->coll_box[2].getX(),laser->coll_box[2].getY(),
		laser->coll_box[0].getX(),laser->coll_box[0].getY(),
		ENEMIES);

	//laser->setVelocity(0,0);
	laser->setAlive(true);
	laser->setLifetime(250);
	laser->setCollidable(false);
	laser->coll_box.clear();
	g_engine->addSprite(laser,PARTICLES_COLL);

	this->p_player->fire_tick = 0;
	this->time_lastshot = timeGetTime();

	this->addMuzzle(this->p_player->getBone("$GUN1"),angle);

}//firelaser()

void Player::fireminigun(Sprite *player)
{
	Sprite *weapon = player->getBone("$GUN1");
	double angle = weapon->getRotation();

	std::vector <Vector3> collision_dots;

	Sprite *laser = new Sprite();
	laser->setClass(PLAYER_WEAPON);
	laser->setType(BULLET);
	laser->setID(-606);
	laser->setImage(this->minigun_trail);
	laser->energy = 1;
	laser->setWidth(1500);
	laser->setRotation(angle);
	laser->setRotationAxis(0,laser->hh());
	weapon->attach(0,laser);
	g_engine->coll_manager->setColl_line3(laser);

	float coll_xp;
	float coll_yp;
	Sprite *temp_sprite_p = g_engine->coll_manager->collision_line_getclosest(
		laser->coll_box[2].getX(),laser->coll_box[2].getY(),
		laser->coll_box[0].getX(),laser->coll_box[0].getY(),
		WORLD_FRONT_COLL,
		coll_xp,coll_yp);

	float coll_xe;
	float coll_ye;
	Sprite *temp_sprite_e = g_engine->coll_manager->collision_line_getclosest(
		laser->coll_box[2].getX(),laser->coll_box[2].getY(),
		laser->coll_box[0].getX(),laser->coll_box[0].getY(),
		ENEMIES,
		coll_xe,coll_ye);

	if(temp_sprite_p && temp_sprite_e)
	{
		float distance_p = (float)g_engine->math->Distance(laser->coll_box[2].getX(),laser->coll_box[2].getY(),coll_xp,coll_yp);
		float distance_e = (float)g_engine->math->Distance(laser->coll_box[2].getX(),laser->coll_box[2].getY(),coll_xe,coll_ye);
		if(distance_p < distance_e)
		{
			laser->setWidth(distance_p);g_engine->coll_manager->collbox_update(laser);
			if(!game_spriteCollision(laser,temp_sprite_p)) game_spriteCollision(temp_sprite_p,laser);
		}
		else
		{
			laser->setWidth(distance_e);g_engine->coll_manager->collbox_update(laser);
			if(!game_spriteCollision(laser,temp_sprite_e)) game_spriteCollision(temp_sprite_e,laser);
		}
	}
	else if(temp_sprite_p)
	{
		float distance_p = (float)g_engine->math->Distance(laser->coll_box[2].getX(),laser->coll_box[2].getY(),coll_xp,coll_yp);
		laser->setWidth(distance_p);g_engine->coll_manager->collbox_update(laser);
		if(!game_spriteCollision(laser,temp_sprite_p)) game_spriteCollision(temp_sprite_p,laser);

	}
	else if(temp_sprite_e)
	{
		float distance_e = (float)g_engine->math->Distance(laser->coll_box[2].getX(),laser->coll_box[2].getY(),coll_xe,coll_ye);
		laser->setWidth(distance_e);g_engine->coll_manager->collbox_update(laser);
		if(!game_spriteCollision(laser,temp_sprite_e)) game_spriteCollision(temp_sprite_e,laser);
	}

	//fade away
	laser->setID(CHAINGUN);
	laser->setAlive(true);
	laser->setCollidable(false);
	laser->coll_box.clear();
	g_engine->addSprite(laser,PARTICLES_COLL);
	this->addMuzzle(this->p_player->getBone("$GUN1"),angle);	

	//add light
	Sprite *a = laser->Copy();
	a->setID(CHAINGUN);
	a->setAlive(true);
	a->setImage(this->minigun_trail_light);
	a->setWidth(laser->w());
	a->setColor_Texture(0xFF00FF00);
	g_engine->addSprite(a,SHADERSPRITES);

	//reset timers
	this->p_player->fire_tick = 0;
	this->time_lastshot = timeGetTime();

}//firelaser()

//////////////////////////////////////////////////
////////////////////////////////////  CONTROLS  //
//////////////////////////////////////////////////

Sprite* Player::climb_ladder()
{
	std::list<Sprite*>::iterator iter = g_engine->game_sprites[WORLD_FRONT_COLL-1]->sprites.begin();

	while (iter != g_engine->game_sprites[WORLD_FRONT_COLL-1]->sprites.end())//treba stavit u neki manji layer (enemy ili nesto)
	{
		Sprite *c = *iter;
		if (c->getID()==5001 && c->getAlive())
		{
			if (g_engine->coll_manager->TestCollision_br(c,this->p_player)) return c;			
		}
		iter++;
	}

	return NULL;
}

///////////////////////////////////////////////
/////////////////////////////////  CONTROLS  //
///////////////////////////////////////////////

void Player::control_press(int key)
{
	Sprite *a = NULL;

	if (this->p_player==NULL || this->p_player->energy<=0 ) return;

	if (key==this->FIRE1)
	{
		this->player_fire = true;
		return;
	}

	if (key==this->ACTION)
	{
		this->control_action = true;
		return;
	}

	//MOVEMENT
	//left
	if(key==this->LEFT)
	{
		if(!this->nocontrol)
		{
			int h;
			if(SwimP(this->p_player,h))
			{
				if(this->p_player->getVelocityX()>-2) this->p_player->setVelocityX(this->p_player->getVelocityX()-0.065*g_engine->game_speed*g_engine->GetDeltaFix());
				return;
			}

			if(g_engine->noclip)
			{
				this->p_player->setVelocityX(this->p_player->getVelocityX()-0.45);
				if (this->p_player->getVelocityX()<-2*this->player_speed) this->p_player->setVelocityX(-2*this->player_speed);
			}
			else
			{
				if (this->p_player->getMovementState()!=OUTOFCONTROL && this->p_player->getMovementState()!=KNEEL)
				{
					this->p_player->setVelocityX(this->p_player->getVelocityX()-0.45*g_engine->game_speed*g_engine->GetDeltaFix());

					if (this->p_player->getVelocityX()<-this->player_speed)
						this->p_player->setVelocityX(-this->player_speed);
				}

				if (this->p_player->getMovementState()==KNEEL)
				{
					this->p_player->setVelocityX(this->p_player->getVelocityX()-0.45*g_engine->game_speed*g_engine->GetDeltaFix());

					if (this->p_player->getVelocityX()<-this->speed_crouch)
						this->p_player->setVelocityX(-this->speed_crouch);
				}			

				if (this->p_player->getMovementState()==CLIMB)
				{
					a=NULL;
					a=climb_ladder();
					if (a==NULL)
					{
						this->p_player->setMovementState(FALL);				
					}
				}
			}
			return;
		}
	}

	//right
	if(key==this->RIGHT)
	{
		if(!this->nocontrol)
		{
			int h;
			if(SwimP(this->p_player,h))
			{
				if(this->p_player->getVelocityX()<2) this->p_player->setVelocityX(this->p_player->getVelocityX()+0.065*g_engine->game_speed*g_engine->GetDeltaFix());
				return;
			}

			if(g_engine->noclip)
			{
				this->p_player->setVelocityX(this->p_player->getVelocityX()+0.45*g_engine->game_speed*g_engine->GetDeltaFix());
				if (this->p_player->getVelocityX()>2*this->player_speed) this->p_player->setVelocityX(2*this->player_speed);
			}
			else
			{
				if (this->p_player->getMovementState()!=OUTOFCONTROL && this->p_player->getMovementState()!=KNEEL)
				{
					this->p_player->setVelocityX(this->p_player->getVelocityX()+0.45*g_engine->game_speed*g_engine->GetDeltaFix());

					if (this->p_player->getVelocityX()>this->player_speed)
						this->p_player->setVelocityX(this->player_speed);
				}

				if (this->p_player->getMovementState()==KNEEL)
				{
					this->p_player->setVelocityX(this->p_player->getVelocityX()+0.45*g_engine->game_speed*g_engine->GetDeltaFix());

					if (this->p_player->getVelocityX()>this->speed_crouch)
						this->p_player->setVelocityX(this->speed_crouch);
				}			

				if (this->p_player->getMovementState()==CLIMB)
				{
					a = NULL;
					a = climb_ladder();
					if (a==NULL)
					{
						this->p_player->setMovementState(FALL);			
					}
				}
			}
			return;
		}
	}

	if(key==this->C_FLY && this->getInventory("fly"))
	{
		if(!this->nocontrol)
		{
			if (this->p_player->getMovementState()==FALL && this->p_player->getVelocityY()>-5)
				this->p_player->setVelocityY(
				this->p_player->getVelocityY()-0.7*g_engine->game_speed*g_engine->GetDeltaFix());
			return;
		}
	}

	//jump
	if(key==this->JUMP)
	{
		if(!this->nocontrol)
		{
			if (this->p_player->getMovementState()==KNEEL)
			{
				this->p_player->setVelocityY(-6.4);//-6.35
				this->p_player->animation_name = "jump_crouch";
				this->p_player->setMovementState(FALL);
				g_engine->audio->Play_path(this->player_models[this->model_id]->root_directory+"jump1.wav");
			}
			else if (this->p_player->getMovementState()==WALK)
			{
				this->p_player->setVelocityY(-7.2);
				this->p_player->animation_name = "jump";
				this->p_player->setMovementState(FALL);
				g_engine->audio->Play_path(this->player_models[this->model_id]->root_directory+"jump1.wav");
			}
			else if (this->p_player->getMovementState()==FALL && this->p_player->getVelocityY()<0)
			{
				int h;
				if(SwimP(this->p_player,h) && h<this->p_player->hh())
				{
					this->p_player->setY(this->p_player->getY()-fabs((double)(h+5)));
					g_engine->coll_manager->collbox_update(this->p_player);
					this->p_player->setVelocityY(-5);
					this->p_player->animation_name = "jump";
					this->p_player->setMovementState(FALL);
					g_engine->audio->Play_path(this->player_models[this->model_id]->root_directory+"jump1.wav");
				}
				else
					this->p_player->setVelocityY(this->p_player->getVelocityY()-0.15*g_engine->game_speed*g_engine->GetDeltaFix());
			}

			return;
		}
	}

	if(key==this->DOWN)
	{
		if(!this->nocontrol)
		{
			this->kneel = true;

			int h;
			if(SwimP(this->p_player,h))
			{
				if(this->p_player->getVelocityY()<2) this->p_player->setVelocityY(this->p_player->getVelocityY()+0.065*g_engine->game_speed*g_engine->GetDeltaFix());
				return;
			}

			if(g_engine->noclip)
			{
				this->p_player->setVelocityY(this->p_player->getVelocityY()+0.45*g_engine->game_speed*g_engine->GetDeltaFix());
				if (this->p_player->getVelocityY()>2*this->player_speed) this->p_player->setVelocityY(2*this->player_speed);
			}
			else
			{
				if (this->p_player->getMovementState()==CLIMB)
				{
					a = climb_ladder();
					if (a)
					{
						this->p_player->setY(p_player->getY()+2.5);				
						p_player->setVelocityY(0);
						if (p_player->getY()+p_player->h()>a->getY()+a->getHeight())
						{
							p_player->setY(a->getY()+a->getHeight()-p_player->h());
							p_player->setMovementState(WALK);
						}
					}
				}	
			}
			return;
		}
	}

	if(key==this->UP)
	{
		if(!this->nocontrol)
		{
			int h;
			if(SwimP(this->p_player,h))
			{
				/*Line *temp_line0 = new Line(this->p_player->getX(),this->p_player->getY(),this->p_player->getX(),this->p_player->getY()+h, this->p_player->getLayer(), 0xFF00FF00);				
				g_engine->line_list.push_back(temp_line0);*/

				if(this->p_player->getVelocityY()>-2 && h>this->p_player->h()) this->p_player->setVelocityY(this->p_player->getVelocityY()-0.1*g_engine->game_speed*g_engine->GetDeltaFix());
				return;
			}
			if(g_engine->noclip)
			{
				this->p_player->setVelocityY(this->p_player->getVelocityY()-0.45*g_engine->game_speed*g_engine->GetDeltaFix());
				if (this->p_player->getVelocityY()<-2*this->player_speed) this->p_player->setVelocityY(-2*this->player_speed);
			}
			else
			{
				a = climb_ladder();
				if (a)
				{			
					this->p_player->setY(p_player->getY()-2.5);
					this->p_player->setMovementState(CLIMB);			
					p_player->setVelocityY(0);
					if (p_player->getY()+p_player->h()<a->getY())
					{
						p_player->setY(a->getY()-p_player->h());
						p_player->setMovementState(WALK);
					}
				}	
			}
			return;
		}
	}	

	if(key==this->PDA)
	{
		this->draw_inventory = true;
	}

}//keyPress()

void Player::control_release(int key)
{
	if (this->p_player==NULL) return;
	if (this->p_player->energy<=0) return;	

	//mouse
	bool weapon = false;
	int old_weapon = this->player_weapon;

	if (key==this->ACTION)
	{
		this->control_action = false;
		return;
	}

	if (key==this->NEXT_WEAPON)
	{
		while(weapon==false)
		{
			this->player_weapon++;
			if (this->player_weapon>10)
			{
				this->player_weapon = old_weapon;
				return;
			}
			if(this->hud->weapon_unlocked[player_weapon])
			{
				this->setWeaponTexture();
				weapon = true;
			}			
		}
		return;
	}

	if (key==this->PREV_WEAPON)
	{
		while(weapon==false)
		{
			this->player_weapon--;
			if (this->player_weapon<0)
			{
				this->player_weapon = old_weapon;
				return;
			}
			if (this->hud->weapon_unlocked[player_weapon])
			{
				this->setWeaponTexture();
				weapon = true;
			}			
		}
		return;
	}	

	//jump
	if(key==this->JUMP){}

	//stop left/right
	if (key==this->LEFT || key ==this->RIGHT)
	{
		if(!this->nocontrol) this->p_player->setVelocityX(0);
		return;
	}

	if (key==this->UP)
	{
		if(g_engine->noclip) this->p_player->setVelocityY(0);
		return;
	}

	//stand up	
	if (key==this->DOWN)
	{
		if(g_engine->noclip) this->p_player->setVelocityY(0);
		else if (this->p_player->getMovementState()==KNEEL)
		{
			this->p_player->setMovementState(WALK);
		}
		return;
	}

	if(key==this->BULLETTIME && this->getInventory("bullettime"))
	{
		g_engine->bullet_time = !g_engine->bullet_time;
		return;
	}

	////////////////////////////////////////////////WEAPON SELECTION

	if(key==this->WEAPON0)//pistol
	{
		if(this->player_weapon!=BLASTER && this->hud->weapon_unlocked[BLASTER])
		{
			this->player_weapon = BLASTER;
			this->setWeaponTexture();
		}
		return;
	}
	else if(key==this->WEAPON1)//shotgun
	{
		if(this->player_weapon!=SHOTGUN && this->hud->weapon_unlocked[SHOTGUN])
		{
			this->player_weapon = SHOTGUN;
			this->setWeaponTexture();
		}		
		return;
	}
	else if(key==this->WEAPON2)//super shotgun
	{
		if(this->player_weapon!=S_SHOTGUN && this->hud->weapon_unlocked[S_SHOTGUN])
		{
			this->player_weapon = S_SHOTGUN;
			this->setWeaponTexture();
		}		
		return;
	}
	else if(key==this->WEAPON3)//machinegun
	{
		if(this->player_weapon!=MACHINEGUN && this->hud->weapon_unlocked[MACHINEGUN])
		{
			this->player_weapon = MACHINEGUN;
			this->setWeaponTexture();
		}
		return;
	}
	else if(key==this->WEAPON4)//chaingun
	{
		if(this->player_weapon!=CHAINGUN && this->hud->weapon_unlocked[CHAINGUN])
		{
			this->player_weapon = CHAINGUN;
			this->setWeaponTexture();
		}		
		return;
	}
	else if(key==this->WEAPON5)//grenade launcher
	{
		if(this->player_weapon!=G_LAUNCHER && this->hud->weapon_unlocked[G_LAUNCHER])
		{
			this->player_weapon = G_LAUNCHER;
			this->setWeaponTexture();
		}			
		return;
	}
	else if(key==this->WEAPON6)//rocket launcher
	{
		if(this->player_weapon!=R_LAUNCHER && this->hud->weapon_unlocked[R_LAUNCHER])
		{
			this->player_weapon = R_LAUNCHER;
			this->setWeaponTexture();
		}		
		return;
	}
	else if(key==this->WEAPON7)//hyperblaster
	{
		if(this->player_weapon!=HYPERBLASTER && this->hud->weapon_unlocked[HYPERBLASTER])
		{
			this->player_weapon = HYPERBLASTER;
			this->setWeaponTexture();
		}
		return;
	}
	else if(key==this->WEAPON8)//railgun
	{
		if(this->player_weapon!=RAILGUN && this->hud->weapon_unlocked[RAILGUN])
		{
			this->player_weapon = RAILGUN;
			this->setWeaponTexture();
		}		
		return;
	}
	else if(key==this->WEAPON9)//cannon/bfg
	{
		if(this->player_weapon!=BFG && this->hud->weapon_unlocked[BFG])
		{
			this->player_weapon = BFG;
			this->setWeaponTexture();
		}		
		return;
	}

	if(key==this->PDA)
	{
		this->draw_inventory = false;
	}

}//control_release()


bool Player::StandUp()
{
	//treba ovaj swim samo jednom pozva, a ovako sto puta
	int h;
	if(SwimP(this->p_player,h)) return false;
	/*{
	Sprite *b = this->p_player;
	Line *temp_line0 = new Line(b->coll_box[0].getX(), b->coll_box[0].getY(), b->coll_box[1].getX(), b->coll_box[1].getY(), b->getLayer(), 0xFFFFFFFF);				
	g_engine->line_list.push_back(temp_line0);
	Line *temp_line1 = new Line(b->coll_box[1].getX(), b->coll_box[1].getY(), b->coll_box[2].getX(), b->coll_box[2].getY(), b->getLayer(), 0xFFFFFFFF);				
	g_engine->line_list.push_back(temp_line1);
	Line *temp_line2 = new Line(b->coll_box[2].getX(), b->coll_box[2].getY(), b->coll_box[3].getX(), b->coll_box[3].getY(), b->getLayer(), 0xFFFFFFFF);				
	g_engine->line_list.push_back(temp_line2);
	Line *temp_line3 = new Line(b->coll_box[3].getX(), b->coll_box[3].getY(), b->coll_box[0].getX(), b->coll_box[0].getY(), b->getLayer(), 0xFFFFFFFF);				
	g_engine->line_list.push_back(temp_line3);
	}*/

	//if(this->in_lift)
	//ako je u liftu onda je layer a ne sektori

	int temp_frame = this->p_player->getFrameCurrent();

	this->p_player->setFrameCurrent(0);
	this->p_player->setFramesTotal(1);
	this->p_player->setFrameTimer(0);
	if (this->p_player->getFrameCurrent() < 0) this->p_player->setFrameCurrent(this->p_player->getFramesTotal()-1);
	if (this->p_player->getFrameCurrent() > this->p_player->getFramesTotal()-1) this->p_player->setFrameCurrent(0);
	this->player_models[this->model_id]->animation_stand.setRotation(this->p_player);

	//set bone positions
	g_engine->skeleton_manager.Walk_Bipedal(this->p_player);

	//dynamic collbox (expect problems)
	float pos_y = this->p_player->getY()+this->p_player->h();

	if(this->p_player->getBone("$FOOT1"))
	{
		if(this->p_player->getBone("$FOOT1")->getAtt_posY(0) > this->p_player->getBone("$FOOT2")->getAtt_posY(0))
			this->p_player->setHeight((int)fabs(this->p_player->getBone("$TORSO")->getAtt_posY(0)-this->p_player->getBone("$FOOT1")->getAtt_posY(0)));
		else
			this->p_player->setHeight((int)fabs(this->p_player->getBone("$TORSO")->getAtt_posY(0)-this->p_player->getBone("$FOOT2")->getAtt_posY(0)));
	}
	else
	{
		if(this->p_player->getBone("$LEG1_DOWN")->getAtt_posY(0) > this->p_player->getBone("$LEG2_DOWN")->getAtt_posY(0))
			this->p_player->setHeight((int)fabs(this->p_player->getBone("$TORSO")->getAtt_posY(0)-this->p_player->getBone("$LEG1_DOWN")->getAtt_posY(0)));
		else
			this->p_player->setHeight((int)fabs(this->p_player->getBone("$TORSO")->getAtt_posY(0)-this->p_player->getBone("$LEG2_DOWN")->getAtt_posY(0)));
	}	

	if(this->p_player->getBone("$HEAD")) this->p_player->setHeight((int)(this->p_player->getHeight()+this->p_player->getBone("$HEAD")->att_distance[0]));
	this->p_player->setY(pos_y-this->p_player->h());	

	g_engine->coll_manager->updateColl_box(this->p_player);

	/*{
	Sprite *b = this->p_player;
	Line *temp_line0 = new Line(b->coll_box[0].getX(), b->coll_box[0].getY(), b->coll_box[1].getX(), b->coll_box[1].getY(), b->getLayer(), 0xFFFFFF00);				
	g_engine->line_list.push_back(temp_line0);
	Line *temp_line1 = new Line(b->coll_box[1].getX(), b->coll_box[1].getY(), b->coll_box[2].getX(), b->coll_box[2].getY(), b->getLayer(), 0xFFFFFF00);				
	g_engine->line_list.push_back(temp_line1);
	Line *temp_line2 = new Line(b->coll_box[2].getX(), b->coll_box[2].getY(), b->coll_box[3].getX(), b->coll_box[3].getY(), b->getLayer(), 0xFFFFFF00);				
	g_engine->line_list.push_back(temp_line2);
	Line *temp_line3 = new Line(b->coll_box[3].getX(), b->coll_box[3].getY(), b->coll_box[0].getX(), b->coll_box[0].getY(), b->getLayer(), 0xFFFFFF00);				
	g_engine->line_list.push_back(temp_line3);
	}*/

	for(int i=0;i<g_engine->game_maps[g_engine->map_current]->sectors.size();i++)
	{
		//entity inside sector
		if(
			g_engine->game_maps[g_engine->map_current]->sectors[i].IsInside(this->p_player->coll_box[0].getX(),this->p_player->coll_box[0].getY())
			|| g_engine->game_maps[g_engine->map_current]->sectors[i].IsInside(this->p_player->coll_box[1].getX(),this->p_player->coll_box[1].getY())
			|| g_engine->game_maps[g_engine->map_current]->sectors[i].IsInside(this->p_player->coll_box[2].getX(),this->p_player->coll_box[2].getY())
			|| g_engine->game_maps[g_engine->map_current]->sectors[i].IsInside(this->p_player->coll_box[3].getX(),this->p_player->coll_box[3].getY())
			)
		{
			//check collisions with sprites inside sector
			std::list<Sprite*>::iterator iter = g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.begin();
			while (iter != g_engine->game_maps[g_engine->map_current]->sectors[i].sprite_list.end())
			{
				Sprite *a = *iter;
				if (a->getAlive() && a->getCollidable())//&& a->getType()==501
				{
					if(g_engine->coll_manager->DotInsideSpriteBR(this->p_player->coll_box[0].getX(),this->p_player->coll_box[0].getY(),a)
						|| g_engine->coll_manager->DotInsideSpriteBR(this->p_player->coll_box[1].getX(),this->p_player->coll_box[1].getY(),a))
					{
						//stay crouched
						if(this->p_player->getVelocityX()==0)
						{
							this->p_player->setFrameCurrent(0);
							this->p_player->setFramesTotal(1);
							this->p_player->setFrameTimer(0);
							if (this->p_player->getFrameCurrent() < 0) this->p_player->setFrameCurrent(this->p_player->getFramesTotal()-1);
							if (this->p_player->getFrameCurrent() > this->p_player->getFramesTotal()-1) this->p_player->setFrameCurrent(0);
							this->player_models[this->model_id]->animation_kneel.setRotation(this->p_player);
						}
						else
						{
							this->p_player->setFrameCurrent(temp_frame);
							this->p_player->setFramesTotal(this->player_models[this->model_id]->animation_walkcrouch.totalframes);
							this->p_player->setFrameTimer(this->player_models[this->model_id]->animation_walkcrouch.timer);
							if (this->p_player->getFrameCurrent() < 0) this->p_player->setFrameCurrent(this->p_player->getFramesTotal()-1);
							if (this->p_player->getFrameCurrent() > this->p_player->getFramesTotal()-1) this->p_player->setFrameCurrent(0);
							this->player_models[this->model_id]->animation_walkcrouch.setRotation(this->p_player);
						}						

						//update bone positions
						g_engine->skeleton_manager.Walk_Bipedal(this->p_player);	

						//keep bottom position
						float pos_y = this->p_player->getY()+this->p_player->h();

						if(this->p_player->getBone("$FOOT1"))
						{
							if(this->p_player->getBone("$FOOT1")->getAtt_posY(0) > this->p_player->getBone("$FOOT2")->getAtt_posY(0))
								this->p_player->setHeight((int)fabs(this->p_player->getBone("$TORSO")->getAtt_posY(0)-this->p_player->getBone("$FOOT1")->getAtt_posY(0)));
							else
								this->p_player->setHeight((int)fabs(this->p_player->getBone("$TORSO")->getAtt_posY(0)-this->p_player->getBone("$FOOT2")->getAtt_posY(0)));
						}
						else
						{
							if(this->p_player->getBone("$LEG1_DOWN")->getAtt_posY(0) > this->p_player->getBone("$LEG2_DOWN")->getAtt_posY(0))
								this->p_player->setHeight((int)fabs(this->p_player->getBone("$TORSO")->getAtt_posY(0)-this->p_player->getBone("$LEG1_DOWN")->getAtt_posY(0)));
							else
								this->p_player->setHeight((int)fabs(this->p_player->getBone("$TORSO")->getAtt_posY(0)-this->p_player->getBone("$LEG2_DOWN")->getAtt_posY(0)));
						}	

						if(this->p_player->getBone("$HEAD")) this->p_player->setHeight((int)(this->p_player->getHeight()+this->p_player->getBone("$HEAD")->att_distance[0]));
						this->p_player->setY(pos_y-this->p_player->h());						

						g_engine->coll_manager->updateColl_box(this->p_player);

						/*Sprite *b = this->p_player;
						Line *temp_line0 = new Line(b->coll_box[0].getX(), b->coll_box[0].getY(), b->coll_box[1].getX(), b->coll_box[1].getY(), b->getLayer(), 0xFF00FF00);				
						g_engine->line_list.push_back(temp_line0);
						Line *temp_line1 = new Line(b->coll_box[1].getX(), b->coll_box[1].getY(), b->coll_box[2].getX(), b->coll_box[2].getY(), b->getLayer(), 0xFF00FF00);				
						g_engine->line_list.push_back(temp_line1);
						Line *temp_line2 = new Line(b->coll_box[2].getX(), b->coll_box[2].getY(), b->coll_box[3].getX(), b->coll_box[3].getY(), b->getLayer(), 0xFF00FF00);				
						g_engine->line_list.push_back(temp_line2);
						Line *temp_line3 = new Line(b->coll_box[3].getX(), b->coll_box[3].getY(), b->coll_box[0].getX(), b->coll_box[0].getY(), b->getLayer(), 0xFF00FF00);				
						g_engine->line_list.push_back(temp_line3);*/

						return false;

					}
				}

				iter++;
			}
		}
	}

	this->p_player->setFrameCurrent(temp_frame);

	return true;

}