#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define TEXTURES_MISC 3

#define KRV 512
#define DIM 513
#define FIRE 514
#define SPARK 515
#define EXP 516

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

class Particles
{
private:
	Texture *smoke_t;	
	Texture *rocket_flame;	
	Texture *bloodmist;
	Texture *spark_t;
	Texture *light0;

public:
	Particles(void);
	bool LoadTextures(void);

	void addBloodBig(float x , float y, int amount);
	void addSpark(float x , float y, int amount);
	void addRocketFlame(Sprite *rocket);
	void addExplosionCollision(float x, float y, int radius);
	void addSmoke(float x, float y, int amount);	
	void addFire(float x, float y, int amount);
	void addFireLinear(float x, float y, int angle);

	void addLight(float x, float y, float scale);

	
	void addBulletTrail(float x, float y, float angle, int amount);

	void addBulletSpark(float x, float y, int amount);	
	void addBulletSpark(Sprite *bullet, Sprite *wall, int amount);

	void addBulletSpark_Grey(float x, float y, int amount);	
	void addBulletSpark_Grey(Sprite *bullet, Sprite *wall, int amount);

	void addFlies(Sprite *range);
	void addFlies_full(float x, float y);

};//Particles
