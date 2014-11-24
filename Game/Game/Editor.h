#include "..\..\Engine\Engine\Advanced2D.h"

//next-gen
#include "Beheaded.h"
#include "Biomech.h"
#include "Gnaar.h"
#include "Beshtiya.h"

#include "Clipboard.h"
#include "PlayerControls.h"
#include "Options.h"

#include "Player.h"
#include "World.h"
#include "Particles.h"

using namespace Advanced2D;

#define PLAYERMODEL 5031
#define SELECT_TILEMAP 5030

#define PLAYER_START 32000
#define LEVEL_BORDER 32001

//BUTTONS
#define BUTTON_TOMAINMENU 98
#define BUTTON_GAME 99
#define BUTTON_GAMECUSTOM 97
#define BUTTON_1PLAYER 100
#define BUTTON_2PLAYER 101
#define BUTTON_4PLAYER 102
#define BUTTON_QUIT 110
#define EXIT_PROGRAM 111
#define BUTTON_EDITOR 130
#define BUTTON_ANIMEDITOR 131

#define BUTTON_LOAD 140
#define BUTTON_GRID 141
#define BUTTON_TILED 142
#define BUTTON_CURSORCOLLISION 143
#define BUTTON_WORKINGLAYER 144
#define BUTTON_GRID_SIZE 145
#define BUTTON_OPTIONS 146
#define BUTTON_DIFFICULTY 147

#define BUTTON_SAVE 150
#define BUTTON_EXIT_EDITOR 160
#define BUTTON_CLEAR_ALL 190
#define BUTTON_ELOAD 200

#define LOAD_LEVEL 666
#define MAP_SAVE 667
#define MAP_SAVEAS 669
#define MAP_EDIT 668

#define BUTTON_TEXTURE 9876
#define ANIMATION -5
#define PLATFORMA 501
#define SOUND -501
#define SOUND_POINT -502
#define SOUND_ONCE -503

#define LAYER_SOUND 11

#pragma once

namespace Advanced2D
{
	class Editor
	{
	public:
		int sector_size;

		std::vector <Sprite*> pl_models;

		Clipboard *clipboard;	

		//textures
		Texture *t_action;
		Texture *t_cursor_menu;
		Texture *t_options_banner;
		Texture *t_video_banner;
		Texture *t_menu_paused;
		Texture *t_banner_game;
		Texture *t_menu_game0;
		Texture *t_menu_game1;
		Texture *t_menu_options0;
		Texture *t_menu_options1;
		Texture *t_menu_quit0;
		Texture *t_menu_quit1;
		Texture *t_menu_loading;
		Texture *t_menu_editor0;
		Texture *t_menu_editor1;
		Texture *t_menu_animator0;
		Texture *t_menu_animator1;
		Texture *t_ingame_helpconsole;
		Texture *t_consoleback;
		Texture *t_modelbox;
		Texture *t_inventory;
		Texture *t_save;
		Texture *t_back;

		Texture *t_1player;

		Texture *t_areyousure;
		Texture *t_yes;
		Texture *t_no;

		Texture *Cursor_Menu,*Cursor_ingame,*Cursor_sniper;
		Texture *player_start;		
		Texture *default_t;

		Texture *grid_t;		
		Texture *tiled_t;

		Texture *t_sound;
		Texture *t_spam;

		bool tiled_sprite;	
		bool display_f1;
		float f1_tick;		
		float gui_scale;
		bool options_visible;

		int player_number;

		int map_red;
		int map_green;
		int map_blue;
		std::string loaded_level;

		//LEVEL BORDERS
		Texture *map_border_t;
		float border_left,border_right,border_up,border_down;

		//EDIT STUFF
		EditSprite *edit_sprite;
		EditLayer *edit_layer;
		Options *options;

		//MANAGERS
		std::vector <Player*> Player_managers;

		//next-gen		
		Beheaded *Beheaded_manager;		
		Biomech *Biomech_manager;
		Gnaar *GnaarM_manager;
		Beshtiya *Beshtiya_manager;

		World *World_manager;
		Particles *Particle_manager;		

		//PRIMARY STUFF
		Editor();
		bool init();
		void setEditor(bool real_editor);
		bool LoadTextures();
		void Reset();		
		void Draw();		

		//CONTROLS
		void keyPress(int key);
		void keyRelease(int key);
		void mousePress(int button);
		void mouseRelease(int button);
		void mouseWheel(int wheel);
		void buttonPress(Button *button, int mousebutton);
		void buttonRelease(Button *button, int mousebutton);

		//BUTTONS
		void addMenuButtons();
		void addEditorButtons();		

		//SPRITES
		void addPlayerStart(double x, double y);
		void addLevelBorder(double x, double y, std::string text);

		//TEXTURES
		void addTextureButtons_tilemap(int x,int y);	
		void addTextureButtons_normal(int x,int y);	

		//LOAD AND SAVE MAP
		std::string map_current;
		std::vector <std::string> map_list;

		//MAP
		bool Map_ListLoad();
		bool Map_ListSave();
		bool Map_ListOn(std::string filename);
		void Map_LoadButtons(Button *button,int button_type);

		bool Map_LoadFromFile(std::string filename,int player_num);
		bool Map_LoadEntitiesFromFile(std::vector <std::string> file_vec,int player_num);

		bool Map_LoadSubMaps(std::string filename,int player_num);		
		bool Map_SaveToFile(std::string filename);

		//ADD SPRITE
		void addSprite_texture(Button *button);
		bool LoadGameAssets();
		void SortBorders();
		void DrawF1();

		void addMenu2Buttons();
		void addQuitButtons();

		void addSound(double x, double y);
		void addSpammer(double x, double y);

		void DrawInventory(int id);

	};//Editor

}//namespace
