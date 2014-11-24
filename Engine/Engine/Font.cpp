#include "..\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Font.h"

namespace Advanced2D
{
	Font::Font()
	{
		this->scale = 1;
		this->font_map = NULL;
		this->height = 0;
		this->char_dist = 0;
		this->row_dist = 0;
	}	

	bool Font::Load(std::string font_file,std::string font_name)
	{
		std::string temp_font_file = font_file + ".txt";

		std::ifstream filein(temp_font_file.c_str());
		if(!filein.is_open())
		{
			g_engine->message("Error loading " + font_file + ".txt");
			return false;
		}

		this->font_map = new Texture();
		if (!this->font_map->Load(font_file + ".png"))
		{
			g_engine->message("Error loading " + font_file + ".png");
			return false;
		}
		
		std::string line;	
		while(std::getline(filein,line))
		{
			char c;
			int x;
			int y;
			int w;
			int h;
			std::string temp;

			std::stringstream stream(line);

			size_t found0;
			found0 = line.find("height");
			size_t found1;
			found1 = line.find("row_dist");
			size_t found2;
			found2 = line.find("char_dist");
			size_t found3;
			found3 = line.find("space");


			if (found0!=string::npos)//check for height
			{
				if (stream >> temp >> h)
					this->setHeight(h);
				else
				{
					g_engine->log_text.push_back("ERROR#FONT - Failed to read font height data \"" + font_name + "\"");
					return false;
				}
			}
			else if (found1!=string::npos)//check for row_dist
			{
				if (stream >> temp >> h)
					this->row_dist = h;
				else
				{
					g_engine->log_text.push_back("ERROR#FONT - Failed to read font row_dist data \"" + font_name + "\"");
					return false;
				}
			}
			else if (found2!=string::npos)//check for char_dist
			{
				if (stream >> temp >> h)
					this->char_dist = h;
				else
				{
					g_engine->log_text.push_back("ERROR#FONT - Failed to read font char_dist data \"" + font_name + "\"");
					return false;
				}
			}
			else if (found3!=string::npos)//check for space
			{
				if (stream >> temp >> x >> y >> w >> h)//load char
				{
					Font_Sign *temp = new Font_Sign;
					temp->setC(' ');
					temp->setX(x);
					temp->setY(y);
					temp->setW(w);
					temp->setH(h);

					this->font_signs.push_back(temp);
				}
				else
				{
					g_engine->log_text.push_back("ERROR#FONT - Failed to read font char data \"" + font_name + "\"");
					return false;
				}
			}
			else
			{
				if (stream >> c >> x >> y >> w >> h)//load char
				{
					Font_Sign *temp = new Font_Sign;
					temp->setC(c);
					temp->setX(x);
					temp->setY(y);
					temp->setW(w);
					temp->setH(h);

					this->font_signs.push_back(temp);
				}
				else
				{
					g_engine->log_text.push_back("ERROR#FONT - Failed to read font char data \"" + font_name + "\"");
					return false;
				}
			}

		}

		filein.close();

		this->font_name = font_name;

		return true;
	}

	int Font::getSign(char c)
	{
		for(int i=0;i<this->font_signs.size();i++)
		{
			//if(this->font_signs[i]->getC()==c || this->font_signs[i]->getC()==toupper(c))
			//return i; //mala slova == velika
			
			if(this->font_signs[i]->getC()==c) return i;
		}
		return 0;
	}

	void Font::PrintSign(char c,int x,int y,int c_x,int c_y,int c_w,int c_h,float scale,D3DXCOLOR color)
	{
		RECT srcRect;

		srcRect.left = c_x;
		srcRect.top = c_y;
		srcRect.right = c_x + c_w;
		srcRect.bottom = c_y + c_h;

		D3DXMATRIX mat;
		D3DXVECTOR2 scale_m(scale, scale);
		D3DXVECTOR2 trans((float)x, (float)y);		
		D3DXMatrixTransformation2D(&mat,NULL,0,&scale_m,NULL,0,&trans);
		g_engine->getSpriteHandler()->SetTransform(&mat);

		g_engine->getSpriteHandler()->Draw(this->font_map->GetTexture(),&srcRect,NULL,NULL,color);

	}

	void Font::PrintText(std::string text,int x,int y,float scale, int dist, D3DXCOLOR color)
	{
		int j=0;

		for(int i=0;i<text.size();i++)
		{
			j = this->getSign(text[i]);

			this->PrintSign(
				text[i],
				x,y,
				this->font_signs[j]->getX(),this->font_signs[j]->getY(),
				this->font_signs[j]->getW(),this->font_signs[j]->getH(),
				scale*this->scale,color);

			x += (this->font_signs[j]->getW()+ dist*this->scale)*scale*this->scale;
		}

	}

	void Font::PrintText(std::vector <std::string> text,int x,int y,float scale, int dist, D3DXCOLOR color)
	{
		for(int i=0;i<text.size();i++)
		{
			this->PrintText(text[i],x,y+i*(this->getHeight()+this->row_dist)*this->scale,scale,dist,color);			
		}
	}

	void Font::PrintTextRight(std::string text,int x,int y,float scale, int dist, D3DXCOLOR color)
	{
		x -= this->getLength(text,dist,scale);

		this->PrintText(text,x,y,scale,dist,color);

	}

	void Font::PrintTextRight(std::vector <std::string> text,int x,int y,float scale, int dist, D3DXCOLOR color)
	{
		for(int i=0;i<text.size();i++)
		{			
			this->PrintTextRight(text[i],x,y+i*(this->getHeight()+this->row_dist)*this->scale,scale,dist,color);			
		}
	}

	int Font::getLength(std::string text,int dist,float scale)
	{
		int length = 0;

		for(int i=0;i<text.size();i++)
		{
			length += this->font_signs[this->getSign(text[i])]->getW()*scale*this->scale;
		}

		length += (text.size()-1)*dist*this->scale; //razmaci medju slovima

		return length;
	}

	//MANAGER

	Font* AR_FontManager::LoadFont(std::string font_file,std::string font_name)
	{
		Font *temp_font = new Font();
		if(temp_font->Load(font_file,font_name))
		{
			this->fonts.push_back(temp_font);
			return temp_font;
		}
		else
		{
			delete temp_font;
			return NULL;
		}

		return NULL;
	}

	Font* AR_FontManager::GetFont(std::string font_name)
	{
		std::vector <Font*>::iterator iter = this->fonts.begin();
		while(iter != this->fonts.end())
		{
			Font *a = *iter;
			if (a->getFontName()==font_name) return a;
			iter++;
		}

		return NULL ;
	}

	int AR_FontManager::GetFont_Index(std::string font_name)
	{
		int i=0;
		std::vector <Font*>::iterator iter = this->fonts.begin();
		while(iter != this->fonts.end())
		{
			Font *a = *iter;
			if (a->getFontName()==font_name) return i;
			iter++;
			i++;
		}

		return 0 ;//treba napravit zastitu, ako je emtpy pri ispisu GUI nece se ni pozvat
	}	

}//Advanced2D