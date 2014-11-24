#include "Advanced2D.h"

namespace Advanced2D
{
	Bone::Bone()
	{
		this->name = "#";
		this->rot_axis_x = 0;
		this->rot_axis_y = 0;
		this->att_x = 0;
		this->att_y = 0;
		this->scale_x = 1;
		this->scale_y = 1;
	}

	Bone::~Bone(){}

	Sprite::Sprite() : Entity()
	{
		this->target = 0;
		this->map_current = 0;
		this->difficulty = 0;
		this->mass = 100;
		this->jump = 0;//za neprijatelje da preskoce platformu

		this->tick_vision = 0;
		this->setVelocity(0.0f,0.0f);
		this->angular_vel = 0;
		this->rotation = 0;
		this->collision_method = NONE;

		this->render_type = NORMAL;
		this->offset_mx = 1;
		this->offset_my = 1;
		this->scroll_x = 0;
		this->scroll_y = 0;
		this->scroll_step_x = 0;
		this->scroll_step_y = 0;

		this->animation_control = ANIMATE_AUTO;
		this->animdir = 1;
		this->animation_loop = ANIMATE_LOOP;	

		this->animation_name = "#";
		this->skeleton_name = "#";

		//misc
		this->sound = NULL;
		this->p_texture = NULL;
		this->parent = NULL;
		this->child = NULL;

		this->raycast_counter = 0;
		this->parent_att_dot = 0;		
		this->rel_rotation = 0;
		this->orig_rotation = 0;
		this->rotating_sprite = false;
		
		this->av_dir = 0;		
		this->frame_tick[0] = 0;		
		
		this->movement_type = MOVE_AUTO;
		this->state_movement = -999;
		this->fire_tick = 0;		
		this->flip_v = false;
		this->flip_h = false;
		this->elastic = 1;
		this->energy = 0;		
		this->rotate = false;
		this->rotateX = 0;
		this->rotateY = 0;
		this->shader = "#";
		this->ShaderOn = false;	

		this->setText("#");
	}

	Sprite::~Sprite(){}		

	void Sprite::move()
	{
		if (this->movement_type==MOVE_AUTO)
		{
			this->setX(this->getX() + this->velocity.getX()*g_engine->game_speed*g_engine->GetDeltaFix());
			this->setY(this->getY() + this->velocity.getY()*g_engine->game_speed*g_engine->GetDeltaFix());
			this->setRotation(this->getRotation() + this->getVelocityAngular()*g_engine->game_speed*g_engine->GetDeltaFix());
		}
	}

	void Sprite::animate() 
	{
		if(this->getRenderType()==SCROLLER_X || this->getRenderType()==TILESCROLLER_X)
		{
			this->setScrollX(this->getScrollX()+this->getScrollStepX()*g_engine->game_speed*g_engine->GetDeltaFix());
		}
		else if(this->getRenderType()==SCROLLER_Y || this->getRenderType()==TILESCROLLER_Y)
		{
			this->setScrollY(this->getScrollY()+this->getScrollStepY()*g_engine->game_speed*g_engine->GetDeltaFix());
		}

		//update frame based on animdir - missing
		if (this->getFrameTimer() > 0)
		{
			this->frame_tick[0] += g_engine->game_speed*g_engine->GetDeltaFix();

			if (this->frame_tick[0] >= this->getFrameTimer()) 
			{ 
				//reset animation timer
				this->setFrameCurrent(this->getFrameCurrent()+1);
				this->frame_tick[0]=0;

				switch(animation_loop)
				{
				case ANIMATE_LOOP:
					if (this->getFrameCurrent() < 0) this->setFrameCurrent(this->getFramesTotal()-1);
					if (this->getFrameCurrent() > this->getFramesTotal()-1) this->setFrameCurrent(0);
					break;

				case ANIMATE_ONCE:
					if (this->getFrameCurrent() > this->getFramesTotal()-1)
					{
						this->setFrameCurrent(this->getFramesTotal()-1);
						this->setFrameTimer(0);
					}
					break;

				case ANIMATE_KILL:
					if (this->getFrameCurrent() > this->getFramesTotal()-1)
					{
						this->setFrameCurrent(this->getFramesTotal()-1);
						this->setAlive(false);
					}
					break;

				}//switch
			}//if
		}//frametimer

		//calculate frame position and size
		if(this->getImage()!=NULL)
		{
			int fx = (this->getFrameCurrent() % this->getFrameColumns()) * this->getImage()->tile_width;
			int fy = (this->getFrameCurrent() / this->getFrameColumns()) * this->getImage()->tile_height;
			fx += (this->getFrameCurrent() % this->getFrameColumns()) * this->getImage()->tile_offset_x;
			fy += (this->getFrameCurrent() / this->getFrameColumns()) * this->getImage()->tile_offset_y;
			fx += this->getFrameRelX();
			fy += this->getFrameRelY();
			
			this->setFrameX(fx);
			this->setFrameY(fy);
		}

	}//animate

