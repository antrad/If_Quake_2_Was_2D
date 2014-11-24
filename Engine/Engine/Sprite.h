#include "Advanced2D.h"

#define NORMAL 0
#define TILE 1
#define SHADERSPRITE 2
#define SCROLLER_X 3
#define SCROLLER_Y 4
#define TILESCROLLER_X 5
#define TILESCROLLER_Y 6
#define STRETCH 7
#define REL_FRAME 8

#define MOVE_AUTO 0
#define MOVE_MANUAL 1

#define ANIMATE_AUTO 0
#define ANIMATE_MANUAL 1

#pragma once

namespace Advanced2D
{
	enum AnimationLoop
	{
		ANIMATE_ONCE = 0,
		ANIMATE_LOOP = 1,
		ANIMATE_KILL = 2,
	};

	enum SpriteState
	{
		WALK = 0,
		WALKANDFALL = 1,
		FALL = 2,
		STANDSTILL = 3,
		OUTOFCONTROL = 4,
		KNEEL = 5, 
	};

	enum CollisionMethod
	{
		BOX = 0,
		BOX_CENTER = 1,
		BOX_RXYC = 2,
		LINE = 3,
		CENTER = 4,	
		NONE = 5
	};

	class Bone
	{
	public:
		std::string name;
		double rot_axis_x,rot_axis_y,att_x,att_y,scale_x,scale_y;

		Bone();
		~Bone();

		void Copy(Bone a)
		{
			this->rot_axis_x = a.rot_axis_x;
			this->rot_axis_y = a.rot_axis_y;
			this->att_x = a.att_x;
			this->att_y = a.att_y;
			this->scale_x = a.scale_x;
			this->scale_y = a.scale_y;
		}
	};

	class Sprite : public Entity
	{
	private:
		Vector3 velocity;
		float angular_vel;
		float rotation;
		int collision_method;

		int render_type;//sprite rendering types
		float offset_mx;//private offset multipliers
		float offset_my;
		float scroll_x;//scrolling sprite
		float scroll_y;
		float scroll_step_x;//velocity of scrolling
		float scroll_step_y;
		
		//animation
		int animation_loop;
		int animdir;
		int animation_control;

		//rotation axis
		float rotateX;
		float rotateY;

		std::string text;

	public:
		int map_current;
		int target;

		std::vector<int> child_uigid;
		std::vector <std::string> page;

		Sprite();
		virtual ~Sprite();

		std::string animation_name;
		std::string skeleton_name;
		float tick_vision;

		void move();
		void animate();
		Sprite* Copy();

		int difficulty;
		int mass;
		int jump;

		AR_Rectangle vector_rect;

		//velocity
		void setVelocity(float x, float y) {velocity.setX(x); velocity.setY(y);}
		void setVelocityX(float x){velocity.setX(x);}
		float getVelocityX(){return velocity.getX();}
		void setVelocityY(float y){velocity.setY(y);}		
		float getVelocityY(){return velocity.getY();}
		void setVelocityAngular(float value) {this->angular_vel=value;}
		float getVelocityAngular() {return this->angular_vel;}

		//rotation
		float getRotation() {return this->rotation;}
		void setRotation(float value) {this->rotation = value;}

		//collision
		int getCollisionMethod() {return this->collision_method;}
		void setCollisionMethod(int type) {this->collision_method=type;}		

		//animation
		void setAnimationLoop(int value) {animation_loop = value;}
		int getAnimationLoop() {return this->animation_loop;}		
		void setAnimationDirection(int value) {animdir = value;}
		int getAnimationDirection() {return animdir;}		
		void setAnimationControl(int value) {animation_control = value;}
		int getAnimationControl() {return animation_control;}		

		//rendering, scrolling...
		void setRenderType(int value){this->render_type=value;}
		int getRenderType(){return this->render_type;}

		void setOffsetMX(float value){this->offset_mx=value;}
		float getOffsetMX(){return this->offset_mx;}
		void setOffsetMY(float value){this->offset_my=value;}
		float getOffsetMY(){return this->offset_my;}

		void setScrollStepX(float value){this->scroll_step_x=value;}
		float getScrollStepX(){return this->scroll_step_x;}
		void setScrollStepY(float value){this->scroll_step_y=value;}
		float getScrollStepY(){return this->scroll_step_y;}
		void setScrollX(float value){this->scroll_x=value;}
		float getScrollX(){return this->scroll_x;}
		void setScrollY(float value){this->scroll_y=value;}
		float getScrollY(){return this->scroll_y;}

		void setRotationAxis(float x, float y);
		void setRotateX(float value){this->rotateX=value;}
		float getRotateX(){return this->rotateX;}
		void setRotateY(float value){this->rotateY=value;}
		float getRotateY(){return this->rotateY;}
		float getRotateX_real(){return this->rotateX*this->getScaleX();}
		float getRotateY_real(){return this->rotateY*this->getScaleY();}

		//MISC
		int parent_att_dot;
		FMOD::Channel* sound;
		std::vector <int> frame_rotation;
		std::vector <Texture*> texture_vector;
		LPDIRECT3DTEXTURE9 p_texture;
		Sprite *parent;
		Sprite *child;
		float frame_tick[10];
		float av_dir;
		bool rotating_sprite;
		int movement_type;
		std::vector <Vector3> coll_box;
		std::vector <Sprite*> attachments;
		std::vector <Sprite*> skeleton;
		std::vector <Sprite*> children;
		float rel_rotation;
		float orig_rotation;
		float xy_angle;
		float xy_distance;
		int state_movement;
		float fire_tick;
		float elastic;
		bool flip_v;
		bool flip_h;
		float energy;
		std::string shader;
		bool ShaderOn;
		bool rotate;
		int raycast_counter;

