#include "..\Engine\Advanced2D.h"
using namespace Advanced2D;

#pragma once

namespace Advanced2D
{
	class Font_Sign
	{
	private:
		char c;
		int x;
		int y;
		int w;
		int h;

	public:
		char getC(){return this->c;};
		void setC(char value){this->c = value;};

		int getX(){return this->x;};
		void setX(int value){this->x = value;};
		int getY(){return this->y;};
		void setY(int value){this->y = value;};

		int getW(){return this->w;};
		void setW(int value){this->w = value;};
		int getH(){return this->h;};
		void setH(int value){this->h = value;};
	};

	class Font
	{
	private:
		Texture *font_map;
		int height;		

	public:
		std::string font_name;		
		std::vector <Font_Sign*> font_signs;		
		
		int getHeight(){return this->height;};
		void setHeight(int value){this->height = value;};
		int row_dist;
		int char_dist;
		float scale;

		Font();

		bool Load(std::string font_file,std::string font_name);

		int getSign(char c);

		float getScale(){return this->scale;};
		int getLength(std::string text,int dist,float scale);
		std::string getFontName(){return this->font_name;};
		
		void PrintSign(char c,int x,int y,int c_x,int c_y,int c_w,int c_h,float scale,D3DXCOLOR color);
		void PrintText(std::string text,int x,int y,float scale,int dist, D3DXCOLOR color);
		void PrintTextRight(std::string text,int x,int y,float scale, int dist, D3DXCOLOR color);	

		void PrintText(std::vector <std::string> text,int x,int y,float scale, int dist, D3DXCOLOR color);
		void PrintTextRight(std::vector <std::string> text,int x,int y,float scale, int dist, D3DXCOLOR color);

	};

	class AR_FontManager
	{
	private:
	public:
		std::vector <Font*> fonts;

		Font* LoadFont(std::string font_file,std::string font_name);

		Font* GetFont(std::string font_name);
		int GetFont_Index(std::string font_name);


	};

}//Advanced2D