#include "Advanced2D.h"

namespace Advanced2D
{
	Line::Line(float x1,float y1,float x2,float y2, int layer, D3DCOLOR color)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
		this->layer = layer;
		this->color = color;
	}

	void AR_Rectangle::Set(float w, float h, float rot_x, float rot_y)
	{
		this->distance_a = g_engine->math->Distance(0,0,rot_x,rot_y);
		this->distance_b = g_engine->math->Distance(w,0,rot_x,rot_y);
		this->distance_c = g_engine->math->Distance(w,h,rot_x,rot_y);
		this->distance_d = g_engine->math->Distance(0,h,rot_x,rot_y);

		this->angle_a = g_engine->math->AngleToTarget(rot_x,rot_y,0,0);
		this->angle_b = g_engine->math->AngleToTarget(rot_x,rot_y,w,0);
		this->angle_c = g_engine->math->AngleToTarget(rot_x,rot_y,w,h);
		this->angle_d = g_engine->math->AngleToTarget(rot_x,rot_y,0,h);

		this->distance_a2 = g_engine->math->Distance(0,0,w-rot_x,rot_y);
		this->distance_b2 = g_engine->math->Distance(w,0,w-rot_x,rot_y);
		this->distance_c2 = g_engine->math->Distance(w,h,w-rot_x,rot_y);
		this->distance_d2 = g_engine->math->Distance(0,h,w-rot_x,rot_y);

		this->angle_a2 = g_engine->math->AngleToTarget(w-rot_x,rot_y,0,0);
		this->angle_b2 = g_engine->math->AngleToTarget(w-rot_x,rot_y,w,0);
		this->angle_c2 = g_engine->math->AngleToTarget(w-rot_x,rot_y,w,h);
		this->angle_d2 = g_engine->math->AngleToTarget(w-rot_x,rot_y,0,h);
	}

	void AR_Rectangle::Draw(float cx, float cy, float angle_r, bool flip_h, float offsetx, float offsety, D3DCOLOR color)
	{
		cx += offsetx;
		cy += offsety;

		g_engine->ar_graphics->draw_line(
			this->getX_A(cx,angle_r,flip_h), this->getY_A(cy,angle_r,flip_h),
			this->getX_B(cx,angle_r,flip_h), this->getY_B(cy,angle_r,flip_h),
			0, 0, color);

		g_engine->ar_graphics->draw_line(
			this->getX_B(cx,angle_r,flip_h), this->getY_B(cy,angle_r,flip_h),
			this->getX_C(cx,angle_r,flip_h), this->getY_C(cy,angle_r,flip_h),
			0, 0, color);

		g_engine->ar_graphics->draw_line(
			this->getX_C(cx,angle_r,flip_h), this->getY_C(cy,angle_r,flip_h),
			this->getX_D(cx,angle_r,flip_h), this->getY_D(cy,angle_r,flip_h),
			0, 0, color);

		g_engine->ar_graphics->draw_line(
			this->getX_D(cx,angle_r,flip_h), this->getY_D(cy,angle_r,flip_h),
			this->getX_A(cx,angle_r,flip_h), this->getY_A(cy,angle_r,flip_h),
			0, 0, color);		

		if(g_engine->display_rotationaxis) g_engine->ar_graphics->draw_dot(cx,cy,0,0,color);		

	}	

	float AR_Rectangle::getX_A(float cx, float angle_r, bool flip_h)
	{
		if(!flip_h) return cx + cos(angle_r+this->angle_a)*this->distance_a;
		else
		{
			angle_r -= 3.141592654;
			return cx + cos(angle_r+this->angle_a2)*this->distance_a2;
		}
	}

	float AR_Rectangle::getY_A(float cy, float angle_r, bool flip_h)
	{
		if(!flip_h) return cy + sin(angle_r+this->angle_a)*this->distance_a;
		else
		{
			angle_r -= 3.141592654;

			return cy + sin(angle_r+this->angle_a2)*this->distance_a2;
		}
	}

	float AR_Rectangle::getX_B(float cx, float angle_r, bool flip_h)
	{
		if(!flip_h) return cx + cos(angle_r+this->angle_b)*this->distance_b;
		else
		{
			angle_r -= 3.141592654;
			return cx + cos(angle_r+this->angle_b2)*this->distance_b2;
		}
	}

	float AR_Rectangle::getY_B(float cy, float angle_r, bool flip_h)
	{
		if(!flip_h) return cy + sin(angle_r+this->angle_b)*this->distance_b;
		else
		{
			angle_r -= 3.141592654;
			return cy + sin(angle_r+this->angle_b2)*this->distance_b2;
		}
	}

	float AR_Rectangle::getX_C(float cx, float angle_r, bool flip_h)
	{
		if(!flip_h) return cx + cos(angle_r+this->angle_c)*this->distance_c;
		else
		{
			angle_r -= 3.141592654;
			return cx + cos(angle_r+this->angle_c2)*this->distance_c2;
		}
	}

	float AR_Rectangle::getY_C(float cy, float angle_r, bool flip_h)
	{
		if(!flip_h) return cy + sin(angle_r+this->angle_c)*this->distance_c;
		else
		{
			angle_r -= 3.141592654;
			return cy + sin(angle_r+this->angle_c2)*this->distance_c2;
		}
	}

	float AR_Rectangle::getX_D(float cx, float angle_r, bool flip_h)
	{
		if(!flip_h) return cx + cos(angle_r+this->angle_d)*this->distance_d;
		else
		{
			angle_r -= 3.141592654;
			return cx + cos(angle_r+this->angle_d2)*this->distance_d2;
		}
	}

	float AR_Rectangle::getY_D(float cy, float angle_r, bool flip_h)
	{
		if(!flip_h) return cy + sin(angle_r+this->angle_d)*this->distance_d;
		else
		{
			angle_r -= 3.141592654;
			return cy + sin(angle_r+this->angle_d2)*this->distance_d2;
		}
	}

}