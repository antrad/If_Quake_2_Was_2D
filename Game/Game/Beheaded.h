#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define BIOMECH_LASER 2158
#define BIOMECH_ROCKET 2199

#define ENEMY 300
#define BEHEADED 301
#define GUARD_LIGHT 0
#define GUARD_SHOTGUN 1
#define GUARD_MACHINEGUN 2
#define ENFORCER 3
#define PARASITE 4
#define GUNNER 5
#define KAMIKAZE 6

#define GRENADE_E -402

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

#pragma once

class BeheadedModel
{
public:
	BeheadedModel();

	bool Load(std::string root_directory,std::string name);
	std::string root_directory;
	std::string name;

	Skeleton *skeleton;	

	Animation animation_hit_front_air;
	Animation animation_hit_front_ground;
	Animation animation_hit_weak;
	Animation animation_hit_weak_ground;
	Animation animation_run;
	Animation animation_walk;
	Animation animation_stand;	
};

class Beheaded
{
private:
	Texture *t_pistol;
	Texture *t_bullet;
	Texture *t_sacma;
	Texture *t_tongue;
	Texture *t_grenade;		

public:
	Beheaded(void);

	double walk_gunner;
	double run_gunner;
	double velocity_walk;
	double velocity_run;
	double velocity_run_kamikaze;

	std::vector <BeheadedModel*> beheaded_models;
	Animation enforcer_punch;

	//LOAD TEXTURES
	bool LoadTextures();

	void addSkeleton(Sprite *a,double scale_x,double scale_y);	
	Sprite* addGuard(double x, double y, int id,double scale_x,double scale_y,int energy,int difficulty,std::string command);

	void Update(Sprite *b, Sprite *player, Sprite *player2);
	void UpdateSkeleton(Sprite *a, Sprite *player, Sprite *player2);
	void UpdateSkeleton_lite(Sprite *a);

	void HitPlatform(Sprite *a, int direction,Sprite *b);
	void TurnLeft(Sprite *b);
	void TurnRight(Sprite *b);
	
	void Fire_Light(Sprite *weapon);
	void Fire_Shotgun(Sprite *weapon, int amount);
	void Fire_Machinegun(Sprite *weapon);
	void Fire_Parasite(Sprite *head, Sprite *player);

	void DontFall(Sprite *test_me);

	void addGrenade(Sprite *weapon);

};//Beheaded
