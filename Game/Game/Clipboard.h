#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#pragma once

namespace Advanced2D
{
	class Clipboard
	{
	public:
		Clipboard();		

		bool grid;
		int grid_size;
		int working_layer;
		bool mouse_pressed;
		
		//CLIPBOARD
		std::vector <Sprite*> clipboard;		
		
		void Copy_sprite(Sprite *a,std::string command);
		void Copy(std::string command, int layer, bool clear);
		void CopyAll(std::string command);
		void Paste();
		void Delete(int layer);
		void DeleteAll();
		void Draw();
		void Clear();		
		
		//SELECTION BOX		
		void SelectionBox_position(int cursor_x,int cursor_y);
		bool SelectionBox_inside(Sprite *a,int x1,int y1,int x2,int y2);
		void SelectionBox_draw();

		std::vector <int> clipboard_relposx;
		std::vector <int> clipboard_relposy;

		std::vector <int> clipboard_box_x1;
		std::vector <int> clipboard_box_y1;
		std::vector <int> clipboard_box_x2;
		std::vector <int> clipboard_box_y2;

		int getX1(int layer)
		{
			if (this->clipboard_box_x1[layer]<this->clipboard_box_x2[layer])
				return this->clipboard_box_x1[layer];
			else
				return this->clipboard_box_x2[layer];
		}

		int getX2(int layer)
		{
			if (this->clipboard_box_x2[layer]>this->clipboard_box_x1[layer])
				return this->clipboard_box_x2[layer];
			else
				return this->clipboard_box_x1[layer];
		}

		int getY1(int layer)
		{
			if (this->clipboard_box_y1[layer]<this->clipboard_box_y2[layer])
				return this->clipboard_box_y1[layer];
			else
				return this->clipboard_box_y2[layer];
		}

		int getY2(int layer)
		{
			if (this->clipboard_box_y2[layer]>this->clipboard_box_y1[layer])
				return this->clipboard_box_y2[layer];
			else
				return this->clipboard_box_y1[layer];
		}

		//CONTROLS
		void keyPress(int key);
		void keyRelease(int key);
		void mousePress(int button);
		void mouseRelease(int button);
		void mouseWheel(int wheel);
		void buttonPress(Button *button, int mousebutton);
		void buttonRelease(Button *button, int mousebutton);

		//MISC
		int getFrame(int x,int y,int w,int h,int column_num,int frame_offsetx,int frame_offsety);
		int getGridX(int x, int layer);
		int getGridY(int y, int layer);
		int getFramePositionX(int x,int w,int frame_offsetx);
		int getFramePositionY(int y,int h,int frame_offsety);
		void Grid_draw();
		void Draw_parent_child(Sprite *a);
		bool dotcollision(double x, double y, int entity_list);
		void movesprite(Sprite *a);

	};//class Clipboard
}