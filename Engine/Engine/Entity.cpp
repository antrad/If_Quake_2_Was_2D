#include "Advanced2D.h"

namespace Advanced2D
{
	Entity::Entity()
	{
		//CLASSIFICATION
		this->name = "#";
		this->entity_class = -999;
		this->type = -999;
		this->id = -999;
		this->uigid = -999;

		//PRIMARY PARAMETERS
		this->state = -999;
		this->visible = true;
		this->alive = true;
		this->collidable = true;
		this->layer = 0;
		
		//SIZE		
		this->width = 1;
		this->height = 1;

		//SCALING
		this->scale_x = 1;
		this->scale_y = 1;

		//POSITION
		this->setPosition(0.0f,0.0f);
		this->posx_old = 0;
		this->posy_old = 0;

		//FRAME STUFF
		this->frame_x = 0;
		this->frame_y = 0;
		this->frame_w = 32;
		this->frame_h = 32;
		this->frame_relx = 0;
		this->frame_rely = 0;
		this->frame_current = 0;
		this->frame_total = 1;
		this->frame_columns = 1;
		this->frame_timer = 0;
		this->frame_offsetx = 0;
		this->frame_offsety = 0;

		//COLOR
		this->color_box = 0xFFFFFFFF;
		this->color_texture = 0xFFFFFFFF;
		this->color_text = 0xFFFFFFFF;
		this->color_alpha = 255;
		this->color_red = 255;
		this->color_green = 255;
		this->color_blue = 255;

		//MISC
		this->image = NULL;		
		this->life_length = 0;
		this->life_age = 0;		

		this->rel_posx = 0;
		this->rel_posy = 0;
		
		this->oneframelife = false;
		this->just_created = true;
	}

	void Entity::setImage(Texture *image)
	{
		this->image = image;
		this->setWidth(image->getWidth());
		this->setHeight(image->getHeight());
		this->setFrameW(image->tile_width);
		this->setFrameH(image->tile_height);
	}

};
