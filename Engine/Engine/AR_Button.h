#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class Button : public Entity
	{
	private:
		int gui_class;
		std::string text;

	public:
		Button();
		virtual ~Button();

		bool cursor_inside;
		bool cursor_insidepast;

		int text_limit;
		bool font_png;
		char text_positioning;
		std::vector <Texture*> texture_vector;
		int row_height;
		int row_x;
		int row_y;
		Button *parent;
		Button *child;
		std::vector <Button*> children;
		std::vector <Button*> form;
		Entity *child_sprite;		
		std::string text1;//checkbox yes
		std::string text2;//checkbox no
		std::vector <std::string> page;
		bool clickable;//wont change color
		bool selected;
		void drawtexture(D3DCOLOR);

		//gui_class
		void setGUI(int value) {this->gui_class=value;};
		int getGUI() {return this->gui_class;};

		void setText(std::string text) {this->text=text;};
		std::string getText() {return this->text;};

	};//class Button

}//namespace
