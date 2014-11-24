#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define ENEMY 300
#define BIOMECH 303
#define BIOMECH_MINOR 0
#define BIOMECH_MAJOR 1

#define BIOMECH_LASER 2158
#define BIOMECH_ROCKET 2199

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

class BiomechModel
{
public:
	BiomechModel();
	bool Load(std::string root_directory,std::string name);
	std::string root_directory;
	std::string name;

	Skeleton *skeleton;	

	Animation animation_walk;
	Animation animation_stand;
	Animation animation_dead;
};

class Biomech
{
private:	
	Texture *rocket;
	Texture *laser;

	double velocity_minor;
	double velocity_major;

	double energy_minor;
	double energy_major;	

public:
	Biomech(void);	

	std::vector <BiomechModel*> biomech_models;

    bool LoadTextures();	

	Sprite* addBiomech(double x, double y, int id,double scale_x,double scale_y,int energy,int difficulty,std::string command);
	void addSkeleton(Sprite *a,double scale_x,double scale_y);
	
	void Update(Sprite *b, Sprite *player, Sprite *player2);	
	void UpdateSkeleton(Sprite *a, Sprite *player, Sprite *player2);
	void UpdateSkeleton_lite(Sprite *a);

	void HitPlatform(Sprite *a, int direction, Sprite *b);
	void TurnLeft(Sprite *b);
	void TurnRight(Sprite *b);
	
	void FireWeapon(Sprite *biomech, Sprite *weapon);

	
	void DontFall(Sprite *test_me);

};//Biomech
