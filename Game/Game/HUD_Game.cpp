#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "HUD_Game.h"

HUD_Game::HUD_Game(void)
{
	//f1
	this->new_f1 = true;
	this->f1icon_tick = 0;
	this->f1icon_display = true;

	//textures
	this->weapon_tab_t = NULL;	
	this->ammo_t = NULL;
	this->energy_t = NULL;
	this->hud_weapons_t = NULL;
	this->armor_t = NULL;
	this->help_t = NULL;
	
	//sprites
	this->selected_weapon = NULL;
	this->weapon_tab = NULL;
	this->energy = NULL;
	this->ammo_s = NULL;
	this->armor_s = NULL;
	this->help_s = NULL;

	//misc
	this->ammo_current=-1;
	this->player_weapon = 0;
	this->weapon_group = 0;
	this->hudweapons_tick = 255;	
	this->gui_scale = 1;	

}

HUD_Game::~HUD_Game(void){}

bool HUD_Game::load_textures(void)
{
	ammo_amount_max[0] = 999;//blaster
	ammo_amount_max[1] = 100;//shotgun
	ammo_amount_max[2] = 200;//machinegun
	ammo_amount_max[3] = 50;//gl
	ammo_amount_max[4] = 50;//rl
	ammo_amount_max[5] = 200;//hyperblaster
	ammo_amount_max[6] = 50;//railgun
	ammo_amount_max[7] = 200;//bfg	
	
	for(int i=0;i<10;i++) ammo_amount[i] = 0;
	for(int i=0;i<10;i++) weapon_unlocked[i] = false;	
	weapon_unlocked[0] = true;	

	//SELECTION BOX
	this->hud_weapons_t = g_engine->texture_manager->findTexture("Data/Textures/Weapons/weapons.png");
	if (this->hud_weapons_t==NULL){g_engine->message("\"Data/Textures/Weapons/weapons.png\" not preloaded.");return false;}

	//AMMUNITION STATUS
	this->ammo_t = g_engine->texture_manager->findTexture("Data/Textures/Weapons/ammo_icons.png");
	if (this->ammo_t==NULL){g_engine->message("\"Data/Textures/Weapons/ammo_icons.png\" not preloaded.");return false;}

	this->armor_t = g_engine->texture_manager->findTexture("Data/Textures/Weapons/i_armor.png");
	if (this->armor_t==NULL){g_engine->message("\"Data/Textures/Weapons/i_armor.png\" not preloaded.");return false;}

	this->help_t = g_engine->texture_manager->findTexture("Data/Textures/Weapons/i_help.png");
	if (this->help_t==NULL){g_engine->message("\"Data/Textures/Weapons/i_help.png\" not preloaded.");return false;}

	this->energy_t = g_engine->texture_manager->findTexture("Data/Textures/Weapons/i_health.png");
	if (this->energy_t==NULL){g_engine->message("\"Data/Textures/Weapons/i_health.png\" not preloaded.");return false;}

	this->weapon_tab_t = g_engine->texture_manager->findTexture("Data/Textures/Weapons/weapon_tab.png");
	if (this->weapon_tab_t==NULL){g_engine->message("\"Data/Textures/Weapons/weapon_tab.png\" not preloaded.");return false;}

	return true;
}

