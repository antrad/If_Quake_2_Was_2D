/* Antonio R.*/
#include "Advanced2D.h"

namespace Advanced2D
{
	AR_Map::AR_Map()
	{
		this->map_bottom = -99999;
		this->dark = 100;
		this->objective_p = "#";
		this->objective_s = "#";

		for(unsigned int i=0;i<10;i++)
		{
			Vector3 temp;
			this->player_spawn.push_back(temp);
		}
	}

	void AR_Map::Reset()
	{
		this->name = "";
		//this->player_spawn.clear();
		this->sectors.clear();		

		this->border_down_vec.clear();
		this->border_x_vec.clear();		
	}

	void AR_Map::addSpriteLayer(int amount)
	{
		for(int i=0;i<amount;i++)
		{
			SpriteLayer *layer = new SpriteLayer();
			sprite_layers.push_back(layer);
		}
	}

	void AR_Map::addSprite(Sprite *sprite, int sprite_layer)
	{
		if (sprite_layer>sprite_layers.size() || sprite_layers.empty())
		{
			MessageBox(NULL, "Adding entity to a non existing list!", NULL, MB_OK);
			g_engine->Close();
			return;
		}

		if(sprite->rotate)
			sprite->vector_rect.Set(sprite->w(),sprite->h(),sprite->getRotateX_real(),sprite->getRotateY_real());
		else
			sprite->vector_rect.Set(sprite->w(),sprite->h(),sprite->hw(),sprite->hh());

		sprite->setLayer(sprite_layer);
		sprite_layers[sprite_layer]->sprites.push_back(sprite);
	}

	void AR_Map::SetBorders(float left, float right, float up, float down)
	{
		this->border_left = left;
		this->border_right = right;
		this->border_up = up;
		this->border_down = down;

	}//SetBorders()	

	void AR_Map::DeleteSprites()
	{
		std::list<Sprite*>::iterator iter;
		for (int i=0;i<this->sprite_layers.size();i++)
		{
			iter = this->sprite_layers[i]->sprites.begin();
			while (iter != this->sprite_layers[i]->sprites.end())
			{
				(*iter)->setAlive(false);
				iter++;
			}
		}
	}

}//Advanced2D
