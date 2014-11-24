#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "AR_WeaponManager.h"

AR_Weapon::AR_Weapon()
{
	this->texture = NULL;
	this->name = "#";
	this->scale_x = 1;
	this->scale_y = 1;
	this->rot_axis_x = 0;
	this->rot_axis_y = 0;
	this->handle1_x = 0;
	this->handle1_y = 0;
	this->handle2_x = 0;
	this->handle2_y = 0;
	this->barrel_end_x = 0;
	this->barrel_end_y = 0;
	this->shell_exit_x = 0;
	this->shell_exit_y = 0;

	this->timer_fire = 0;
}

bool AR_WeaponManager::Load_WeaponSettings(std::string file_path)
{
	std::ifstream filein(file_path.c_str());		
	if(!filein.is_open())
	{
		g_engine->log_text.push_back("ERROR#SETTINGS_WEAPONS - Failed to open file \"" + file_path + "\"");
		return false;
	}

	std::string line;
	while(std::getline(filein,line))
	{
		std::stringstream stream(line);
		std::string error(line);
		AR_Weapon w_h;
		std::string texture_path;
		if(stream
			>> w_h.name
			>> w_h.scale_x >> w_h.scale_y
			>> w_h.rot_axis_x >> w_h.rot_axis_y
			>> w_h.barrel_end_x >> w_h.barrel_end_y
			>> w_h.handle1_x >> w_h.handle1_y >> w_h.handle2_x >> w_h.handle2_y
			>> w_h.shell_exit_x >> w_h.shell_exit_y
			>> texture_path >> w_h.timer_fire)
		{
			w_h.texture = g_engine->texture_manager->findTexture(texture_path);
			if(w_h.texture==NULL)
			{
				g_engine->log_text.push_back("ERROR#SETTINGS_WEAPONS - Failed to find texture \"" + texture_path + "\"");
				return false;
			}
			this->weapon_holder.push_back(w_h);
		}
		else
		{
			g_engine->log_text.push_back("ERROR#SETTINGS_WEAPONS - Failed to read line in \"" + file_path + "\"");
			g_engine->log_text.push_back("ERROR#SETTINGS_WEAPONS - line \"" + error + "\"");
			return false;
		}
	}

	return true;

}//Load_WeaponSettings()

void AR_WeaponManager::Switch_WeaponTexture(Sprite *a, std::string weapon_name)
{
	for(unsigned int i=0;i<this->weapon_holder.size();i++)
	{
		if(weapon_holder[i].name == weapon_name)
		{
			a->setText(weapon_name);
			a->setImage(this->weapon_holder[i].texture);
			a->setScale(this->weapon_holder[i].scale_x,this->weapon_holder[i].scale_y);			
			
			a->setRotationAxis(
				this->weapon_holder[i].rot_axis_x,
				this->weapon_holder[i].rot_axis_y);			
			
			a->setAtt_pos(
				0,
				this->weapon_holder[i].barrel_end_x,
				this->weapon_holder[i].barrel_end_y);
			
			a->setAtt_pos(
				1,
				this->weapon_holder[i].shell_exit_x,
				this->weapon_holder[i].shell_exit_y);
			
			//druga ruka se spaja na 1,a ne na dva
			a->setAtt_pos(
				2,
				this->weapon_holder[i].handle2_x,
				this->weapon_holder[i].handle2_y);
			
			a->setAtt_pos(
				3,
				this->weapon_holder[i].handle1_x,
				this->weapon_holder[i].handle1_y);

			return;
		}
	}
}

bool AR_WeaponManager::Check_FireTick(std::string weapon_name, float &fire_tick)
{
	for(unsigned int i=0;i<this->weapon_holder.size();i++)
	{
		if(weapon_holder[i].name==weapon_name)
		{
			if(fire_tick >= weapon_holder[i].timer_fire)
			{
				fire_tick = 0;
				return true;
			}
			else
				return false;
		}
	}

	return false;

}//CheckFireTick()