void HUD_Game::draw(int energy, int armor, int ammo_current)
{
	//SELECTION BOX
	/*
	if (this->hudweapons_tick>50)
	{
		this->hudweapons_tick -= 2*g_engine->game_speed*g_engine->GetDeltaFix();

		for(int i=0;i<this->hud_weapons.size();i++)
		{
			//nevalja
			if (i==player_weapon)
				this->hud_weapons[i]->setColor_Texture(0xFFFFDE1B);
			else
				this->hud_weapons[i]->setColor_Texture(0xFF2D5B95);
			
			this->hud_weapons[i]->setColor_Texture(D3DCOLOR_ARGB(
				(int)this->hudweapons_tick,
				(int)g_engine->ar_graphics->getRed(this->hud_weapons[i]->getColor_Texture()),
				(int)g_engine->ar_graphics->getGreen(this->hud_weapons[i]->getColor_Texture()),
				(int)g_engine->ar_graphics->getBlue(this->hud_weapons[i]->getColor_Texture()))					
				);

			if (this->hud_weapons[i]->getClass()==this->weapon_group && this->weapon_unlocked[i])
				g_engine->ar_graphics->draw_sprite(this->hud_weapons[i],0,0);
		}
	}
	else
		this->hudweapons_tick = 0;

	this->weapon_tab->setColor_Texture(D3DCOLOR_ARGB(
				(int)this->hudweapons_tick,
				(int)g_engine->ar_graphics->getRed(this->weapon_tab->getColor_Texture()),
				(int)g_engine->ar_graphics->getGreen(this->weapon_tab->getColor_Texture()),
				(int)g_engine->ar_graphics->getBlue(this->weapon_tab->getColor_Texture()))
				);

	g_engine->ar_graphics->draw_sprite(this->weapon_tab,0,0);//brojevi*/	
		
	//AMMUNITION STATUS (down-right)
	/*for(int i=0;i<this->ammo_sprites.size();i++)
	{
		//TEXTURE
		if (this->ammo_sprites[i]->getVisible())
			g_engine->ar_graphics->draw_sprite(this->ammo_sprites[i],0,0);

		//STATUS
		double height = ((double)ammo_amount[i]/ammo_amount_max[i])*this->ammo_sprites[i]->getHeight();
		if(height>0 && height<1) height = 1;
		else if (height<0) height = 0;			

		g_engine->ar_graphics->draw_texture(
			g_engine->gui->default_t,
			this->ammo_sprites[i]->getX()+this->ammo_sprites[i]->getWidth()-6,
			this->ammo_sprites[i]->getY()+this->ammo_sprites[i]->getHeight()-(int)height,
			0,0,
			0,0,
			6,(int)height,
			0xFFFFDE1B);	
	}*/	

	//ENERGY
	std::ostringstream text;

	if (energy<0) energy = 0;
	text << (int)energy;

	if(g_engine->font_manager.fonts.size()!=0)
		g_engine->font_manager.fonts[1]->PrintText(
		text.str(),
		this->energy->getX()-g_engine->font_manager.fonts[1]->getLength(text.str(),g_engine->font_manager.fonts[1]->char_dist,this->gui_scale)-2,
		this->energy->getY()+this->energy->hh()-g_engine->font_manager.fonts[1]->getHeight()/2*this->gui_scale,
		this->gui_scale,2,0xFFFFFFFF);

	g_engine->ar_graphics->draw_sprite(this->energy,0,0);

	//ARMOR
	text.str("");

	if(armor<0) armor = 0;
	text << (int)armor;

	if(armor>0)
	{
		if(g_engine->font_manager.fonts.size()!=0)
			g_engine->font_manager.fonts[1]->PrintText(
			text.str(),
			this->armor_s->getX()-g_engine->font_manager.fonts[1]->getLength(text.str(),g_engine->font_manager.fonts[1]->char_dist,this->gui_scale)-2,
			this->armor_s->getY()+this->armor_s->hh()-g_engine->font_manager.fonts[1]->getHeight()/2*this->gui_scale,
			this->gui_scale,2,0xFFFFFFFF);

		g_engine->ar_graphics->draw_sprite(this->armor_s,0,0);
	}

	//AMMO	
	text.str("");

	if(this->ammo_current==0)
		text << " ";
	else
		text << (int)ammo_amount[this->ammo_current];

	if(g_engine->font_manager.fonts.size()!=0)
		g_engine->font_manager.fonts[1]->PrintText(
		text.str(),
		this->ammo_s->getX()-g_engine->font_manager.fonts[1]->getLength(text.str(),g_engine->font_manager.fonts[1]->char_dist,this->gui_scale)-2,
		this->ammo_s->getY()+this->ammo_s->hh()-g_engine->font_manager.fonts[1]->getHeight()/2*this->gui_scale,
		this->gui_scale,2,0xFFFFFFFF);

	g_engine->ar_graphics->draw_sprite(this->ammo_s,0,0);

	//WEAPON
	g_engine->ar_graphics->draw_sprite(this->selected_weapon,0,0);
	
	//F1
	if(this->new_f1)
	{
		if(this->f1icon_tick<750/14) this->f1icon_tick += g_engine->game_speed*g_engine->GetDeltaFix();
		if(this->f1icon_tick>=750/14)
		{
			this->f1icon_display = !this->f1icon_display;
			this->f1icon_tick = 0;
		}
		
		if(this->f1icon_display) g_engine->ar_graphics->draw_sprite(this->help_s,0,0);
	}

}//draw()

