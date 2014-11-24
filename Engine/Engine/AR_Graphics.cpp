#include "Advanced2D.h"

//only one type of collision box per frame

namespace Advanced2D
{
	/////////////////////////
	//  "VECTOR" GRAPHICS  //
	/////////////////////////

	void AR_Graphics::draw_dot(float x, float y, float offsetx, float offsety, D3DCOLOR color)
	{
		x += (int)offsetx;
		y += (int)offsety;

		RECT srcRect = {0,0,4,4};
		D3DXMATRIX mat;
		D3DXVECTOR2 trans((int)(x-2),(int)(y-2));
		D3DXVECTOR2 axis(2,2);		

		D3DXMatrixTransformation2D(&mat,NULL,0,NULL,&axis,0.785f,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);

		g_engine->getSpriteHandler()->Draw(g_engine->gui->default_t->GetTexture(),&srcRect,NULL,NULL,color);

	}//draw_dot()

	void AR_Graphics::draw_line(float x1, float y1, float x2, float y2, float offsetx, float offsety, D3DCOLOR color)
	{
		offsetx = (int)(offsetx);
		offsety = (int)(offsety);

		x1 += offsetx;
		y1 += offsety;
		x2 += offsetx;
		y2 += offsety;

		RECT srcRect = {0,0,1,1};
		D3DXMATRIX mat;
		D3DXVECTOR2 trans;
		trans.x = (int)x1;
		trans.y = (int)y1;
		D3DXVECTOR2 scale;
		scale.x = g_engine->math->Distance(x1,y1,x2,y2);
		scale.y = 1;
		float rotation = g_engine->math->AngleToTarget(x1,y1,x2,y2);

		D3DXMatrixTransformation2D(&mat,NULL,0,&scale,NULL,rotation,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);

		g_engine->getSpriteHandler()->Draw(g_engine->gui->default_t->GetTexture(),&srcRect,NULL,NULL,color);

	}//draw_line()

	void AR_Graphics::draw_box(float x, float y, float w, float h, float offsetx, float offsety, D3DCOLOR color)
	{
		this->draw_line(x, y, x+w, y, offsetx, offsety, color);
		this->draw_line(x+w, y, x+w, y+h, offsetx, offsety, color);
		this->draw_line(x+w, y+h, x, y+h, offsetx, offsety, color);
		this->draw_line(x, y+h, x, y, offsetx, offsety, color);

	}//draw_box()

	void AR_Graphics::draw_xbox(float x,float y,float w,float h,float offsetx,float offsety,D3DCOLOR color)
	{
		this->draw_box(x,y,w,h,offsetx,offsety,color);
		this->draw_line(x,y,x+w,y+h,offsetx,offsety,color);
		this->draw_line(x+w-1,y,x,y+h-1,offsetx,offsety,color);

	}//draw_xbox()

	void AR_Graphics::draw_circle(float x, float y, float r, float offsetx, float offsety, D3DCOLOR color)
	{
		/*x += offsetx;
		y += offsety;		

		D3DXVECTOR2 line[17];

		for(int i=0;i<17;i++)
		{
			float angle = 0 + i*22.5;
			int dot_x = x + cos((double)g_engine->math->toRadians(angle))*r;
			int dot_y = y + sin((double)g_engine->math->toRadians(angle))*r;
			line[i].x = dot_x;
			line[i].y = dot_y;
			this->draw_dot(dot_x,dot_y,0,0,0xFF0000FF);
		}

		g_engine->p_line->Draw(line,17,color);//vector,number of points,color
		*/

	}//draw_circle()

	////////////////////////
	//  TEXTURE GRAPHICS  //
	////////////////////////

	void AR_Graphics::Render2D_ShaderStart(std::string shader)
	{
		g_engine->ar_graphics->Render2D_Stop();
		g_engine->p_effect->SetTechnique(shader.c_str());
		UINT passes = 0;
		g_engine->p_effect->Begin(&passes, 0);
		g_engine->ar_graphics->Render2D_Start();
		g_engine->p_effect->BeginPass(0);

	}

