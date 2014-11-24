#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Screen.h"
#include "HUD_Game.h"
#include "AR_WeaponManager.h"

#define TEXTURES_PLAYER 2

#define IGRAC_1 20
#define P1_WEAPON 21
#define SANTAHAT 521
#define SHELL 509
#define BULLET 400
#define GRANATA 402
#define ROCKET 403
#define SACMA 406
#define OBJECT_EXP 502
#define FT_FLAME 444
#define CANNONBALL 445
#define BFGBALL 446
#define ITEM 450

#define TEXTURES_ENEMY 1
#define PLAYER_WEAPON 999
#define KONZOLA 12

#define BACKGROUND_0 0
#define BACKGROUND_1 1 
#define WORLD_BACK_0 2
#define WORLD_BACK_1 3
#define ENEMIES 4
#define PLAYER 5
#define PARTICLES_COLL 6
#define PARTICLES_NON 7
#define WORLD_FRONT_COLL 8
#define WORLD_FRONT_NON 9
#define SHADERSPRITES 10

#define CLIMB -123

//controls
#define KEYBOARD_MOUSE 0
#define JOYSTICK 1

#pragma once

class PlayerModel
{
public:
	PlayerModel();

	bool Load(std::string root_directory,std::string name);
	std::string root_directory;
	std::string name;

	Skeleton *skeleton;	

	Animation animation_run;
	Animation animation_stand;
	Animation animation_kneel;
	Animation animation_jump;
	Animation animation_walkcrouch;
	Animation animation_swim;
	Animation jump_crouch;

	Animation getAnimation(std::string name);

};

class Player
{
private:
public:
	Screen *screen;

	DWORD time_lastshot;
	std::vector <PlayerModel*> player_models;
	int model_id;	
	int map_id;
	bool in_lift;
	bool god;
	bool draw_inventory;
	bool kneel;
	bool nocontrol;

	Texture *weapons;
	Texture *flamethrower_flame;
	Texture *flamethrower_flame_light;

	Sprite *flashlight;
	Texture *light;
	Texture *bulletflame;
	Texture *sacma;
	Texture *shell_shotgun;
	Texture *Bullet;
	Texture *raketah;
	Texture *Granata;
	Texture *cahura;
	Texture *laser_beam;   
	Texture *laser_beam_light;   
	Texture *cannon_ball;
	Texture *bfg_ball;

	Texture *minigun_trail;
	Texture *minigun_trail_light;

	//coop
	std::string root_directory;
	int player_control;
	int player_id;
	bool active;
	//	

	std::vector <std::string> inventory;
	bool getInventory(std::string item)
	{
		for(unsigned int i=0;i<this->inventory.size();i++)
		{
			if(inventory[i]==item) return true;
		}

		return false;
	}

	void RemoveFromInventory(std::string item)
	{
		std::vector<std::string>::iterator iter = this->inventory.begin();
		while(iter != this->inventory.end())
		{
			if(*iter==item)
			{
				this->inventory.erase(iter);
				return;
			}
			else
				iter++;
		}
	}

	int armor;

	bool control_action_prev;
	bool control_action;
	
	//CONTROLS
	int LEFT,RIGHT,UP,DOWN,JUMP,FIRE1,FIRE2,PREV_WEAPON,NEXT_WEAPON,ACTION,PDA,C_FLY,BULLETTIME;
	int WEAPON0,WEAPON1,WEAPON2,WEAPON3,WEAPON4,WEAPON5,WEAPON6,WEAPON7,WEAPON8,WEAPON9;

	bool change_weapon;
	double fire_angle;
	
	HUD_Game *hud;
	AR_WeaponManager *weapon_manager;

	Texture *laser_beam2;   
	Player(std::string root_directory, int id);	

	double player_speed;
	double speed_crouch;
	int player_weapon;
	bool player_fire;
	double rotation;	

	Sprite *p_player;

	bool LoadTextures();	

	Sprite* addPlayer(double x , double y);
	void addSkeleton(Sprite *a);

	void UpdatePlayer();
	void UpdateSkeleton();
	void UpdateSkeleton_js();
	void HoldWeapon(Sprite *a);	
	void HoldWeapon_js(Sprite *a);

	Sprite* climb_ladder();

	void setWeaponTexture();	

	void FireWeapon();
	void firebullet(int tip, int time, Sprite *IGRAC1);
	void addGrenade( Sprite *player);
	void firelaser(Sprite *player);
	void fireminigun(Sprite *player);
	void addMuzzle( Sprite *weapon , double angle);
	void addShell(Sprite *weapon);	

	//CONTROLS
	void control_press(int key);
	void control_release(int key);

	void screen_create(int x,int y,int w,int h);
	bool LoadModels();

	bool StandUp();	

};//Enemy_Kamikaze
