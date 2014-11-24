#include "AR_DXFont.h"

AR_DXFont::AR_DXFont()
{
	this->dx_font = NULL;
	this->size = 14;
}

bool AR_DXFont::setFont(LPDIRECT3DDEVICE9 d3d_device, LPCTSTR fontname, int height, int width, int weight, bool italic)
{
	HRESULT hr;

	this->size = height;
	this->font_name = fontname;

	D3DXFONT_DESC FontDesc={
		height,					//font character height 
		width,					//font character width
		weight,					//weight of the font 0-1000
		0,						//mip level
		bool(italic),			//italic
		DEFAULT_CHARSET,		//character set
		OUT_TT_PRECIS,			//output precision
		CLIP_DEFAULT_PRECIS,	//output quality
		DEFAULT_PITCH,			//font pitch and family
	};

	sprintf(FontDesc.FaceName,fontname);//font type

	hr = D3DXCreateFontIndirect(d3d_device,&FontDesc,&this->dx_font);		

	if(hr == D3D_OK)
		return true;
	else
		return false;

}//setFont()

void AR_DXFont::write_text(std::string text, float x, float y , float w, float h, char positioning, D3DCOLOR color)
{
	DWORD value;
	
	if(positioning=='R') value = DT_RIGHT;
	else if(positioning=='C') value = DT_CENTER;
	else value = DT_LEFT;

	RECT pos = {(int)x,(int)y,(int)x+(int)w,(int)y+(int)h};
	this->dx_font->DrawText(NULL,text.c_str(),-1,&pos,value,color);

}//write_text

void AR_DXFont::write_stringvector(LPDIRECT3DDEVICE9 d3d_device, std::vector <std::string> text, float x, float y ,float w, float h, char positioning, D3DCOLOR color)
{
	d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	d3d_device->BeginScene();

	for(int i=0;i<text.size();i++)
	{
		this->write_text(text[i],x, y ,w, h ,positioning, color);
		y+=this->size;
	}		

	d3d_device->EndScene();	
	d3d_device->Present(NULL, NULL, NULL, NULL);
}

void AR_DXFont::write_stringvector2(std::vector <std::string> text, float x, float y ,float w, float h, char positioning, int row_d, D3DCOLOR color)
{
	for(int i=0;i<text.size();i++)
	{
		this->write_text(text[i],x,y,w,h,positioning,color);
		y += row_d;
	}		
}

bool AR_DXFontManager::LoadFont(LPDIRECT3DDEVICE9 d3d_device, LPCTSTR fontname, int height, int width, int weight, bool italic)
{
	AR_DXFont *temp_font = new AR_DXFont();
	if(temp_font->setFont(d3d_device, fontname, height, width, weight, italic))
	{
		this->fonts.push_back(temp_font);
		return true;
	}
	else
	{
		delete temp_font;
		return false;
	}
}

AR_DXFont* AR_DXFontManager::GetFont(std::string font_name)
{
	std::vector <AR_DXFont*>::iterator iter = this->fonts.begin();
	while(iter != this->fonts.end())
	{
		AR_DXFont *a = *iter;
		if (a->getFontName()==font_name) return a;
		iter++;
	}

	return NULL ;
}

int AR_DXFontManager::GetFont_Index(std::string font_name)
{
	int i=0;
	std::vector <AR_DXFont*>::iterator iter = this->fonts.begin();
	while(iter != this->fonts.end())
	{
		AR_DXFont *a = *iter;
		if (a->getFontName()==font_name) return i;
		iter++;
		i++;
	}

	return 0 ;//treba napravit zastitu, ako je emtpy pri ispisu GUI nece se ni pozvat
}