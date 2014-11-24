/* Antonio R.*/
#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class AR_Map
	{
	public:
		AR_Map();
		~AR_Map();

		void Reset();

		int dark;

		std::string name;
		std::vector <Vector3> player_spawn;
		std::vector <Sector> sectors;

		std::string objective_p;
		std::string objective_s;

		float map_bottom;

		//borders
		int map_red, map_green, map_blue;
		float border_left, border_right, border_up, border_down;
		std::vector <float> border_down_vec;
		std::vector <float> border_x_vec;
		void SetBorders(float left, float right, float up, float down);	

		void setName(std::string value) {this->name = value;};
		std::string getName() {return this->name;};

		//sprites
		std::vector <SpriteLayer*> sprite_layers;
		void addSpriteLayer(int amount);
		void addSprite(Sprite *sprite, int sprite_layer);

		void DeleteSprites();		
	
	};//AR_Map()

}//namespace
