#include "Advanced2D.h"

#pragma once

class Texture
{
private:
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 texture;
	std::string file_path;
	std::string name;

	bool tilemap;//use as tilemap

public:
	Texture();
	~Texture();

	int tile_width;
	int	tile_height;	
	int	tile_offset_x;
	int	tile_offset_y;	
	int	tile_columns;
	int	tile_rows;	
	int	tile_num;
	
	//name
	std::string getName() {return this->name;}
	void setName(std::string value) {this->name = value;}

	//tilemap texture
	void setTilemap(bool value){this->tilemap=value;}
	bool getTilemap(){return this->tilemap;}

	bool Load(std::string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(255,0,255));
	LPDIRECT3DTEXTURE9 GetTexture() { return texture; }

	int getWidth();
	int getHeight();

	void Release();	

	std::string getFilePath(){return this->file_path;}
	
};
