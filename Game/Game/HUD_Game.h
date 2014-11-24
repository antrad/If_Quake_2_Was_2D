#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define BLASTER 0
#define SHOTGUN 1
#define S_SHOTGUN 2
#define MACHINEGUN 3
#define CHAINGUN 4
#define G_LAUNCHER 5
#define R_LAUNCHER 6
#define HYPERBLASTER 7
#define RAILGUN 8
#define BFG 9

#define TEXTURES_PLAYER 2

#pragma once

class HUD_Game
{
private:
public:
	float f1icon_tick;
	float f1icon_display;
	
	std::vector<Sprite*> hud_weapons;
	Sprite *selected_weapon;
	Sprite *weapon_tab;
	Sprite *energy;	
	Sprite *ammo_s;
	Sprite *help_s;
	Sprite *armor_s;

	Texture *hud_weapons_t;
	Texture *ammo_t;
	Texture *energy_t;	
	Texture *weapon_tab_t;
	Texture *armor_t;
	Texture *help_t;
	
	std::vector<Sprite*> ammo_sprites;
	bool weapon_unlocked[15];
	double ammo_amount[10];
	int ammo_amount_max[10];
	int ammo_current;	
	int weapon_group;
	float gui_scale;
	bool new_f1;
	
	int player_weapon;	
	double hudweapons_tick;

	HUD_Game(void);
	~HUD_Game(void);	

	bool load_textures();
	void HUD_weapons();
	void setHUDpositions(int x, int y, int w, int h);
	void draw(int energy, int armor, int ammo_current);	

	int x,y,w,h;

};//Weapons
