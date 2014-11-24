#include "Advanced2D.h"

namespace Advanced2D {

	Button::Button()
	{		
		this->cursor_inside = false;
		this->cursor_insidepast = false;
		this->text_limit = 256;
		this->font_png = false;
		this->text_positioning = 'C';

		this->row_height = 20;
		//offset for text
		this->row_x = 2;
		this->row_y = 2;

		this->parent = NULL;
		this->child = NULL;
		this->child_sprite = NULL;

		this->selected=false;		
		this->clickable=true;

		this->setText("");
		this->text1="";
		this->text2="";

	}

	Button::~Button(){}

	void Button::drawtexture(D3DCOLOR color)
	{
		D3DXMATRIX mat;
		D3DXVECTOR2 trans((float)getX(),(float)getY());
		D3DXVECTOR2 scale((float)getScaleX(),(float)getScaleY());
		D3DXMatrixTransformation2D(&mat,NULL,0,&scale,NULL,0,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);

		//calculate frame position and size
		int fx = (this->getFrameCurrent() % this->getFrameColumns()) * this->getImage()->tile_width;
		int fy = (this->getFrameCurrent() / this->getFrameColumns()) * this->getImage()->tile_height;
		fx += (this->getFrameCurrent() % this->getFrameColumns()) * this->getImage()->tile_offset_x;
		fy += (this->getFrameCurrent() / this->getFrameColumns()) * this->getImage()->tile_offset_y;

		RECT srcRect;
		srcRect.left = fx;
		srcRect.top = fy;
		
		if(this->getScaleX()==1)
			srcRect.right = fx + this->getWidth();
		else
			srcRect.right = fx + this->getImage()->getWidth();
		
		if(this->getScaleY()==1)
			srcRect.bottom = fy + this->getHeight();			
		else
			srcRect.bottom = fy + this->getImage()->getHeight();			
		
		g_engine->getSpriteHandler()->Draw(this->getImage()->GetTexture(),&srcRect,NULL,NULL,color);

	}

}//Advanced2D