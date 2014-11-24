#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Screen.h"

Screen::Screen()
{
	this->screen_x = 0;
	this->screen_y = 0;
	this->screen_w = 0;
	this->screen_h = 0;

	this->border_left = 0;
	this->border_right = 0;
	this->border_up = 0;
	this->border_down = 0;

	this->offset_x = 0;
	this->offset_y = 0;

	this->RenderTexture = NULL;
	this->RenderSurface = NULL;
	this->DarkTexture = NULL;
	this->DarkSurface = NULL;
	this->DarkTextureBack = NULL;
	this->DarkSurfaceBack = NULL;

	this->color = 0xFFFFFFFF;
}

void Screen::Init(int x, int y, int w, int h)
{
	this->screen_x = x;
	this->screen_y = y;
	this->screen_w = w;
	this->screen_h = h;

	if (this->RenderTexture)
	{
		this->RenderTexture->Release();//treba li release surface?
		this->DarkTexture->Release();
		this->DarkTextureBack->Release();

		this->RenderTexture = NULL;
		this->RenderSurface = NULL;
		this->DarkTexture = NULL;
		this->DarkSurface = NULL;
		this->DarkTextureBack = NULL;
		this->DarkSurfaceBack = NULL;
	}

	//D3DFMT_A8R8G8B8,D3DFMT_R5G6B5

	//screen
	g_engine->p_device->CreateTexture(this->screen_w,this->screen_h,
		1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&this->RenderTexture,NULL);	
	this->RenderTexture->GetSurfaceLevel(0,&this->RenderSurface);

	//darkness
	g_engine->p_device->CreateTexture(this->screen_w,this->screen_h,
		1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&this->DarkTexture,NULL);	
	this->DarkTexture->GetSurfaceLevel(0,&this->DarkSurface);

	//darkness back
	g_engine->p_device->CreateTexture(this->screen_w,this->screen_h,
		1,D3DUSAGE_RENDERTARGET,D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT,&this->DarkTextureBack,NULL);	
	this->DarkTextureBack->GetSurfaceLevel(0,&this->DarkSurfaceBack);

}//Init()

void Screen::SetBorders(float left, float right, float up, float down)
{
	this->border_left = left;
	this->border_right = right;
	this->border_up = up;
	this->border_down = down;

}//SetBorders()

void Screen::setOffset(float x, float y, float shift_x, float shift_y)
{
	//X i Y OFFSET = PLAYER + MOUSE
	this->offset_x = -x + this->getWidth()/2;
	this->offset_y = -y + this->getHeight()/2;
	
	//mouse bi trebala biti opcija
	this->offset_x += shift_x; 
	this->offset_y += shift_y;	

	//SCREEN BORDERS
	//X
	if (this->offset_x > -g_engine->game_maps[g_engine->map_current]->border_left)
		this->offset_x = -g_engine->game_maps[g_engine->map_current]->border_left;
	if (this->offset_x < -g_engine->game_maps[g_engine->map_current]->border_right+this->getWidth())
		this->offset_x = -g_engine->game_maps[g_engine->map_current]->border_right+this->getWidth();
	//Y
	if (this->offset_y > -g_engine->game_maps[g_engine->map_current]->border_up)
		this->offset_y = -g_engine->game_maps[g_engine->map_current]->border_up;
	if (this->offset_y < -this->border_down+this->getHeight())
		this->offset_y = -this->border_down+this->getHeight();

		/*
	//SCREEN BORDERS
	//X
	if (this->offset_x > -this->border_left)
		this->offset_x = -this->border_left;
	if (this->offset_x < -this->border_right+this->getWidth())
		this->offset_x = -this->border_right+this->getWidth();
	//Y
	if (this->offset_y > -this->border_up)
		this->offset_y = -this->border_up;
	if (this->offset_y < -this->border_down+this->getHeight())
		this->offset_y = -this->border_down+this->getHeight();
		*/

}//setOffset()

void Screen::Draw()
{
	D3DXMATRIX mat;
	D3DXVECTOR2 trans(this->screen_x,this->screen_y);
	D3DXMatrixTransformation2D(&mat,NULL,0,NULL,0,0,&trans);
	RECT srcRect = {0,0,this->screen_w,this->screen_h};

	g_engine->getSpriteHandler()->SetTransform(&mat);
	g_engine->getSpriteHandler()->Draw(this->RenderTexture,&srcRect,NULL,NULL,0xFFFFFFFF);	

}

void Screen::DrawDark()
{
	D3DXMATRIX mat;
	D3DXVECTOR2 trans(this->screen_x,this->screen_y);
	D3DXMatrixTransformation2D(&mat,NULL,0,NULL,0,0,&trans);
	RECT srcRect = {0,0,this->screen_w,this->screen_h};	
	
	g_engine->getSpriteHandler()->SetTransform(&mat);
	g_engine->getSpriteHandler()->Draw(this->DarkTexture,&srcRect,NULL,NULL,0xFFFFFFFF);
}

void Screen::DrawDarkBack()
{
	D3DXMATRIX mat;
	D3DXVECTOR2 trans(0,0);//screen je vec pomaknut, pa valjda ne treba i ovo micat
	D3DXMatrixTransformation2D(&mat,NULL,0,NULL,0,0,&trans);
	RECT srcRect = {0,0,this->screen_w,this->screen_h};	
	
	g_engine->getSpriteHandler()->SetTransform(&mat);
	g_engine->getSpriteHandler()->Draw(this->DarkTextureBack,&srcRect,NULL,NULL,0xFFFFFFFF);
}

void Screen::Draw_ToScreen()
{
	g_engine->p_device->SetRenderTarget(0,this->RenderSurface);
	g_engine->ar_graphics->ClearScene(this->color);
	
	g_engine->p_device->BeginScene();
	g_engine->ar_graphics->Render2D_Start();

	for(unsigned int i=0;i<g_engine->game_sprites.size();i++)
	{
		if(g_engine->game_sprites[i]->getShader()!="#") g_engine->ar_graphics->Render2D_ShaderStart(g_engine->game_sprites[i]->getShader());
		g_engine->game_sprites[i]->draw(g_engine->getRenderType());
		if(g_engine->game_sprites[i]->getShader()!="#") g_engine->ar_graphics->Render2D_ShaderStop();
	}		

	g_engine->ar_graphics->Render2D_Stop();
	g_engine->p_device->EndScene();

}