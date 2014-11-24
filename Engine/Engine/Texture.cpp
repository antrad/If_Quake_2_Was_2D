#include "Advanced2D.h"


Texture::Texture()
{
	this->texture = NULL;
	this->file_path = "";	

	this->tilemap = false;	

	this->tile_width = 1;
	this->tile_height = 1;	
	this->tile_offset_x = 0;
	this->tile_offset_y = 0;	
	this->tile_columns = 1;//rusilo se kad je ovdje bilo nula
	this->tile_rows = 1;	
	this->tile_num = 1;
}

Texture::~Texture()
{
	Release();
}

int Texture::getWidth() 
{ 
	return info.Width; 
};

int Texture::getHeight() 
{ 
	return info.Height; 
};

bool Texture::Load(std::string filename, D3DCOLOR transcolor)
{
	this->file_path = filename;	

	//standard Windows return value
	HRESULT result;

	//get width and height from file
	result = D3DXGetImageInfoFromFile(filename.c_str(), &info);

	if (result != D3D_OK)
	{
		texture = NULL;
		g_engine->log_text.push_back("ERROR#TEXTURE - Failed to load texture \"" + filename + "\"");
		return false;
	}

	//create the new texture by loading a bitmap image file
	result = D3DXCreateTextureFromFileEx( 
		g_engine->getDevice(), //Direct3D device object
		filename.c_str(),      //bitmap filename
		info.Width,            //bitmap image width
		info.Height,           //bitmap image height
		1,                     //mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,       //the type of surface (standard)
		D3DFMT_UNKNOWN,        //surface format (default)
		D3DPOOL_DEFAULT,       //memory class for the texture
		D3DX_DEFAULT,          //image filter
		D3DX_DEFAULT,          //mip filter
		0,					   //color key for transparency
		&info,                 //bitmap file info (from loaded file)
		NULL,                  //color palette
		&texture );            //destination texture

	//make sure the texture was loaded correctly
	if (result != D3D_OK)
	{
		texture = NULL;
		g_engine->log_text.push_back("ERROR#TEXTURE - Failed to create texture \"" + filename + "\"");
		return false;
	}

	this->tile_width = info.Width;
	this->tile_height = info.Height;

	g_engine->log_text.push_back("Loading texture -> \"" + filename + "\" ... OK");

	return true;
}

void Texture::Release()
{
	if (texture != NULL) texture->Release();
}
