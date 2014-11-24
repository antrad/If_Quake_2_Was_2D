#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class Entity
	{
	private:
		//CLASSIFICATION
		std::string name;
		int entity_class;
		int type;
		int id;
		int uigid;

		//PRIMARY PARAMETERS
		int state;
		bool visible;
		bool alive;
		bool collidable;
		int layer;

		//SCALING
		float scale_x;
		float scale_y;

		//POSITION
		Vector3 position;
		float posx_old;
		float posy_old;		

		//FRAME STUFF
		int frame_x;
		int frame_y;
		int frame_w;
		int frame_h;
		int frame_relx;
		int frame_rely;
		int frame_current;
		int frame_total;
		int frame_columns;
		int frame_timer;
		int frame_offsetx;
		int frame_offsety;

		//COLOR
		D3DCOLOR color_box;
		D3DCOLOR color_texture;
		D3DCOLOR color_text;
		double color_alpha;
		double color_red;
		double color_green;
		double color_blue;

		//MISC
		Texture *image;		

	public:
		Entity();
		virtual ~Entity(){};

		//CLASSIFICATION
		std::string getName() {return this->name;}
		void setName(std::string value) {this->name = value;}

		int getClass() {return this->entity_class;}
		void setClass(int value) {this->entity_class = value;}

		int getType() {return this->type;}
		void setType(int value) {this->type = value;}

		void setID(int value) {this->id = value;}
		int getID() {return this->id;}

		void setUIGID(int value) {this->uigid = value;}
		int getUIGID() {return this->uigid;}

		//PRIMARY PARAMETERS
		int getState() {return this->state;}
		void setState(int value) {this->state = value;}

		bool getVisible() {return this->visible;}
		void setVisible(bool value) {this->visible = value;}

		bool getAlive() {return this->alive;}
		void setAlive(bool value) {this->alive = value;}

		bool getCollidable() {return this->collidable;}
		void setCollidable(bool value) {this->collidable = value;}

		int getLayer() {return this->layer;}
		void setLayer(int value) {this->layer = value;}

		//SIZE
		int width;
		int height;
		void setSize(int width, int height) {this->width = width; this->height=height;}
		int getWidth() {return this->width;}
		void setWidth(int value) {this->width = value;}
		int getHeight() {return this->height;}
		void setHeight(int value) {this->height = value;}

		//POSITION
		void setPosition(float x, float y)
		{
			this->posx_old = this->getX();
			this->posy_old = this->getY();
			this->position.Set(x,y,0);
		}
		float getX() {return this->position.getX();}		
		void setX(float x)
		{
			this->posx_old = this->getX();
			this->position.setX(x);
		}
		float getY() {return this->position.getY();}
		void setY(float y)
		{
			this->posy_old = this->getY();
			this->position.setY(y);
		}

		//SCALING
		void setScale(float scale_x,float scale_y)
		{
			this->scale_x = scale_x;
			this->scale_y = scale_y;
		}
		float getScaleX(){return this->scale_x;}
		void setScaleX(float value){this->scale_x = value;}
		float getScaleY(){return this->scale_y;}		
		void setScaleY(float value){this->scale_y = value;}

		//FRAME STUFF		
		void setFrameXY(float x,float y) {this->frame_x=x;this->frame_y=y;}
		int getFrameX() {return this->frame_x;}
		void setFrameX(int value) {this->frame_x = value;}
		int getFrameY() {return this->frame_y;}
		void setFrameY(int value) {this->frame_y = value;}
		int getFrameW() {return this->frame_w;}
		void setFrameW(int value) {this->frame_w = value;}
		int getFrameH() {return this->frame_h;}
		void setFrameH(int value) {this->frame_h = value;}
		int getFrameRelX() {return this->frame_relx;}
		void setFrameRelX(int value) {this->frame_relx = value;}
		int getFrameRelY() {return this->frame_rely;}
		void setFrameRelY(int value) {this->frame_rely = value;}
		int getFrameCurrent() {return frame_current;}
		void setFrameCurrent(int value) {frame_current = value;}
		int getFramesTotal() {return frame_total;}
		void setFramesTotal(int value) {frame_total = value;}
		int getFrameColumns() { return this->frame_columns; }
		void setFrameColumns(int value)
		{
			if (value <= 0) value = 1;
			else this->frame_columns = value;
		}
		int getFrameTimer() {return frame_timer;}
		void setFrameTimer(int value) {frame_timer = value/14;}
		void setFrameTimer_ms(int value) {frame_timer = value/14;}
		void setFrameTimer_frames(int value) {frame_timer = value;}

		//COLOR		
		D3DCOLOR getColor_Texture() {return this->color_texture;}
		void setColor_Texture(D3DCOLOR color)
		{
			this->color_texture = color;
			this->color_alpha = this->getAlpha(color);
			this->color_red = this->getRed(color);
			this->color_green = this->getGreen(color);
			this->color_blue = this->getBlue(color);
		}
		D3DCOLOR getColor_Box() {return this->color_box;}
		void setColor_Box(D3DCOLOR color) {this->color_box = color;}
		D3DCOLOR getColor_Text() {return color_text;}
		void setColor_Text(D3DCOLOR col) {color_text = col;}

		double getAlpha() {return this->color_alpha;}
		void setAlpha(double value) {this->color_alpha=value;}
		double getRed() {return this->color_red;}
		void setRed(double value) {this->color_red=value;}
		double getGreen() {return this->color_green;}
		void setGreen(double value) {this->color_green=value;}
		double getBlue() {return this->color_blue;}
		void setBlue(double value) {this->color_blue=value;}

		int getAlpha(D3DCOLOR color) {return (color & 0xFF000000) >> (6*4);}
		int getRed(D3DCOLOR color) {return (color & 0x00FF0000) >> (4*4);}
		int getGreen(D3DCOLOR color) {return (color & 0x0000FF00) >> (2*4);}
		int getBlue(D3DCOLOR color) {return (color & 0x000000FF);}

		//MISC		
		float life_length;
		float life_age;
		float rel_posx;
		float rel_posy;
		bool oneframelife;		
		bool just_created;

		void setImage(Texture *);
		Texture* getImage() {return this->image;};		

		float getLifetime() {return this->life_length;}
		void setLifetime(float ms) {this->life_length = (float)ms/14;}

		float getAge() {return this->life_age;}
		void setAge(float value) {this->life_age=value;}

	};//entity

};//advanced2d