		//states
		int getMovementState() { return state_movement; }
		void setMovementState(int value) { state_movement = value; }		

		void setShader(std::string value)
		{
			if (value.compare("")==0 || value.compare("null")==0 || value.compare("Null")==0 || value.compare("NULL")==0)
			{
				this->shader="";
				this->ShaderOn=false;
			}
			else 
			{
				this->shader=value;
				this->ShaderOn=true;
			}
		};

		std::string getShader(){return this->shader;};

		float getBoxCX();
		float getBoxCY();
		void setBoxCXY(float x, float y);
		void setScale_C(float scale);

		float cx();
		float cy();
		float w();
		float h();
		float hw();
		float hh();

		//////////

		std::vector <float> att_angle;
		std::vector <float> att_distance;
		std::vector <float> att_posx;
		std::vector <float> att_posy;		

		void setAtt_num(int num)
		{
			if (num==0)
			{
				this->att_angle.clear();
				this->att_distance.clear();
				this->att_posx.clear();
				this->att_posy.clear();
				return;
			}

			for(int i=0;i<num;i++)
			{
				this->att_angle.push_back(0);
				this->att_distance.push_back(0);
				this->att_posx.push_back(0);
				this->att_posy.push_back(0);
			}
		};

		void setAtt_pos(int num, float x, float y);
		float getAtt_posX(int num);
		float getAtt_posY(int num);

		void attach(int num,Sprite *a)
		{
			if (num > this->att_angle.size()-1) return;
			a->setPosition(this->getAtt_posX(num),this->getAtt_posY(num));
		}

		Sprite* getBone(std::string name)
		{
			for(unsigned int i=0;i<this->skeleton.size();i++)
			{
				if(this->skeleton[i]->getName()==name) return this->skeleton[i];
			}

			return NULL;
		}

		void FlipAll_H(bool value)
		{
			this->flip_h = value;
			for(unsigned int i=0;i<this->skeleton.size();i++) this->skeleton[i]->flip_h = value;
			for(unsigned int i=0;i<this->attachments.size();i++) this->attachments[i]->flip_h = value;

		}

		void FlipAll_V(bool value)
		{
			this->flip_v = value;
			for(unsigned int i=0;i<this->skeleton.size();i++) this->skeleton[i]->flip_v = value;
			for(unsigned int i=0;i<this->attachments.size();i++) this->attachments[i]->flip_v = value;
		}


		float getX_A()
		{
			if(this->rotate) return this->vector_rect.getX_A(this->getX(),this->getRotation(),this->flip_h);
			else return this->vector_rect.getX_A(this->cx(),this->getRotation(),this->flip_h);
		}

		float getY_A()
		{
			if(this->rotate) return this->vector_rect.getY_A(this->getY(),this->getRotation(),this->flip_h);
			else return this->vector_rect.getY_A(this->cy(),this->getRotation(),this->flip_h);
		}

		float getX_B()
		{
			if(this->rotate) return this->vector_rect.getX_B(this->getX(),this->getRotation(),this->flip_h);
			else return this->vector_rect.getX_B(this->cx(),this->getRotation(),this->flip_h);
		}

		float getY_B()
		{
			if(this->rotate) return this->vector_rect.getY_B(this->getY(),this->getRotation(),this->flip_h);
			else return this->vector_rect.getY_B(this->cy(),this->getRotation(),this->flip_h);
		}

		float getX_C()
		{
			if(this->rotate) return this->vector_rect.getX_C(this->getX(),this->getRotation(),this->flip_h);
			else return this->vector_rect.getX_C(this->cx(),this->getRotation(),this->flip_h);
		}

		float getY_C()
		{
			if(this->rotate) return this->vector_rect.getY_C(this->getY(),this->getRotation(),this->flip_h);
			else return this->vector_rect.getY_C(this->cy(),this->getRotation(),this->flip_h);
		}

		float getX_D()
		{
			if(this->rotate) return this->vector_rect.getX_D(this->getX(),this->getRotation(),this->flip_h);
			else return this->vector_rect.getX_D(this->cx(),this->getRotation(),this->flip_h);
		}

		float getY_D()
		{
			if(this->rotate) return this->vector_rect.getY_D(this->getY(),this->getRotation(),this->flip_h);
			else return this->vector_rect.getY_D(this->cy(),this->getRotation(),this->flip_h);
		}

		void setText(std::string text)
		{
			this->text = text;
			this->Arg_Update();//zbog ovoga sada text nije u entity
		};
		std::string getText() {return this->text;};


		std::vector <std::string> arg_list;
		void Arg_Update()
		{
			//return;

			this->arg_list.clear();
			std::string arg;
			std::stringstream stream(this->getText());

			while(stream >> arg)
			{
				this->arg_list.push_back(arg);
				arg.clear();
			}
		}

		std::string getArg(int n)
		{
			if( n > this->arg_list.size()-1 ) return "#";
			
			return this->arg_list[n];
		}
		
	};

};