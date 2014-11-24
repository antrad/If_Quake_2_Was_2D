#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#pragma once

class Screen
{
	int screen_x;
	int screen_y;
	int screen_w;
	int screen_h;	

	float offset_x;
	float offset_y;

	LPDIRECT3DTEXTURE9 RenderTexture;
	LPDIRECT3DSURFACE9 RenderSurface;

	LPDIRECT3DTEXTURE9 DarkTexture;
	LPDIRECT3DSURFACE9 DarkSurface;

	LPDIRECT3DTEXTURE9 DarkTextureBack;
	LPDIRECT3DSURFACE9 DarkSurfaceBack;

	D3DCOLOR color;

public:	
	Screen();

	float border_left;
	float border_right;
	float border_up;
	float border_down;

	std::vector <float> border_down_vec;
	std::vector <float> border_x_vec;

	void setColor(D3DCOLOR color) {this->color = color;};

	void setX(int value) {this->screen_x = value;};
	int getX() {return this->screen_x;};
	void setY(int value) {this->screen_y = value;};
	int getY() {return this->screen_y;};
	void setWidth(int value) {this->screen_w = value;};
	int getWidth() {return this->screen_w;};
	void setHeight(int value) {this->screen_h = value;};
	int getHeight() {return this->screen_h;};

	void setOffsetX(int value) {this->offset_x = value;};
	int getOffsetX() {return this->offset_x;};
	void setOffsetY(int value) {this->offset_y = value;};
	int getOffsetY() {return this->offset_y;};

	LPDIRECT3DTEXTURE9 getRenderTexture() {return this->RenderTexture;};
	LPDIRECT3DSURFACE9 getRenderSurface() {return this->RenderSurface;};
	
	LPDIRECT3DTEXTURE9 getDarkTexture() {return this->DarkTexture;};
	LPDIRECT3DSURFACE9 getDarkSurface() {return this->DarkSurface;};

	LPDIRECT3DTEXTURE9 getDarkTextureBack() {return this->DarkTextureBack;};
	LPDIRECT3DSURFACE9 getDarkSurfaceBack() {return this->DarkSurfaceBack;};

	void Init(int x, int y, int w, int h);
	void SetBorders(float left, float right, float up, float down);
	void setOffset(float x, float y, float shift_x, float shift_y);
	void Draw();
	void Draw_ToScreen();
	void DrawDark();
	void DrawDarkBack();
};