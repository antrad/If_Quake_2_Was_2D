#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

//NEZAVISNO, ALI MI SE NE SVIDJA OVO GORE,
//A I U WRITE TEXT JE BILO SCREEN WIDTH I HEIGHT KOJE SAM SAD SAMO STAVIO KAO 1440X900

//gamedev : The SDK comes with D3DFont in the Common source which is many times faster than D3DXFont. //co to je ???

#pragma once

class AR_DXFont
{
private:
	LPD3DXFONT dx_font;

public:
	std::string font_name;
	int size;

	AR_DXFont();

	bool setFont(LPDIRECT3DDEVICE9 d3d_device, LPCTSTR fontname, int height, int width, int weight, bool italic);

	std::string getFontName(){return this->font_name;};

	void write_text(std::string text, float x, float y , float w, float h, char positioning, D3DCOLOR color);	
	void write_stringvector(LPDIRECT3DDEVICE9 d3d_device,std::vector <std::string> text,float x, float y, float w, float h, char positioning, D3DCOLOR color);
	void write_stringvector2(std::vector <std::string> text, float x, float y , float w, float h, char positioning, int row_d, D3DCOLOR color);

};

class AR_DXFontManager
{
private:
public:
	std::vector <AR_DXFont*> fonts;

	bool LoadFont(LPDIRECT3DDEVICE9 d3d_device, LPCTSTR fontname, int height, int width, int weight, bool italic);

	AR_DXFont* GetFont(std::string font_name);
	int GetFont_Index(std::string font_name);

};