	void AR_Graphics::Render2D_ShaderStop()
	{
		g_engine->ar_graphics->Render2D_Stop();
		g_engine->p_effect->EndPass();
		g_engine->p_effect->End();
		g_engine->ar_graphics->Render2D_Start();
	}

	void AR_Graphics::transform_sprite(Sprite *a, float offsetx, float offsety)
	{
		//add private multiplier
		offsetx *= a->getOffsetMX();
		offsety *= a->getOffsetMY();

		int width = a->getWidth();
		int height = a->getHeight();

		//set transformation matrix
		D3DXMATRIX mat;
		D3DXVECTOR2 scale;
		D3DXVECTOR2 trans;
		D3DXVECTOR2 rotation_axis;
		float rotation = 0;

		if (a->rotate == false)//NORMAL
		{			
			rotation = (float)a->getRotation();
			scale.x = (float)a->getScaleX();
			scale.y = (float)a->getScaleY();

			trans.x = (int)((float)a->getX()+(float)offsetx);
			trans.y = (int)((float)a->getY()+(float)offsety);
			rotation_axis.x = (float)(width*a->getScaleX())/2;
			rotation_axis.y = (float)(height*a->getScaleY())/2;

			if (a->flip_h)//horizontal texture flip, rotation angle and axis also
			{
				scale.x = -(float)a->getScaleX();
				rotation_axis.x = -(float)width*a->getScaleX()/2;
				trans.x = (int)((float)a->getX()+(float)a->w()+(float)offsetx);
			}

			if (a->flip_v)//vertical texture flip
			{
				scale.y = -(float)a->getScaleY();
				rotation_axis.y = -(float)height*a->getScaleY()/2;
				trans.y = (int)((float)a->getY()+(float)a->h()+(float)offsety);				
			}
		}
		else//ROTATING
		{
			scale.y = (float)a->getScaleY();
			trans.y = (float)a->getY()-(float)a->getRotateY_real()+(float)offsety;
			rotation_axis.y = (float)a->getRotateY_real();

			if (a->flip_h)
			{
				scale.x = -(float)a->getScaleX();				
				trans.x = (float)a->getX()+(float)a->getRotateX_real()+(float)offsetx;				
				rotation_axis.x = -(float)a->getRotateX_real();				
				rotation = -(float)(3.141592654-a->getRotation());
			}
			else
			{
				scale.x = (float)a->getScaleX();				
				trans.x = (float)a->getX()-(float)a->getRotateX_real()+(float)offsetx;				
				rotation_axis.x = (float)a->getRotateX_real();				
				rotation = (float)a->getRotation();
			}
		}
		
		D3DXMatrixTransformation2D(&mat,NULL,0,&scale,&rotation_axis,rotation,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);

	}	

	void AR_Graphics::draw_sprite(Sprite *a, float offsetx, float offsety)
	{
		if (a->getImage()==NULL) return;
		
		offsetx = (int)(offsetx*a->getOffsetMX());
		offsety = (int)(offsety*a->getOffsetMY());

		//part of the texture to be rendered
		RECT srcRect = {a->getFrameX(),a->getFrameY(),a->getFrameX()+a->getWidth(),a->getFrameY()+a->getHeight()};

		//trensform sprite
		this->transform_sprite(a,offsetx,offsety);

		//draw sprite
		if (a->ShaderOn) Render2D_ShaderStart(a->shader);
		g_engine->getSpriteHandler()->Draw(a->getImage()->GetTexture(),&srcRect,NULL,NULL,a->getColor_Texture());
		if (a->ShaderOn) Render2D_ShaderStop();

	}//draw_sprite

