#include "..\..\Engine\Engine\Advanced2D.h"

#include "PlayerControls.h"

#define OPTIONS_CLASS -299

//misc
#define O_SCR_SCALE -304
#define O_FULLSCREEN -305
#define O_VSYNC -306
#define O_RESOLUTION -308
#define O_VOLUMEMUSIC -310
#define O_VOLUMESOUND -311
#define O_SPLITSCREEN -312
#define O_SCALEHUD -313
#define O_CONTROLS -314
#define O_EXITCONTROLS -315
#define O_LOCKFRAMERATE -316

//exit
#define OPTIONS_SAVE -320
#define OPTIONS_EXIT -321
#define OPTIONS_EXITCONTROLS -322

#pragma once

namespace Advanced2D
{
	class Options
	{
	private:
	public:
		//game settings
		bool s_fullscreen;
		bool s_vsync;		
		int s_screenw;
		int s_screenh;
		int s_volumemusic;
		int s_volumesound;
		bool s_splitscreen;
		double s_scalehud;
		int s_lockframerate;
		bool s_gamelocked;
		
		//screen
		Button *fullscreen;
		Button *v_sync;
		Button *scr_resolution;
		Button *splitscreen;
		Button *b_scalehud;
		Button *b_lockframerate;

		//misc
		Button *volume_music;
		Button *volume_sound;
		Button *b_controls;

		Texture *t_save;
		Texture *t_back;

		//player controls
		std::vector <PlayerControls*> player_controls;

		std::vector <Button*> buttons;
		std::vector <Button*> buttons_controls;

		bool options_visible;

		Options();

		bool LoadTextures();

		void setNULL();
		bool loadSettings();
		void setOptions();

		void ButtonRelease(Button *button,int mousebutton);

		void options_savetofile();
		void options_createdefaultfile();	

		void setOptions_Controls();

	};

}//Advanced2D