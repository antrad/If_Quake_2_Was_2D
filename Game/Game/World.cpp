#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "World.h"
#include <ios>
#include <sstream> 

Sprite* World::addSprite_basic(int type,
							   double x, double y, double w, double h,
							   int my_layer, int render_type,
							   std::string texture_path, int frame)
{
	//FIND TEXTURE
	Texture *my_texture = NULL;
	my_texture = g_engine->texture_manager->findTexture(texture_path);
	if (my_texture==NULL)
	{
		g_engine->gui->messagebox_add("ERROR",texture_path + " not found !",10);
		return NULL;
	}

	//CREATE SPRITE
	Sprite *sprite_temp = new Sprite();
	sprite_temp->setImage(my_texture);
	sprite_temp->setFrameColumns(my_texture->tile_columns);
	sprite_temp->setFrameCurrent(frame);
	sprite_temp->setRenderType(render_type);
	sprite_temp->setType(type);	
	sprite_temp->setPosition(x,y);	
	sprite_temp->setSize(w,h);	
	g_engine->addSprite(sprite_temp,my_layer);

	return sprite_temp;

}//addSprite_basic()

Sprite* World::addSprite_string(int my_class, int type, int id, int state, std::string line, std::string third_line)
{
	std::string line2 = line ;

	double x, y, w, h;
	int my_layer,collidable;
	int render_type;
	double scale_x,scale_y;
	double rotation;								
	std::string texture_path;
	int frame,frame_relx,frame_rely;
	int energy;
	double scroll_x,scroll_y,scroll_step_x,scroll_step_y;
	double offset_mx,offset_my;
	int flip_h,flip_v;
	double vel_x,vel_y;
	int alpha,red,green,blue;
	
	//std::string command;

	std::stringstream stream(line);	

	if (stream
		>>x>>y>>w>>h
		>>my_layer>>collidable>>render_type
		>>scale_x>>scale_y>>rotation
		>>texture_path
		>>frame>>frame_relx>>frame_rely
		>>scroll_x>>scroll_y>>scroll_step_x>>scroll_step_y>>offset_mx>>offset_my>>flip_h>>flip_v
		>>vel_x>>vel_y>>energy
		>>alpha>>red>>green>>blue)
	{
		Sprite *block = new Sprite();
		block->setClass(my_class);	
		block->setType(type);	
		block->setID(id);
		block->setState(state);

		block->setText(third_line);		

		/////find texture
		Texture *my_texture = NULL;
		my_texture = g_engine->texture_manager->findTexture(texture_path);
		if(my_texture==NULL)
		{
			my_texture = g_engine->gui->default_t;
			block->setColor_Texture(0xFFFF00F6);
			//g_engine->gui->messagebox_add("ERROR ADDING TILE SPRITE","Missing texture " + texture_path + ".",100);
			//return NULL;
		}
		//image
		block->setImage(my_texture);
		block->setFrameColumns(my_texture->tile_columns);
		block->setFrameCurrent(frame);
		block->setFrameRelX(frame_relx);
		block->setFrameRelY(frame_rely);
		//

		//render type
		block->setRenderType(render_type);
		block->setOffsetMX(offset_mx);
		block->setOffsetMY(offset_my);
		block->setScrollX(scroll_x);
		block->setScrollY(scroll_y);
		block->setScrollStepX(scroll_step_x);
		block->setScrollStepY(scroll_step_y);		
		block->setSize((int)w,(int)h);
		block->setScale(scale_x,scale_y);
		if(third_line=="fitscreen")
		{
			block->setScale(
				(float)g_engine->getScreenWidth()/block->getImage()->getWidth(),
				(float)g_engine->getScreenHeight()/block->getImage()->getHeight());

			block->setSize(
				(float)g_engine->getScreenWidth(),
				(float)g_engine->getScreenHeight());
		}
		if(third_line=="fitscreen_tiled")
		{
			block->setSize((float)g_engine->getScreenWidth(),(float)g_engine->getScreenHeight());
			block->setRenderType(TILE);
		}
		else if(third_line[0]=='&')
		{
			//srusit ce se ako je state prevelik ili -999 !!!
			if(g_engine->save_to_map)
			{
				g_engine->game_maps[g_engine->map_current]->player_spawn[state].setX(x);
				g_engine->game_maps[g_engine->map_current]->player_spawn[state].setY(y);
			}
		}

		block->setRotation(rotation);		
		//

		block->flip_h = flip_h;
		block->flip_v = flip_v;
		block->setVelocity(vel_x,vel_y);
		block->setPosition(x,y);
		block->setColor_Texture(D3DCOLOR_ARGB(alpha,red,green,blue));

		block->setCollidable(collidable);
		if (block->getCollidable()) g_engine->coll_manager->setColl_box(block);

		g_engine->addSprite(block,my_layer);
		
		if(block->getID()==ITEM)
		{
			block->rel_posx = block->getY()-8;
			block->rel_posy = block->getY()+8;
			block->setVelocityY(0.3);
		}

		if(block->getClass()==SOUND)
		{
			if(block->getID()==SOUND_POINT)
			{
				block->rel_posx = g_engine->stringtoint(block->getArg(block->arg_list.size()-1));//domet zvuka zadnji argument
			}
			//block->sound = g_engine->audio->FindSound(block->getText());
		}

		return block;
	}
	else
	{
		g_engine->gui->messagebox_add("ERROR ADDING TILE SPRITE","Line not readable.",10);
		g_engine->log_text.push_back(line2);
		return NULL;
	}	

}

Sprite* World::addTileScroller(int my_class, int type, int id, int state, std::string line, std::vector <string> textures, std::string third_line)
{
	std::string line_error = line ;

	int texture_num;
	double x,y;
	int my_layer,collidable;	
	double scroll_x,scroll_y,scroll_step_x,scroll_step_y;
	int alpha,red,green,blue;	

	std::stringstream stream;
	stream << line;

	if (stream
		>>texture_num
		>>x>>y
		>>my_layer>>collidable
		>>scroll_x>>scroll_y>>scroll_step_x>>scroll_step_y		
		>>alpha>>red>>green>>blue)
	{
		Sprite *block = new Sprite();		

		for(unsigned int i=0;i<textures.size();i++)//set textures
		{			
			Texture *my_texture = NULL;
			my_texture = g_engine->texture_manager->findTexture(textures[i]);
			if(my_texture==NULL)
			{
				my_texture = g_engine->gui->default_t;
				block->setColor_Texture(0xFFFF00F6);
				//ERROR something
			}
			else
			{
				block->texture_vector.push_back(my_texture);
				block->setSize(
					block->getWidth() + my_texture->getWidth(),
					my_texture->getHeight());
			}
		}

		if(block->texture_vector.empty())
		{
			delete block;
			return NULL;
		}

		block->setRenderType(TILESCROLLER_X);
		block->setClass(my_class);
		block->setType(type);
		block->setID(id);
		block->setState(state);	
		block->setPosition(x,y);
		block->setScrollX(scroll_x);
		block->setScrollY(scroll_y);
		block->setScrollStepX(scroll_step_x);
		block->setScrollStepY(scroll_step_y);
		block->setCollidable(collidable);
		if (block->getCollidable()) g_engine->coll_manager->setColl_box(block);
		block->setColor_Texture(D3DCOLOR_ARGB(alpha,red,green,blue));

		block->setText(third_line);
		g_engine->addSprite(block,my_layer);	

		return block;
	}	
	else
	{
		g_engine->gui->messagebox_add("ERROR ADDING SPRITE","Line not readable.",10);
		g_engine->log_text.push_back(line_error);
		return NULL;
	}	

}//addTileScroller()