#include "AR_Direct3D.h"

AR_Direct3D::AR_Direct3D()
{
	this->d3d = NULL;
	this->d3d_device = NULL;
	this->d3d_spritehandler = NULL;

	this->d3d_effect = NULL;
	this->d3d_line = NULL;
}

AR_Direct3D::~AR_Direct3D()
{
	//release ???
	/*if (d3d) delete d3d;
	if (d3d_device) delete d3d_device;
	if (d3d_spritehandler) delete d3d_spritehandler;
	if (d3d_effect) delete d3d_effect;
	if (d3d_line) delete d3d_line;*/

}

bool AR_Direct3D::create_d3d()
{
	this->d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (this->d3d == NULL) return false;
	else return true;

}

bool AR_Direct3D::create_device( HWND p_windowHandle, int width, int height, bool fullscreen, bool v_sync )
{
	if (this->d3d == NULL) return false;

	//get system desktop color depth
	D3DDISPLAYMODE dm;
	this->d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dm);

	//set configuration options for Direct3D
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = (!fullscreen);
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (v_sync)
		d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_ONE;
	else
		d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

	d3dpp.BackBufferFormat = dm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.hDeviceWindow = p_windowHandle;

	//create Direct3D device
	this->d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		p_windowHandle,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&this->d3d_device);

	if (this->d3d_device == NULL) return false;
	else return true;		

}

bool AR_Direct3D::create_spritehandler()
{
	if (this->d3d_device == NULL) return false;

	D3DXCreateSprite(this->d3d_device, &this->d3d_spritehandler);

	if (this->d3d_spritehandler == NULL) return false;
	else return true;	

}

bool AR_Direct3D::create_linerenderer()
{
	if (this->d3d_device == NULL) return false;

	D3DXCreateLine(this->d3d_device, &this->d3d_line);		

	if (this->d3d_line == NULL) return false;
	else return true;	

}

bool AR_Direct3D::create_pixelshader(std::string filename)
{
	ID3DXBuffer *errors = 0;
	D3DXCreateEffectFromFile( this->d3d_device, filename.c_str(), 0, 0, D3DXSHADER_DEBUG, 0, &this->d3d_effect, &errors);
	if (errors)
	{
		//trebat ce povezat s log file
		//MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
		return false;
	}

	return true;
}