void HUD_Game::HUD_weapons()
{
	weapon_tab = new Sprite();
	weapon_tab->setImage(this->weapon_tab_t);
	weapon_tab->setPosition(x+5,y+5);
	weapon_tab->setScale(this->gui_scale,this->gui_scale);

	for (int i=0;i<=10;i++)
	{
		Sprite *tile0 = new Sprite();
		tile0->setImage(this->hud_weapons_t);
		tile0->setSize(this->hud_weapons_t->getHeight(),this->hud_weapons_t->getHeight());
		tile0->setScale(this->gui_scale,this->gui_scale);
		this->hud_weapons.push_back(tile0);
	}	

	this->hud_weapons[BLASTER]->setFrameXY(0,0);
	this->hud_weapons[BLASTER]->setClass(0);
	this->hud_weapons[BLASTER]->setScale(this->gui_scale,this->gui_scale);

	this->hud_weapons[SHOTGUN]->setFrameXY(24,0);
	this->hud_weapons[SHOTGUN]->setClass(1);
	this->hud_weapons[SHOTGUN]->setScale(this->gui_scale,this->gui_scale);

	this->hud_weapons[S_SHOTGUN]->setFrameXY(48,0);
	this->hud_weapons[S_SHOTGUN]->setClass(2);
	this->hud_weapons[S_SHOTGUN]->setScale(this->gui_scale,this->gui_scale);

	this->hud_weapons[MACHINEGUN]->setFrameXY(72,0);
	this->hud_weapons[MACHINEGUN]->setClass(3);
	this->hud_weapons[MACHINEGUN]->setScale(this->gui_scale,this->gui_scale);

	this->hud_weapons[CHAINGUN]->setFrameXY(96,0);
	this->hud_weapons[CHAINGUN]->setClass(4);
	this->hud_weapons[CHAINGUN]->setScale(this->gui_scale,this->gui_scale);
	
	this->hud_weapons[G_LAUNCHER]->setFrameXY(120,0);
	this->hud_weapons[G_LAUNCHER]->setClass(5);
	this->hud_weapons[G_LAUNCHER]->setScale(this->gui_scale,this->gui_scale);

	this->hud_weapons[R_LAUNCHER]->setFrameXY(144,0);
	this->hud_weapons[R_LAUNCHER]->setClass(6);
	this->hud_weapons[R_LAUNCHER]->setScale(this->gui_scale,this->gui_scale);

	this->hud_weapons[HYPERBLASTER]->setFrameXY(168,0);
	this->hud_weapons[HYPERBLASTER]->setClass(7);
	this->hud_weapons[HYPERBLASTER]->setScale(this->gui_scale,this->gui_scale);

	this->hud_weapons[RAILGUN]->setFrameXY(192,0);
	this->hud_weapons[RAILGUN]->setClass(7);
	this->hud_weapons[RAILGUN]->setScale(this->gui_scale,this->gui_scale);

	this->hud_weapons[BFG]->setFrameXY(216,0);
	this->hud_weapons[BFG]->setClass(8);
	this->hud_weapons[BFG]->setScale(this->gui_scale,this->gui_scale);
	
	this->energy = new Sprite();
	this->energy->setImage(this->energy_t);
	this->energy->setScale(this->gui_scale,this->gui_scale);

	this->armor_s = new Sprite();
	this->armor_s->setImage(this->armor_t);
	this->armor_s->setScale(this->gui_scale,this->gui_scale);

	this->help_s = new Sprite();
	this->help_s->setImage(this->help_t);
	this->help_s->setScale(this->gui_scale,this->gui_scale);
	
	this->selected_weapon = new Sprite();
	this->selected_weapon->setImage(this->hud_weapons_t);
	this->selected_weapon->setSize(this->hud_weapons_t->getHeight(),this->hud_weapons_t->getHeight());
	this->selected_weapon->setScale(this->gui_scale,this->gui_scale);

	this->ammo_s = new Sprite();
	this->ammo_s->setImage(this->ammo_t);
	this->ammo_s->setSize(this->ammo_t->getHeight(),this->ammo_t->getHeight());
	this->ammo_s->setScale(this->gui_scale,this->gui_scale);
	
	//ammo status
	for (int i=0;i<9;i++)
	{
		Sprite *tile0 = new Sprite();
		tile0->setImage(this->ammo_t);
		tile0->setSize(32,32);
		tile0->setColor_Texture(0xFF2D5B95);
		tile0->setFrameXY(i*32,0);
		tile0->setScale(this->gui_scale,this->gui_scale);
		this->ammo_sprites.push_back(tile0);
	}
	
}//HUD_weapons()

