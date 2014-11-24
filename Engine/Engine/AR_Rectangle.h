#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class AR_Rectangle
	{
	public:
		//A - B
		//|   |
		//D - C

		float distance_a,distance_b,distance_c,distance_d;
		float angle_a,angle_b,angle_c,angle_d;

		float distance_a2,distance_b2,distance_c2,distance_d2;
		float angle_a2,angle_b2,angle_c2,angle_d2;

		void Set(float w, float h, float rot_x, float rot_y);
		void Draw(float cx, float cy, float angle_r, bool flip_h, float offsetx, float offsety, D3DCOLOR color);		

		float getX_A(float cx, float angle_r, bool flip_h);
		float getY_A(float cy, float angle_r, bool flip_h);

		float getX_B(float cx, float angle_r, bool flip_h);
		float getY_B(float cy, float angle_r, bool flip_h);

		float getX_C(float cx, float angle_r, bool flip_h);
		float getY_C(float cy, float angle_r, bool flip_h);

		float getX_D(float cx, float angle_r, bool flip_h);
		float getY_D(float cy, float angle_r, bool flip_h);

	};

	class Line
	{
	public:
		Line(float x1,float y1,float x2,float y2, int layer, D3DCOLOR color);
		float x1;
		float y1;
		float x2;
		float y2;
		int layer;
		D3DCOLOR color;

	};

}