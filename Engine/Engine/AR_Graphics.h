#include "Advanced2D.h"
#include <cmath>

#pragma once

namespace Advanced2D
{
	class AR_Graphics
	{
	private:
	public:
		//VECTOR GRAPHICS
		void draw_dot(float x, float y, float offsetx, float offsety, D3DCOLOR color);
		void draw_line(float x1, float y1, float x2, float y2, float offsetx, float offsety, D3DCOLOR color);		
		void draw_box(float x, float y, float w, float h,float offsetx, float offsety, D3DCOLOR color);
		void draw_xbox(float x,float y,float w,float h,float offsetx,float offsety,D3DCOLOR color);
		void draw_circle(float x,float y,float r,float offsetx, float offsety,D3DCOLOR color);		

		void transform_sprite(Sprite *a, float offsetx, float offsety);		

		void Render2D_ShaderStart(std::string shader);
		void Render2D_ShaderStop();

		int getAlpha(D3DCOLOR color);
		int getRed(D3DCOLOR color);
		int getGreen(D3DCOLOR color);
		int getBlue(D3DCOLOR color);

		void ClearScene(D3DCOLOR color);
		int RenderStart();
		int RenderStop();
		int Render2D_Start();
		int Render2D_Stop();

		void draw_texture(Texture *a, int x, int y, int w ,int h, int left, int top, int w_right, int h_bottom, float scale_x, float scale_y, D3DCOLOR color);

		void draw_sprite(Sprite *a, float offsetx, float offsety);
		void draw_sprite_tile(Sprite *a, float offsetx, float offsety);
		void draw_scroller_x(Sprite *a,float offsetx, float offsety);
		void draw_scroller_y(Sprite *a,float offsetx, float offsety);
		void draw_tilescroller_x(Sprite *a,float offsetx, float offsety);

	};

}//Advanced2D
