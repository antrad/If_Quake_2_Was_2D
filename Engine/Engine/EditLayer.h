#include "Advanced2D.h"

#define EDITLAYER_CLASS -199

#define EDITLAYER_VISIBLE -200
#define EDITLAYER_COLLIDABLE -201
#define EDITLAYER_SHADER -202
#define EDITLAYER_OFFSET_MX -203
#define EDITLAYER_OFFSET_MY -204
#define EDITLAYER_CLEAR -205
#define EDITLAYER_RED -206
#define EDITLAYER_GREEN -207
#define EDITLAYER_BLUE -208
#define EDITLAYER_DARK -209

#pragma once

namespace Advanced2D
{
	class EditLayer
	{
	private:
	public:
		Button *main_form;

		//CHECKBOX
		std::vector <Button*> visible;
		std::vector <Button*> collidable;
		//TEXTBOX
		std::vector <Button*> shader;
		std::vector <Button*> offset_mx;
		std::vector <Button*> offset_my;
		Button *darkness;

		//BUTTON
		std::vector <Button*> clear;
		//SLIDER
		Button *b_red;
		Button *b_green;
		Button *b_blue;		

		EditLayer();
		~EditLayer();
		void setNULL();

		void Init();
		void Reset(int &r, int &g, int &b);
		void LayerData_load(std::string filename, int &r, int &g,int &b);
		void LayerData_update(Button *button);

		void ButtonPress(Button *button,int mousebutton, int &r, int &g,int &b);
		void ButtonRelease(Button *button,int mousebutton);

	};

}//Advanced2D