void HUD_Game::setHUDpositions(int x, int y, int w, int h)
{
	//refresh f1
	this->new_f1 = true;
	this->f1icon_tick = 0;
	this->f1icon_display = true;

	//ovo su nepotrebni ulazni podaci
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;	

	//SELECTION BOX
	this->weapon_tab->setPosition(x+5,y+5);	

	int posx = x + 5;
	int posy = y + 5 + this->weapon_tab->getHeight() + 2;

	int d = 32;
	int r = this->hud_weapons[BLASTER]->getHeight() + 2;

	this->hud_weapons[BLASTER]->setPosition(posx,posy);
	posx += d;

	this->hud_weapons[SHOTGUN]->setPosition(posx,posy);
	posx += d;	

	this->hud_weapons[S_SHOTGUN]->setPosition(posx,posy);
	posx += d;	

	this->hud_weapons[MACHINEGUN]->setPosition(posx,posy);
	posx += d;	

	this->hud_weapons[CHAINGUN]->setPosition(posx,posy);
	posx += d;	

	this->hud_weapons[G_LAUNCHER]->setPosition(posx,posy);	
	posx += d;

	this->hud_weapons[R_LAUNCHER]->setPosition(posx,posy);
	posx += d;

	this->hud_weapons[HYPERBLASTER]->setPosition(posx,posy);
	posx += d;	

	this->hud_weapons[RAILGUN]->setPosition(posx,posy);	
	posx += d;
	
	this->hud_weapons[BFG]->setPosition(posx,posy);	

	this->energy->setScale(this->gui_scale,this->gui_scale);
	this->armor_s->setScale(this->gui_scale,this->gui_scale);
	this->selected_weapon->setScale(this->gui_scale,this->gui_scale);
	this->ammo_s->setScale(this->gui_scale,this->gui_scale);
	this->help_s->setScale(this->gui_scale,this->gui_scale);

	//ENERGY AND AMMO
	this->energy->setPosition(x + w/2 - 200*this->gui_scale, y + h - this->energy->h() - 2);
	this->armor_s->setPosition(x + w/2 - 100*this->gui_scale, y + h - this->armor_s->h() - 2);

	this->selected_weapon->setPosition(x + w/2 + 150*this->gui_scale, y + h - this->selected_weapon->h()-2);
	this->ammo_s->setPosition(x + w/2 + 100*this->gui_scale, y +h - this->ammo_s->h()-2);	
	this->help_s->setPosition(x + w/2 - this->help_s->hw(), this->ammo_s->getY() - this->help_s->h() - 2);	

	//AMMUNITION STATUS
	for (int i=0;i<9;i++)
	{
		ammo_sprites[i]->setPosition(x + w - 308 + i*34, y + h - 32 -2);
		ammo_sprites[i]->setScale(this->gui_scale,this->gui_scale);
	}

}//setHUDpositions()