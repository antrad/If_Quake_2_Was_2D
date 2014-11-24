// Advanced2D Engine
// Main header file

#pragma once

#define WIN32_EXTRA_LEAN
#include <windows.h>

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <dxerr.h>
#include <dinput.h>
#include <dinputd.h>//za joystick

#include "Timer.h"

#define DIRECTINPUT_VERSION 0x0800
#include "AR_Direct3D.h"
#include "fmod.hpp"
#include "AR_Audio.h"

#include "Texture.h"
#include "Vector3.h"
#include "AR_Rectangle.h"
#include "Entity.h"
#include "AR_Button.h"
#include "Sprite.h"
#include "AR_DirectInput.h"

#include "Math.h"

//Antonio
#include "AR_Collision.h"
#include "AR_Graphics.h"
#include "AR_GUI.h"
#include "AR_DXFont.h"
#include "Font.h"
#include "EntityLayer.h"
#include "AR_Map.h"
#include "TextureManager.h"
#include "Animation.h"
//

//EDIT STUFF
#include "AR_Skeleton.h"
#include "EditLayer.h"
#include "EditSprite.h"

#define AUTO 0
#define MANUAL 1

//external variables and functions
extern bool gameover;

extern bool game_preload();
extern bool game_init(HWND);
extern void game_update();
extern void game_end();

extern void game_render();

extern void game_spriteUpdate(Advanced2D::Sprite*);
extern bool game_spriteCollision(Advanced2D::Sprite*,Advanced2D::Sprite*);
extern void game_spriteErase(Advanced2D::Sprite*);
extern void game_spritePostRender(Advanced2D::Sprite*);

extern void UpdateSpritesManual();
extern void editor_update();

extern bool IHitAPlatform(Advanced2D::Sprite*,Advanced2D::Sprite*,int side);

namespace Advanced2D 
{
	enum State{GAME,EDITOR,PRELOAD};
	enum GameState{CREDITS,MENU,MENU_D,PLAY,GAMEOVER,EXIT,OPTIONS,OPTIONS_C,INTRO1,INTRO2,INTRO3};
	enum Render_Type{TEXTURE,VECTOR,ALL};

	class Engine
	{
	private:
		HWND p_windowHandle;

		std::string p_apptitle;
		bool p_fullscreen;
		int p_screenwidth;
		int p_screenheight;
		int p_colordepth;
		Timer p_coreTimer;
		long p_frameCount_core;
		long p_frameRate_core;
		Timer p_realTimer;
		long p_frameCount_real;
		long p_frameRate_real;
		Timer timedUpdate;

		long time_past;
		long time_present;
		long time_delta;//ms

		int render_type;
		int engine_state;

	public:
		std::vector <Sector> sectors;

		LPDIRECT3D9 p_d3d;
		LPD3DXSPRITE p_sprite_handler;

		void BurySprites();		
		Input *p_input;		
		
		LPDIRECT3DSURFACE9 pBackBuffer;
			
		Engine();
		virtual ~Engine();

		bool locked;

		bool bullet_time;
		bool v_sync;
		bool noclip;
		int difficulty;

		void save_log();
		std::vector <std::string> log_text;

		int Init(int width, int height, int colordepth, bool fullscreen);
		void Close();
		void Update();
		void UpdateFin();
		void Shutdown();

		void message(std::string message, std::string title = "MUG engine");
		void fatalerror(std::string message, std::string title = "FATAL ERROR");

		int Release();

		float game_speed;

		AR_Direct3D *ar_direct3d;

		std::list <Line*> line_list;		

		//AR
		eng_Collision *coll_manager;
		int game_level;
		int game_state;
		GUI *gui;
		AR_Graphics *ar_graphics;
		LPDIRECT3DDEVICE9 p_device;
		LPD3DXEFFECT p_effect;		

		//display stuff
		bool display_sectors;
		bool display_lineofsight;
		bool display_rotationaxis;
		bool display_collisiondots;
		bool display_fps;