	void Sprite::setScale_C(float scale)
	{
		float x=this->cx();
		float y=this->cy();

		this->setScale(scale,scale);
		this->setPosition(
			x-this->hw(),
			y-this->hh()
			);
	}	

	//return X center of sprite
	float Sprite::cx()
	{
		return (getX()+hw());
	}

	//return Y center of sprite
	float Sprite::cy()
	{
		return (getY()+hh());
	}

	//width
	float Sprite::w()
	{ 
		return width*getScaleX();
	}

	//height
	float Sprite::h()
	{ 
		return height*getScaleY();
	}

	//half width
	float Sprite::hw()
	{ 
		return width*getScaleX()/2;
	}

	//half height
	float Sprite::hh()
	{ 
		return height*getScaleY()/2;
	}

	void Sprite::setBoxCXY(float x, float y)
	{
		this->xy_distance = g_engine->math->Distance(this->rotateX,this->rotateY,x,y);
		this->xy_angle = g_engine->math->AngleToTarget(this->rotateX,this->rotateY,x,y);
	}

	float Sprite::getBoxCX()
	{
		int sign=1;
		if (this->flip_h) sign=-1;
		return this->getX()+this->xy_distance*cos(this->getRotation()+sign*this->xy_angle)*this->getScaleX();
	};

	float Sprite::getBoxCY()
	{
		int sign=1;
		if (this->flip_h) sign=-1;
		return this->getY()+this->xy_distance*sin(this->getRotation()+sign*this->xy_angle)*this->getScaleY();
	};

	void Sprite::setRotationAxis(float x, float y)
	{
		this->rotate = true;
		this->rotateX = x;
		this->rotateY = y;

		setBoxCXY(this->getWidth()/2,this->getHeight()/2);
	};

	//////// treba ovdje dolje uredit kad ce se koristit scale, a kad ne
	//->bolje da se racuna izvana

	void Sprite::setAtt_pos(int num, float x, float y)
	{
		if (num > att_posx.size()-1) return;

		att_posx[num] = x;
		att_posy[num] = y;

		this->att_distance[num] = g_engine->math->Distance(this->getRotateX_real(),this->getRotateY_real(),x*this->getScaleX(),y*this->getScaleY());
		this->att_angle[num] = g_engine->math->AngleToTarget(this->getRotateX_real(),this->getRotateY_real(),x*this->getScaleX(),y*this->getScaleY());		
	}

	float Sprite::getAtt_posX(int num)
	{
		if (num > att_angle.size()-1) return 0;

		int sign=1;
		if (this->flip_h) sign=-1;
		return this->getX()+this->att_distance[num]*cos(this->getRotation()+sign*this->att_angle[num]);
	};

	float Sprite::getAtt_posY(int num)
	{
		if (num > att_angle.size()-1) return 0;

		int sign=1;
		if (this->flip_h) sign=-1;
		return this->getY()+this->att_distance[num]*sin(this->getRotation()+sign*this->att_angle[num]);
	};