	void AR_Graphics::draw_sprite_tile(Sprite *a, float offsetx, float offsety)
	{
		if (a->getImage()==NULL) return;

		offsetx = (int)(offsetx*a->getOffsetMX());
		offsety = (int)(offsety*a->getOffsetMY());

		int width = a->getImage()->tile_width*a->getScaleX();
		int height = a->getImage()->tile_height*a->getScaleY();
		
		bool rows_rest = false;
		bool columns_rest = false;

		int rows = (int)a->w()/width;
		if ((int)a->w() % width != 0)
		{
			rows++;
			rows_rest = true;
		}

		int columns = (int)a->h()/height;
		if ((int)a->h() % height != 0)
		{
			columns++;
			columns_rest = true;
		}

		int draw_width = 0;
		int draw_height = 0;		

		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<columns;j++)
			{
				if (i==rows-1 && rows_rest)
					draw_width = (int)a->w() % width;
				else
					draw_width = a->getImage()->tile_width;
				
				if (j==columns-1 && columns_rest)
					draw_height = (int)a->h() % height;
				else
					draw_height = a->getImage()->tile_height;

				//part of the texture to be rendered
				RECT srcRect = {a->getFrameX(),a->getFrameY(),a->getFrameX()+draw_width,a->getFrameY()+draw_height};

				////////////////set transformation matrix				
				D3DXMATRIX mat;
				D3DXVECTOR2 scale;
				D3DXVECTOR2 trans;
				D3DXVECTOR2 rotation_axis;
				rotation_axis.x = draw_width/2;
				rotation_axis.y = draw_height/2;

				float rotation = (float)a->getRotation();				

				//NORMAL
				scale.x = (float)a->getScaleX();
				scale.y = (float)a->getScaleY();

				trans.x = (int)((float)(a->getX()+offsetx+i*width));
				trans.y = (int)((float)(a->getY()+offsety+j*height));				

				if (a->flip_h)//horizontal texture flip
				{
					scale.x = -(float)a->getScaleX();
					trans.x = (int)((float)(a->getX()+offsetx+i*width+width));					
				}

				if (a->flip_v)//vertical texture flip
				{
					scale.y = -(float)a->getScaleY();
					trans.y = (int)((float)(a->getY()+offsety+j*height+height));					
				}		

				D3DXMatrixTransformation2D(&mat,NULL,0,&scale,&rotation_axis,rotation,&trans);
				g_engine->getSpriteHandler()->SetTransform(&mat);
				/////////////

				g_engine->getSpriteHandler()->Draw(a->getImage()->GetTexture(),&srcRect,NULL,NULL,a->getColor_Texture());
			}
		}		

	}//draw_sprite_tile()

	void AR_Graphics::draw_texture(Texture *a, int x, int y, int w ,int h, int left, int top, int w_right, int h_bottom, float scale_x, float scale_y, D3DCOLOR color)
	{
		if (a->GetTexture()==NULL) return;

		RECT srcRect;

		srcRect.left = left;
		srcRect.top = top;
		srcRect.right = left+w_right;
		srcRect.bottom = top+h_bottom;

		D3DXMATRIX mat;
		D3DXVECTOR2 trans((float)x, (float)y);
		D3DXVECTOR2 scale((float)scale_x, (float)scale_y);
		D3DXMatrixTransformation2D(&mat,NULL,0,&scale,NULL,0,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);

		g_engine->getSpriteHandler()->Draw(a->GetTexture(),&srcRect,NULL,NULL,color);

	}//draw_texture	

	void AR_Graphics::draw_scroller_x(Sprite *a,float offsetx, float offsety)
	{
		//add private multiplier
		//dodan int jer se javljala "sjena" okolo spritea prilikom iscrtavanja
		offsetx *= a->getOffsetMX();
		offsety *= a->getOffsetMY();

		//kad je na rubu jedan pixel pomaka
		offsetx=(int)offsetx;
		offsety=(int)offsety;

		D3DXMATRIX mat;

		//set up the viewport
		int viewportX = a->getX()+offsetx;
		int viewportY = a->getY()+offsety;
		int viewportW = a->getImage()->getWidth();
		int viewportH = a->getImage()->getHeight();

		//determine how to draw the image
		if ((int)a->getScrollX() > a->getImage()->getWidth()) a->setScrollX(0);
		if ((int)a->getScrollX() < 0) a->setScrollX(a->getImage()->getWidth());	

		//rectangle on the texture
		int x = (int)a->getScrollX();
		int y = 0;
		int w = x + (int)viewportW;
		int h = y + (int)a->getImage()->getHeight();
		RECT src1 = {x,y,w,h};

		//position on the screen
		D3DXVECTOR2 trans((int)viewportX,(int)viewportY);
		D3DXMatrixTransformation2D(&mat,NULL,0,NULL,0,0,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);				

		int leftover = a->getImage()->getWidth()- viewportW;		

		//iscrtava se samo kada je prvi pri kraju
		if ((int)a->getScrollX() > leftover)
		{
			////rectangle on the texture
			RECT src11 ={x,y,x+((int)(viewportW-((int)a->getScrollX()-leftover))),h};			
			//draw first rectangle			
			g_engine->getSpriteHandler()->Draw(a->getImage()->GetTexture(),&src11,NULL,NULL,a->getColor_Texture());			

			////rectangle on the texture
			int x = 0; 
			int y = 0;
			int w = (int)(a->getScrollX()-leftover);
			int h = (int)a->getImage()->getHeight();
			RECT src2 = {x,y,w,h};

			//draw second rectangle (leftover)
			D3DXVECTOR2 trans((int)(viewportX+(viewportW-w)),(int)viewportY);
			D3DXMatrixTransformation2D(&mat,NULL,0,NULL,0,0,&trans);
			g_engine->getSpriteHandler()->SetTransform(&mat);		

			if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStart(a->shader);
			g_engine->getSpriteHandler()->Draw(a->getImage()->GetTexture(),&src2,NULL,NULL,a->getColor_Texture());
			if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStop();
		}
		else
		{
			//render
			if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStart(a->shader);
			g_engine->getSpriteHandler()->Draw(a->getImage()->GetTexture(),&src1,NULL,NULL,a->getColor_Texture());
			if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStop();
		}	

	}//draw()

	void AR_Graphics::draw_scroller_y(Sprite *a,float offsetx, float offsety)
	{
		//add private multiplier
		//dodan int jer se javljala "sjena" okolo spritea prilikom iscrtavanja
		offsetx *= a->getOffsetMX();
		offsety *= a->getOffsetMY();

		//kad je na rubu jedan pixel pomaka
		offsetx=(int)offsetx;
		offsety=(int)offsety;

		D3DXMATRIX mat;

		//set up the viewport
		int viewportX = a->getX()+offsetx;
		int viewportY = a->getY()+offsety;
		int viewportW = a->getImage()->getWidth();
		int viewportH = a->getImage()->getHeight();

		//determine how to draw the image
		if ((int)a->getScrollY() > a->getImage()->getHeight()) a->setScrollY(0);
		if ((int)a->getScrollY() < 0) a->setScrollY(a->getImage()->getHeight());

		//rectangle on the texture
		int x = 0;
		int y = (int)a->getScrollY();
		int w = (int)viewportW;
		int h = (int)viewportH;

		RECT src1 = {x,y,w,h};

		//position on the screen
		D3DXVECTOR2 trans((int)viewportX,(int)viewportY);
		D3DXMatrixTransformation2D(&mat,NULL,0,NULL,0,0,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);				

		int leftover = a->getImage()->getHeight()- viewportH;

		if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStart(a->shader);

		//iscrtava se samo kada je prvi pri kraju
		if ((int)a->getScrollY() > leftover)
		{
			////rectangle on the texture
			RECT src11 ={x,y,(int)(viewportW),y+((int)(viewportH-((int)a->getScrollY()-leftover)))};			
			//draw first rectangle			
			g_engine->getSpriteHandler()->Draw(a->getImage()->GetTexture(),&src11,NULL,NULL,a->getColor_Texture());			

			////rectangle on the texture
			x = 0; 
			y = 0;
			w = (int)viewportW;
			h = (int)(a->getScrollY()-leftover);
			RECT src2 = {x,y,w,h};

			//draw second rectangle (leftover)
			D3DXVECTOR2 trans((int)viewportX,(int)(viewportY+(viewportH-h)));
			D3DXMatrixTransformation2D(&mat,NULL,0,NULL,0,0,&trans);
			g_engine->getSpriteHandler()->SetTransform(&mat);	

			//draw second rectangle (leftover)
			if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStart(a->shader);
			g_engine->getSpriteHandler()->Draw(a->getImage()->GetTexture(),&src2,NULL,NULL,a->getColor_Texture());
			if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStop();
		}
		else
		{
			if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStart(a->shader);
			g_engine->getSpriteHandler()->Draw(a->getImage()->GetTexture(),&src1,NULL,NULL,a->getColor_Texture());
			if (a->ShaderOn) g_engine->ar_graphics->Render2D_ShaderStop();
		}		

	}//draw()

	int AR_Graphics::getAlpha(D3DCOLOR color) {return (color & 0xFF000000) >> (6*4);}
	int AR_Graphics::getRed(D3DCOLOR color) {return (color & 0x00FF0000) >> (4*4);}
	int AR_Graphics::getGreen(D3DCOLOR color) {return (color & 0x0000FF00) >> (2*4);}
	int AR_Graphics::getBlue(D3DCOLOR color) {return (color & 0x000000FF);}

	void AR_Graphics::ClearScene(D3DCOLOR color) {g_engine->p_device->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,color,1.0f,0);}

	int AR_Graphics::RenderStart()
	{
		if (!g_engine->p_device) return 0;
		if (g_engine->p_device->BeginScene() != D3D_OK) return 0;
		return 1;
	}

	int AR_Graphics::RenderStop()
	{
		if (!g_engine->p_device) return 0;
		if (g_engine->p_device->EndScene() != D3D_OK) return 0;
		if (g_engine->p_device->Present(NULL, NULL, NULL, NULL) != D3D_OK) return 0;
		return 1;
	}

	int AR_Graphics::Render2D_Start()
	{
		if (g_engine->p_sprite_handler->Begin(D3DXSPRITE_ALPHABLEND) != D3D_OK) return 0;
		else return 1;
	}

	int AR_Graphics::Render2D_Stop()
	{
		g_engine->p_sprite_handler->End();
		return 1;
	}


	void AR_Graphics::draw_tilescroller_x(Sprite *a,float offsetx, float offsety)
	{
		if(!a->texture_vector.empty())
		{
			int scroller_posx = a->getX()+offsetx*a->getOffsetMX();
			int scroller_posy = a->getY()+offsety*a->getOffsetMY();

			std::vector <int> opos_x(a->texture_vector.size(),0);
			int total_width=0;			

			for(int i=0;i<opos_x.size();i++)
			{
				if(i==0)
					opos_x[i]=0;
				else
					opos_x[i]=opos_x[i-1]+a->texture_vector[i-1]->getWidth();				
			}

			for(int i=0;i<a->texture_vector.size();i++)
			{
				total_width += a->texture_vector[i]->getWidth();				
			}

			if(a->getScrollX()>total_width) a->setScrollX(0);
			if(a->getScrollX()<0) a->setScrollX(total_width);

			for(int i=0;i<a->texture_vector.size();i++)
			{
				int x = scroller_posx + (int)(opos_x[i] + a->getScrollX())%total_width;

				if(x+a->texture_vector[i]->getWidth()>scroller_posx+total_width)			
				{
					g_engine->ar_graphics->draw_texture(
						a->texture_vector[i],
						x,scroller_posy,
						(scroller_posx+total_width)-x,a->texture_vector[i]->getHeight(),
						0,0,
						(scroller_posx+total_width)-x,a->texture_vector[i]->getHeight(),
						1,1,
						a->getColor_Texture());

					g_engine->ar_graphics->draw_texture(
						a->texture_vector[i],
						scroller_posx,scroller_posy,
						a->texture_vector[i]->getWidth()-((scroller_posx+total_width)-x),a->texture_vector[i]->getHeight(),
						(scroller_posx+total_width)-x,0,
						a->texture_vector[i]->getWidth()-((scroller_posx+total_width)-x),a->texture_vector[i]->getHeight(),
						1,1,
						a->getColor_Texture());
				}
				else
				{
					g_engine->ar_graphics->draw_texture(
						a->texture_vector[i],
						x,scroller_posy,
						a->texture_vector[i]->getWidth(),a->texture_vector[i]->getHeight(),
						0,0,
						a->texture_vector[i]->getWidth(),a->texture_vector[i]->getHeight(),
						1,1,
						a->getColor_Texture());
				}
			}
		}

	}//draw()

}//Advanced2D