		std::vector <AR_Map*> game_maps;
		int getMap(std::string name)
		{
			for(unsigned int i=0;i<this->game_maps.size();i++)
			{
				if(game_maps[i]->getName()==name) return i;
			}

			//this->audio->Play("jump1");

			return 0;

		};

		int map_current;
		bool save_to_map;
		int lock_framerate;

		//SPRITE AND TEXTURE MANAGEMENT
		std::vector <SpriteLayer*> game_sprites;
		//std::vector <SpriteLayer*> game_sprites2;
		TextureManager *texture_manager;
		SkeletonManager skeleton_manager;
		AnimationManager animation_manager;

		AR_DXFontManager font_manager_dx;
		AR_FontManager font_manager;
		AR_DXFont *e_font;

		float offset_Y;
		float offset_X;

		float collisioncheck_counter;
		float collisioncheck_counter_other;
		float count_check_onscreen;
		float count_check_insector;
		float collision_counter;
		float collidable_counter;
		float lineline_counter;		

		bool p_pauseMode;
		int p_UpdateTime;

		int getGameState() { return this->game_state; }
		void setGameState(int value) { this->game_state = value; }

		int getGameLevel() { return this->game_level; }
		void setGameLevel(int value) { this->game_level = value; }

		int getRenderType() { return this->render_type; }
		void setRenderType(int value) { this->render_type = value; }

		int getEngineState() { return this->engine_state; }
		void setEngineState(int value) { this->engine_state = value; }

		float getCollidableCounter() { return this->collidable_counter; }

		double Engine::stringtoint(std::string text);
		std::string Engine::inttostring(double i);
		
		//public Audio object
		AR_Audio *audio;
		//public Math object
		Math *math;		

		//accessor/mutator functions expose the private variables
		bool getPaused() { return this->p_pauseMode; }
		void setPaused(bool value) { this->p_pauseMode = value; }

		LPDIRECT3DDEVICE9 getDevice() { return this->p_device; }
		LPD3DXSPRITE getSpriteHandler() { return this->p_sprite_handler; }

		void setWindowHandle(HWND hwnd) { this->p_windowHandle = hwnd; }
		HWND getWindowHandle() { return this->p_windowHandle; }

		std::string getAppTitle() { return this->p_apptitle; }
		void setAppTitle(std::string value) { this->p_apptitle = value; }

		long getFrameRate_core(){return this->p_frameRate_core;};
		long getFrameRate_FPS(){return this->p_frameRate_real;};
		long getTime_Delta(){return this->time_delta;};

		float GetDeltaFix()//difference from 60FPS
		{
			if(this->time_delta>32) return 2.0f;//if framerate is under 30FPS don't speed up more
			else return (float)this->time_delta/16;}

		int getScreenWidth() { return this->p_screenwidth; }
		void setScreenWidth(int value) { this->p_screenwidth = value; }
		int getScreenHeight() { return this->p_screenheight; }
		void setScreenHeight(int value) { this->p_screenheight = value; }

		int getColorDepth() { return this->p_colordepth; }
		void setColorDepth(int value) { this->p_colordepth = value; }

		bool getFullscreen() { return this->p_fullscreen; }
		void setFullscreen(bool value) { this->p_fullscreen = value; }		

		void addSprite(Sprite *sprite, int sprite_layer);
		void addSpriteLayer(int amount);

		void addSprite(Sprite *sprite, int map, int sprite_layer);

		void delete_GameSprites();

		int getSpriteCount();

		float getLayerOffsetX(int layer)
		{
			if ( layer > this->game_sprites.size()-1 ) return 0;
			return this->offset_X*this->game_sprites[layer]->getOffsetMultiplierX();
		};

		float getLayerOffsetY(int layer)
		{
			if ( layer > this->game_sprites.size()-1 ) return 0;
			return this->offset_Y*this->game_sprites[layer]->getOffsetMultiplierY();
		};		
		
	}; //class

}; //namespace

//define the global engine object (visible everywhere!)
extern Advanced2D::Engine *g_engine;