	Sprite* Sprite::Copy()
	{
		//pokazivaci,liste, vectori se ne kopiraju
		//if (!this->skeleton.empty() || !this->attachments.empty()) return NULL;

		Sprite *a = new Sprite();

		if (this->getImage()!=NULL) a->setImage(this->getImage());
		if (this->p_texture!=NULL) a->p_texture = this->p_texture;   

		//classification
		a->setName(this->getName());
		a->setClass(this->getClass());
		a->setType(this->getType());
		a->setID(this->getID());

		//primary parameters
		a->setState(this->getState());
		a->setVisible(this->getVisible());
		a->setAlive(this->getAlive());
		a->setCollidable(this->getCollidable());
		a->setCollisionMethod(this->getCollisionMethod());
		a->setLayer(this->getLayer());

		//size
		a->width = this->width;
		a->height = this->height;		

		//position
		a->setPosition(this->getX(),this->getY());

		//velocity
		a->setVelocity(this->getVelocityX(),this->getVelocityY());
		a->setVelocityAngular(this->getVelocityAngular());

		//scaling
		a->setScaleX(this->getScaleX());
		a->setScaleY(this->getScaleY());

		//render type
		a->setRenderType(this->getRenderType());		
		a->setOffsetMX(this->getOffsetMX());
		a->setOffsetMY(this->getOffsetMY());
		a->setScrollStepX(this->getScrollStepX());
		a->setScrollStepY(this->getScrollStepY());
		a->setScrollX(this->getScrollX());
		a->setScrollY(this->getScrollY());

		//frame stuff
		a->setFrameXY(this->getFrameX(),this->getFrameY());
		a->setFrameW(this->getFrameW());
		a->setFrameH(this->getFrameH());
		a->setFrameCurrent((int)this->getFrameCurrent());
		a->setFramesTotal(this->getFramesTotal());
		a->setFrameColumns(this->getFrameColumns());
		a->setFrameTimer(this->getFrameTimer());
		a->setFrameRelX(this->getFrameRelX());
		a->setFrameRelY(this->getFrameRelY());

		//color		
		a->setColor_Box(this->getColor_Box());
		a->setColor_Texture(this->getColor_Texture());
		a->setAlpha(this->getAlpha());
		a->setRed(this->getRed());
		a->setGreen(this->getGreen());
		a->setBlue(this->getBlue());		

		//animation
		a->setAnimationDirection(this->getAnimationDirection());		
		a->setAnimationControl(this->getAnimationControl());
		a->setAnimationLoop(this->getAnimationLoop());

		//MISC
		a->life_length=this->life_length;
		a->life_age=this->life_age;
		a->rel_posx=this->rel_posx;
		a->rel_posy=this->rel_posx;
		a->setText(this->getText());
		a->oneframelife=this->oneframelife;		
		a->just_created=this->just_created;		

		a->setRotation(this->getRotation());
		a->av_dir=this->av_dir;
		a->rotating_sprite=this->rotating_sprite;
		a->movement_type=this->movement_type;		

		a->rel_rotation=this->rel_rotation;
		a->orig_rotation=this->orig_rotation;

		a->xy_angle=this->xy_angle;
		a->xy_distance=this->xy_distance;		

		a->state_movement=this->state_movement;
		a->fire_tick=this->fire_tick;
		a->elastic=this->elastic;		
		a->flip_v=this->flip_v;
		a->flip_h=this->flip_h;

		a->energy=this->energy;		
		a->shader=this->shader;
		a->ShaderOn=this->ShaderOn;

		a->rotate = this->rotate;
		a->rotateX = this->rotateX;
		a->rotateY = this->rotateY;
		a->att_angle = this->att_angle;
		a->att_distance = this->att_distance;
		a->att_posx = this->att_posx;
		a->att_posy = this->att_posy;

		a->parent_att_dot = this->parent_att_dot;

		for(unsigned int i=0;i<this->texture_vector.size();i++)
		{
			a->texture_vector.push_back(this->texture_vector[i]);
		}

		a->page = this->page;

		return a;

	}//Copy()	

}//advanced2d
