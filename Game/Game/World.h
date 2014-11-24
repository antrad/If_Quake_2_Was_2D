#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define TEXTURES_MISC 3

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

#define ITEM 450
#define WATER 451

#define SOUND -501
#define SOUND_POINT -502

#define SPAWNER -5001
#define FLY -5002
#define SKELETON -5003

#pragma once

class World
{
private:
public:
	Sprite* addSprite_string(int my_class, int type, int id, int state,std::string line, std::string third_line);	
	Sprite* addSprite_basic(int type,
		double x, double y,	double w, double h,
		int my_layer, int render_type,
		std::string texture_path, int frame);
	Sprite* addTileScroller(int my_class, int type, int id, int state, std::string line, std::vector <string> textures, std::string third_line);

};//World()
