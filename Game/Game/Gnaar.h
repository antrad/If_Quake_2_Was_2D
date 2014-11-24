#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define ENEMY 600
#define GNAAR 304
#define GNAAR_MALE 0
#define GNAAR_FEMALE 1

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

class GnaarModel
{
public:
	GnaarModel();

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

class Gnaar
{
private:
	double velocity_walk;
	double velocity_run;

	double energy_male;
	double energy_female;

public:
	Gnaar(void);	

	std::vector <GnaarModel*> gnaar_models;
	
	bool LoadTextures();	
	
	Sprite* addGnaar(double x, double y, int id,double scale_x,double scale_y,int energy,int difficulty,std::string command);
	void addSkeleton(Sprite *a,double scale_x,double scale_y);	

	void Update(Sprite *b, Sprite *player, Sprite *player2);
	void UpdateSkeleton(Sprite *a, Sprite *player, Sprite *player2);
	void UpdateSkeleton_lite(Sprite *a);

	void HitPlatform(Sprite *a, int direction,Sprite *b);
	void TurnLeft(Sprite *b);
	void TurnRight(Sprite *b);	

};//Gnaar
