#include "..\..\Engine\Engine\Advanced2D.h"

#define CONTROLS_CLASS -298

//controls
#define O_LEFT -350
#define O_RIGHT -351
#define O_UP -352
#define O_DOWN -353
#define O_JUMP -354
#define O_FIRE1 -355
#define O_FIRE2 -356
#define O_PREVWEAPON -357
#define O_NEXTWEAPON -358
#define O_WEAPON0 -359
#define O_WEAPON1 -360
#define O_WEAPON2 -361
#define O_WEAPON3 -362
#define O_WEAPON4 -363
#define O_WEAPON5 -364
#define O_WEAPON6 -365
#define O_WEAPON7 -366
#define O_WEAPON8 -367
#define O_WEAPON9 -368
#define O_ACTION -367
#define O_PDA -368
#define O_FLY -369
#define O_BULLETTIME -370

#pragma once

namespace Advanced2D
{
	class PlayerControls
	{
	private:
	public:
		int id;
		double s_scalehud;

		//player controls
		Button *left;
		Button *right;
		Button *up;
		Button *down;
		Button *jump;
		Button *fire1;
		Button *fire2;
		Button *action;
		Button *pda;
		Button *b_fly;
		Button *b_bullettime;
		//weapon selection
		Button *prev_weapon;
		Button *next_weapon;
		Button *weapon0;
		Button *weapon1;
		Button *weapon2;
		Button *weapon3;
		Button *weapon4;
		Button *weapon5;
		Button *weapon6;
		Button *weapon7;
		Button *weapon8;
		Button *weapon9;

		std::vector <Button*> buttons;

		int LEFT,RIGHT,UP,DOWN,JUMP,FIRE1,FIRE2,PREV_WEAPON,NEXT_WEAPON,ACTION,PDA,C_FLY,BULLETTIME;
		int WEAPON0,WEAPON1,WEAPON2,WEAPON3,WEAPON4,WEAPON5,WEAPON6,WEAPON7,WEAPON8,WEAPON9;

		PlayerControls();		

		void SetButtons(int x,int y);
		bool Load(std::string filename);
		void UpdateControls();

		void getPlayerControls(
			int &LEFT,int &RIGHT,int &UP,int &DOWN,int &JUMP,
			int &FIRE1,int &FIRE2,
			int &ACTION,int &PDA,
			int &C_FLY,int &BULLETTIME,
			int &NEXT_WEAPON,int &PREV_WEAPON,
			int &WEAPON0,int &WEAPON1,int &WEAPON2,int &WEAPON3,
			int &WEAPON4,int &WEAPON5,int &WEAPON6,int &WEAPON7,int &WEAPON8,int &WEAPON9);

	};	

}//Advanced2D