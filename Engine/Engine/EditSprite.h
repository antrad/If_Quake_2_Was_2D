#include "Advanced2D.h"

#define EDITSPRITE_CLASS -99

#define POS_X -100
#define POS_Y -101
#define SIZE_X -102
#define SIZE_Y -103
#define COLLIDABLE -104
#define SCALE_X -106
#define SCALE_Y -107
#define ROTATE -108
#define RENDER_TYPE -109
#define SCROLL -110
#define MY_CLASS -111
#define TYPE -112
#define ID -113
#define VEL_X -114
#define VEL_Y -115
#define SET_TEXT -116
#define STATE -117
#define B_ALPHA -119
#define B_RED -120
#define B_GREEN -121
#define B_BLUE -122
#define B_OFFSET_MX -123
#define B_OFFSET_MY -124
#define B_FLIPH -125
#define B_FLIPV -126
#define B_FRAME_RELX -127
#define B_FRAME_RELY -128
#define B_ENERGY -129
#define B_DIFFICULTY -130
//set text u command

#pragma once

namespace Advanced2D
{
	class EditSprite
	{
	private:
	public:
		Button *main_form;

		Button *pos_x;
		Button *pos_y;
		Button *size_x;
		Button *size_y;
		Button *collidable;

		Button *scale_x;
		Button *scale_y;

		Button *rotate;
		Button *render_type;
		Button *scroll;
		Button *offset_mx;
		Button *offset_my;
		Button *flip_h;
		Button *flip_v;
		Button *frame_relx;
		Button *frame_rely;

		Button *my_class;
		Button *type;
		Button *id;
		Button *state;

		Button *vel_x;
		Button *vel_y;

		Button *command;

		Button *b_alpha;
		Button *b_red;
		Button *b_green;
		Button *b_blue;

		Button *energy;
		Button *difficulty;

		EditSprite();
		~EditSprite();
		void setNULL();

		void Init();
		void Update_form(Sprite *a);
		void Update_sprite(Sprite *a,Button *button);

		void ButtonPress(Button *button,int mousebutton);
		void ButtonRelease(Button *button,int mousebutton);

	};

}//Advanced2D