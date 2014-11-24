#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define TEXTURES_PLAYER 2

#pragma once

class AR_Weapon
{
public:
	AR_Weapon();

	Texture* texture;
	std::string name;
	float scale_x;
	float scale_y;
	int rot_axis_x;
	int rot_axis_y;
	int handle1_x;
	int handle1_y;
	int handle2_x;
	int handle2_y;
	int barrel_end_x;
	int barrel_end_y;
	int shell_exit_x;
	int shell_exit_y;

	int timer_fire;

};//AR_Weapon

class AR_WeaponManager
{
private:
public:
	std::vector <AR_Weapon> weapon_holder;

	bool Load_WeaponSettings(std::string file_path);
	void Switch_WeaponTexture(Sprite *a, std::string weapon_name);
	bool Check_FireTick(std::string weapon_name, float &fire_tick);

};//AR_WeaponManager