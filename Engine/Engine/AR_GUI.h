/* Antonio R.*/

#include "Advanced2D.h"

//system gui buttons classes 100#0
//system gui buttons classes - members 100##
#define CHECKBOX 10000
#define TEXTBOX 10010
#define DROPBOX 10020
#define DROPBOX_ARROW 10021
#define DROPBOX_OPTION 10022
#define FORM_CLOSE 10030
#define SLIDER_BODY 10040
#define SLIDER_H 10041
#define SLIDER_V 10042
#define FORM_AUTOHIDE 10050

extern void GUI_ButtonPress( Advanced2D::Button*, int );
extern void GUI_ButtonRelease( Advanced2D::Button*, int );
extern void GUI_ButtonUpdate( Advanced2D::Button* );

//extern bool GUI_ParseTextbox( Advanced2D::Button* );
extern bool game_CursorCollision( Advanced2D::Sprite*, Advanced2D::Sprite* );
extern void GUI_RenderCursorButtonPress( Advanced2D::Button* );
extern void GUI_RenderCursorOverButton( Advanced2D::Button* );

#pragma once

namespace Advanced2D {

	class GUI
	{
	private:
		Button *selected_button;
		Sprite *selected_sprite;

	public:
		GUI(void);
		virtual ~GUI(void);		

		Sprite *cursor;
		Sprite *console_panel;

		void setSelectedSprite(Sprite *sprite) {this->selected_sprite=sprite;}
		Sprite* getSelectedSprite() {return this->selected_sprite;}
		void setSelectedButton(Button *button) {this->selected_button=button;}
		Button* getSelectedButton() {return this->selected_button;}

		std::list<Button*> p_buttons;
		std::vector <std::string> console_text;

		int collision_order;

		Texture *default_t;

		bool LoadTextures(void);
		void addButton(Button *button);

		void Update();
		void DrawGUI_Buttons();
		void DrawGUI_Cursor();
		void delete_GuiButtons();
		void BuryButtons();

		bool CheckButtonsPress(int mousebutton);
		bool CheckButtonsRelease(int mousebutton);

		//FORM
		Button* form_add(std::string title, int x, int y, int w, int h, char pos,bool hide_background);
		Button* GUI::form_add_yesno(std::string title, std::string message, int w, int h);		
		void form_move(Button *b, int shift_h, int shift_v);
		void form_close(Button *button);
		void form_hidepanel(Button* form);
		void form_unhidepanel(Button* form);
		void form_delete(Button* form);
		void form_fitpanel_w(Button *b);
		void form_fitpanel_h(Button *b);
		Button* form_add_textinput(std::string title, int x, int y, int w, int h, bool hide_background);

		//DROPBOX
		Button* add_dropbox(int type, int x, int y, int w ,int h, std::vector <std::string> text);
		void dropbox_show(Button* dropbox,Button* arrow);
		void dropbox_hide();

		//CHECKBOX
		Button* add_checkbox(int type, int x, int y, int w, int h, std::string text_yes, std::string text_no, Texture *texture, int state);

		//MESSAGE BOX
		void messagebox_add(std::string title,std::string text,int pos);

		//SLIDER
		Button* add_slider_h(int x,int y,int w,int h,int value,int max);
		Button* add_slider_v(int x,int y,int w,int h,int value,int max);
		void update_slider_h(Button *body,int x,int y);
		void update_slider_v(Button *body,int x,int y);
		Button* addSliderPanelH(int x,int y,int w,int h);
		Button* addSliderPanelV(int x,int y,int w,int h);
		void GUI::update_slider_h_value(Button *body,int value);

		//CURSOR COLLISION
		Button* GUI::CursorInsideAButton();
		void CursorCollision();
		void ReverseCursorCollision();
		bool CursorInsideSprite(float dot_x, float dot_y, Sprite* a, float offsetx, float offsety);		

		//INPUT (STRING-KEY CONVERSION)
		int string_to_key(std::string text);
		std::string key_to_string(int key);
		std::string text_input(int key, std::string edit_text);

	};//GUI

}
