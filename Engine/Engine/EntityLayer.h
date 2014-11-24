/* Antonio R.*/
#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class Sector
	{
	public:
		float x;
		float y;
		float w;
		float h;

		std::list <Sprite*> sprite_list;

		bool IsInside(float x,float y);
		void Draw();

	};

	class SpriteLayer
	{
	private:
		bool visible;
		bool collidable;		
		float offset_multiplierx;
		float offset_multipliery;
		std::string shader;

	public:
		SpriteLayer();
		~SpriteLayer();

		bool shadow;

		std::list <Sprite*> sprites;
		std::list <Sprite*> sprites_render;

		void addSprite(Sprite *a){this->sprites.push_back(a);};
		int getSize(){return this->sprites.size();};
		void UpdateSprites();
		void BurySprites();
		void draw(int rendertype);
		void DrawShadow();
		void draw_checkmap(int rendertype);

		void setShader(std::string value){this->shader=value;};
		std::string getShader(){return this->shader;};

		void setVisible(bool value){this->visible=value;};
		bool getVisible(){return this->visible;};

		void setCollidable(bool value){this->collidable=value;};
		bool getCollidable(){return this->collidable;};

		void setOffsetMultiplierX(float value){this->offset_multiplierx=value;};
		float getOffsetMultiplierX(){return this->offset_multiplierx;};
		void setOffsetMultiplierY(float value){this->offset_multipliery=value;}; 
		float getOffsetMultiplierY(){return this->offset_multipliery;};

		void ClearLayer();
		bool onScreen(Sprite *a);
	
	};//class SpriteLayer

}//namespace
