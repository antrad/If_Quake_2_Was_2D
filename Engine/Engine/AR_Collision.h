#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class eng_Collision
	{
	private:

	public:
		eng_Collision();

		bool notarget;

		virtual ~eng_Collision();

		void setColl_center(Sprite *a);
		void setColl_line2(Sprite *a);
		void setColl_line3(Sprite *a);
		void setColl_box(Sprite *a);

		void updateColl_center(Sprite *a);
		void updateColl_line2(Sprite *a);		
		void updateColl_line3(Sprite *a);
		void updateColl_box(Sprite *a);

		bool DotInsideSprite(float dot_x, float dot_y, Sprite* a);
		bool DotInsideSpriteBR(float dot_x, float dot_y, Sprite* a);
		bool TestCollision_dot(Sprite* a, Sprite *b);
		void TestForCollisions_dot(Sprite *test_me,int entity_list);

		bool TestCollision_br(Sprite *sprite1, Sprite *sprite2);
		void TestForCollisions_br(Sprite *test_me,int entity_list);

		bool dot_dotrect(float dot_x, float dot_y, float x, float y, float w, float h);
		bool dot_dotrect_eq(float dot_x, float dot_y, float x, float y, float w, float h);
		bool dot_dotrect_nocount(float dot_x, float dot_y, float x, float y, float w, float h);

		//line intersection
		bool line_lineline(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
		bool line_linerect( float x1, float y1, float xx1, float yy1, float x2, float y2, float w2, float h2 );
		bool line_rectrect( float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2 );
		bool line_linerectABCD( float x1, float y1, float xx1, float yy1, float x2, float y2, float w2, float h2, bool &AB, bool &BC, bool &CD, bool &DA );

		void line_linerectxy( float x1, float y1, float xx1, float yy1, float x2, float y2, float w2, float h2,
		bool &AB, bool &BC, bool &CD, bool &DA,
		float &ab_x, float &ab_y, float &bc_x, float &bc_y, float &cd_x, float &cd_y, float &da_x, float &da_y);
		bool line_linelinexy(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float &xxx,float &yyy);

		bool raycasting(Sprite *a, Sprite *target);
		bool raycasting_all(Sprite *a, Sprite *target);

		void collision_line_rectrect(Sprite *test_me, int entity_list);

		bool line_collbox_rectrect(float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy,
		float ex,float ey,float fx,float fy,float gx,float gy,float hx,float hy);

		bool line_collbox_linerectABCD( float x1,float y1,float x2,float y2,float ax,float ay,float bx,float by,float cx,float cy,float dx,float dy,bool &AB, bool &BC, bool &CD, bool &DA );


		void collbox_update(Sprite *a);
		void collbox_update_RXYCbox(Sprite *a);

		Sprite* collision_line_getclosest(float x1,float y1,float x2,float y2,int entity_list, float &coll_x, float &coll_y);
		void collision_list_line_rect(Sprite *test_me,float x1, float y1, float x2, float y2, int entity_list);

		void TestForCollisions_br(Sprite *test_me, int map, int entity_list);
		void TestForCollisions_dot(Sprite *test_me, int map, int entity_list);
	

	};

}//Advanced2D
