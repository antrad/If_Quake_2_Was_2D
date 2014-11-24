#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <string>

#pragma once

class AR_Direct3D
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3d_device;		
	LPD3DXSPRITE d3d_spritehandler;

	LPD3DXEFFECT d3d_effect;
	LPD3DXLINE d3d_line;	

public:
	AR_Direct3D();
	~AR_Direct3D();

	LPDIRECT3D9 getD3D() { return this->d3d; }
	LPDIRECT3DDEVICE9 getDevice() { return this->d3d_device; }

	LPD3DXSPRITE getSpriteHandler() { return this->d3d_spritehandler; }
	LPD3DXEFFECT getPixelShader() { return this->d3d_effect; }
	LPD3DXLINE getLineRenderer() { return this->d3d_line; }

	bool create_d3d();
	bool create_device( HWND p_windowhandle, int width, int height, bool fullscreen, bool v_sync );
	bool create_spritehandler();
	bool create_linerenderer();

	bool create_pixelshader(std::string filename);

};//class AR_Direct3D