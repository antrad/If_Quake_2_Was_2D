#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#include "Particles.h"

Particles::Particles(void)
{
	smoke_t = NULL;
	rocket_flame = NULL;	
	bloodmist = NULL;	
	spark_t = NULL;	
	light0 = NULL;
}

bool Particles::LoadTextures()
{
	spark_t = g_engine->texture_manager->findTexture("Data/Textures/Particles/spark.png");
	if (spark_t==NULL){g_engine->message("\"Data/Textures/Particles/spark.png\" not preloaded.");return false;}

	rocket_flame = g_engine->texture_manager->findTexture("Data/Textures/Particles/rocket_flame.tga");
	if (rocket_flame==NULL){g_engine->message("\"Data/Textures/Particles/rocket_flame.tga\" not preloaded.");return false;}

	smoke_t = g_engine->texture_manager->findTexture("Data/Textures/Particles/dim.tga");
	if (smoke_t==NULL){g_engine->message("\"Data/Textures/Particles/dim.tga\" not preloaded.");return false;}

	bloodmist = g_engine->texture_manager->findTexture("Data/Textures/Particles/bloodmist.tga");
	if (bloodmist==NULL){g_engine->message("\"Data/Textures/Particles/bloodmist.tga\" not preloaded.");return false;}	

	light0 = g_engine->texture_manager->findTexture("Data/Textures/Weapons/flamethrower_flame_light.tga");
	if (light0==NULL){g_engine->message("\"Data/Textures/Weapons/flamethrower_flame_light.tga\" not preloaded.");return false;}

	return true;

}//LoadTextures

void Particles::addSmoke(float x, float y, int amount)
{
	//return;

	for (unsigned int i=0;i<amount;i++)
	{
		Sprite *smoke = new Sprite();
		smoke->setType(DIM);
		smoke->setImage(this->smoke_t);
		
		double angle = rand()%360;
		double scale = 3 + rand()%10;
		
		smoke->setVelocity(g_engine->math->LinearVelocityX(angle)*5,g_engine->math->LinearVelocityY(angle)*5);
		smoke->setScale(0.8,0.8);
		smoke->setRotation(g_engine->math->toRadians(angle));		
		smoke->setPosition(x-smoke->hw(),y-smoke->hw());

		if (rand()%2) smoke->setColor_Texture(0xFF555555);
		else smoke->setColor_Texture(0xFF66666);

		smoke->setCollidable(false);
		g_engine->addSprite(smoke,PARTICLES_NON);
	}

}//addSmoke()

void Particles::addFire(float x, float y, int amount)
{
	for (unsigned int i=0;i<2*amount;i++)
	{
		Sprite *smoke = new Sprite();
		smoke->setType(FIRE);
		smoke->setImage(this->smoke_t);		
		
		double angle = rand()%360;
		double scale = 3 + rand()%10;
		double vel = 5 + (rand()%10)*0.1;
		smoke->setVelocity(g_engine->math->LinearVelocityX(angle)*vel,g_engine->math->LinearVelocityY(angle)*vel);
		
		smoke->setScale(0.8,0.8);
		smoke->setRotation(g_engine->math->toRadians(angle));		
		smoke->setPosition(x-smoke->hw(),y-smoke->hh());
		smoke->setColor_Texture(0xFFFFAA11);		
		smoke->setCollidable(false);

		g_engine->addSprite(smoke,PARTICLES_NON);
	}

}//addSmoke()

void Particles::addFireLinear(float x, float y, int angle)
{
	Sprite *smoke = new Sprite();
	smoke->setType(FIRE);
	smoke->setImage(this->smoke_t);	

	smoke->setVelocity(g_engine->math->LinearVelocityX(angle)*5,g_engine->math->LinearVelocityY(angle)*5);
	smoke->setScale(1.5,1.5);
	smoke->setRotation(g_engine->math->toRadians(rand()%360));		
	smoke->setPosition(x-smoke->hw(),y-smoke->hh());

	smoke->setColor_Texture(0xFFFFAA11);		

	smoke->setCollidable(false);
	g_engine->addSprite(smoke,WORLD_BACK_1);

}//addSmoke()

void Particles::addBloodBig(float x, float y, int amount)
{
	for (unsigned int i=0;i<amount;i++)
	{
		Sprite *spark = new Sprite();
		spark->setType(SPARK);
		spark->setMovementState(FALL);
		spark->setImage(g_engine->gui->default_t);
		spark->setSize(2,2);
		spark->setPosition(x,y);
		spark->setColor_Texture(D3DCOLOR_XRGB(64+rand()%128,0,0));
		
		double angle = rand()%360;
		double vel = (double)(rand()%10)*0.1;
		spark->setVelocity(g_engine->math->LinearVelocityX(angle)*(1+vel),g_engine->math->LinearVelocityY(angle)*(1+vel));		

		spark->setCollidable(false);
		g_engine->addSprite(spark,PARTICLES_NON);
	}
}

void Particles::addRocketFlame(Sprite *rocket)
{
	Sprite *smoke = new Sprite();
	smoke->setType(DIM);
	smoke->setImage(this->smoke_t);

	double angle = rand()%360;	

	smoke->setScale(0.25,0.25);
	smoke->setRotation(g_engine->math->toRadians(angle));		
	smoke->setPosition(
		rocket->coll_box[rocket->coll_box.size()-1].getX()-smoke->hw(),
		rocket->coll_box[rocket->coll_box.size()-1].getY()-smoke->hh());

	if (rand()%2) smoke->setColor_Texture(0xFF555555);
	else smoke->setColor_Texture(0xFF66666);

	smoke->setCollidable(false);
	g_engine->addSprite(smoke,PARTICLES_NON);	

}//addRocketFlame()

void Particles::addSpark(float x , float y, int amount)
{
	for (unsigned int i=0;i<amount;i++)
	{
		Sprite *spark = new Sprite();
		spark->setType(SPARK);
		spark->setImage(g_engine->gui->default_t);
		spark->setSize(2,2);
		
		double angle = rand()%360;
		double vel = 6 + (rand()%50)*0.1;

		spark->setVelocity(g_engine->math->LinearVelocityX(angle)*vel,g_engine->math->LinearVelocityY(angle)*vel);
		spark->setPosition(x,y);

		spark->setColor_Texture(D3DCOLOR_ARGB(255,255,128+rand()%128,0));
		spark->setCollidable(false);
		g_engine->addSprite(spark,PARTICLES_NON);
	}

	this->addLight(x,y,3);
}

void Particles::addBulletSpark(float x, float y, int amount)
{
	for (unsigned int i=0;i<amount;i++)
	{
		Sprite *spark = new Sprite();
		spark->setType(SPARK);
		spark->setMovementState(FALL);
		spark->setImage(g_engine->gui->default_t);
		spark->setSize(2,2);
		spark->setPosition(x,y);
		spark->setColor_Texture(D3DCOLOR_XRGB(255,128+rand()%128,0));
		
		double angle = rand()%360;
		double vel = (double)(rand()%10)*0.1;
		spark->setVelocity(g_engine->math->LinearVelocityX(angle)*(1+vel),g_engine->math->LinearVelocityY(angle)*(1+vel));		

		spark->setText("bullet");//fade away
		spark->setCollidable(false);
		g_engine->addSprite(spark,PARTICLES_NON);
	}
}

void Particles::addBulletSpark_Grey(float x, float y, int amount)
{
	for (unsigned int i=0;i<amount;i++)
	{
		Sprite *spark = new Sprite();
		spark->setType(SPARK);
		spark->setMovementState(FALL);
		spark->setImage(g_engine->gui->default_t);
		spark->setSize(2,2);
		spark->setPosition(x,y);

		int temp = 50 + rand()%50;
		spark->setColor_Texture(D3DCOLOR_XRGB(temp,temp,temp));
		
		double angle = rand()%360;
		double vel = (double)(rand()%10)*0.1;
		spark->setVelocity(g_engine->math->LinearVelocityX(angle)*(1+vel),g_engine->math->LinearVelocityY(angle)*(1+vel));		

		spark->setText("bullet");//fade away
		spark->setCollidable(false);
		g_engine->addSprite(spark,PARTICLES_NON);
	}

	this->addLight(x,y,0.5);

}

void Particles::addExplosionCollision(float x, float y, int radius)
{
	Sprite *exp = new Sprite();
	exp->setType(EXP);
	exp->setImage(g_engine->gui->default_t);
	exp->setSize(2*radius,2*radius);
	exp->setPosition(x-radius,y-radius);
	g_engine->coll_manager->setColl_box(exp);
	g_engine->addSprite(exp,PARTICLES_NON);	
}

void Particles::addBulletTrail(float x, float y, float angle, int amount)
{
	for (unsigned int i=0;i<amount;i++)
	{
		Sprite *spark = new Sprite();
		spark->setType(SPARK);
		//spark->setMovementState(FALL);
		spark->setImage(g_engine->gui->default_t);
		spark->setSize(2,2);
		spark->setPosition(x,y);
		spark->setColor_Texture(D3DCOLOR_ARGB(255,255,128,0));
		
		//double angle = rand()%360;
		//double vel = (double)(rand()%10)*0.1;
		//spark->setVelocity(g_engine->math->LinearVelocityX(angle)*(1+vel),g_engine->math->LinearVelocityY(angle)*(1+vel));
		
		spark->setCollidable(false);
		g_engine->addSprite(spark,PARTICLES_NON);
	}
}

////////////////////////
////////////////////////

bool line_linelinexy(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double &xxx,double &yyy)
{

	g_engine->lineline_counter++;

	double epsilon = 0.01;//0.001 doesn't work properly

	double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return false;

	// Get the x and y
	double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	double x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	double y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

	// Check if the x and y coordinates are within both lines
	if ( x < min(x1, x2)-epsilon ||
		x > max(x1, x2)+epsilon ||
		x < min(x3, x4)-epsilon ||
		x > max(x3, x4)+epsilon ) return false;

	if ( y < min(y1, y2)-epsilon ||
		y > max(y1, y2)+epsilon ||
		y < min(y3, y4)-epsilon ||
		y > max(y3, y4)+epsilon ) return false;

	xxx=x;
	yyy=y;
	return true;

}//line_lineline

void line_linerectxy( double x1, double y1, double xx1, double yy1, double x2, double y2, double w2, double h2,
				bool &AB, bool &BC, bool &CD, bool &DA,
				double &ab_x, double &ab_y, double &bc_x, double &bc_y, double &cd_x, double &cd_y, double &da_x, double &da_y)
{
	// A - B
	// |   |
	// D - C
	AB = false;
	BC = false;
	CD = false;
	DA = false;

	// AB
	if (line_linelinexy(x1,y1,xx1,yy1,x2,y2,x2+w2,y2,ab_x,ab_y)) AB = true;
	// BC
	if (line_linelinexy(x1,y1,xx1,yy1,x2+w2,y2,x2+w2,y2+h2,bc_x,bc_y)) BC = true;
	// CD
	if (line_linelinexy(x1,y1,xx1,yy1,x2+w2,y2+h2,x2,y2+h2,cd_x,cd_y)) CD = true;
	// DA
	if (line_linelinexy(x1,y1,xx1,yy1,x2,y2+h2,x2,y2,da_x,da_y)) DA = true;

	

}//line_linerectABCD

void blablabla(Sprite *a,Sprite *wall,double &pos_x,double &pos_y)
{
	bool AB = false; bool BC = false; bool CD = false; bool DA = false;

	double ab_x; double ab_y; double bc_x; double bc_y; double cd_x; double cd_y; double da_x; double da_y;

	double future_x = a->coll_box[0].getX()+5*a->getVelocityX();//new
	double future_y = a->coll_box[0].getY()+5*a->getVelocityY();//new

	double past_x = a->coll_box[2].getX()-10*a->getVelocityX();
	double past_y = a->coll_box[2].getY()-10*a->getVelocityY();

	//+- 1 newest, jednom se pojavilo nakon newest, new newest +2,opetisto , onda +2 + 4
	line_linerectxy(future_x,future_y,past_x,past_y,wall->getX()-2,wall->getY()-2,wall->w()+4,wall->h()+4,
		AB,BC,CD,DA,
		ab_x,ab_y,bc_x,bc_y,cd_x,cd_y,da_x,da_y);

	//a ako je AB && CD

	//check corners
	if (AB && BC)
	{
		if (g_engine->math->Distance(past_x,past_y,ab_x,ab_y)<g_engine->math->Distance(past_x,past_y,bc_x,bc_y)) BC=false;
		else AB=false;
	}
	else if (BC && CD)
	{
		if (g_engine->math->Distance(past_x,past_y,bc_x,bc_y)<g_engine->math->Distance(past_x,past_y,cd_x,cd_y)) CD=false;
		else BC=false;
	}
	else if (CD && DA)
	{
		if (g_engine->math->Distance(past_x,past_y,cd_x,cd_y)<g_engine->math->Distance(past_x,past_y,da_x,da_y)) DA=false;
		else CD=false;
	}
	else if (DA && AB)
	{
		if (g_engine->math->Distance(past_x,past_y,da_x,da_y)<g_engine->math->Distance(past_x,past_y,ab_x,ab_y)) AB=false;
		else DA=false;
	}
	
	//check || and =
	if (AB && CD)
	{
		if (g_engine->math->Distance(past_x,past_y,ab_x,ab_y)<g_engine->math->Distance(past_x,past_y,cd_x,cd_y)) CD=false;
		else AB=false;
	}
	else if (BC && DA)
	{
		if (g_engine->math->Distance(past_x,past_y,bc_x,bc_y)<g_engine->math->Distance(past_x,past_y,da_x,da_y)) DA=false;
		else BC=false;
	}


	//if (!AB && !BC && !CD && !DA) g_engine->gui->messagebox_add(" You got a message!"," linerectxy",100);

	if (AB)
	{
		pos_x=ab_x;
		pos_y=ab_y;
		return;
	}

	if (BC)
	{
		pos_x=bc_x;
		pos_y=bc_y;
		return;
	}

	if (CD)
	{
		pos_x=cd_x;
		pos_y=cd_y;
		return;
	}

	if (DA)
	{
		pos_x=da_x;
		pos_y=da_y;
		return;
	}
}

void Particles::addBulletSpark(Sprite *bullet, Sprite *wall, int amount)
{
	//a sta ako nema kolizije
	double pos_x = 0;
	double pos_y = 0;

	blablabla(bullet,wall,pos_x,pos_y);

	for (unsigned int i=0;i<amount;i++) addBulletSpark(pos_x,pos_y,1);
}

void Particles::addBulletSpark_Grey(Sprite *bullet, Sprite *wall, int amount)
{
	//a sta ako nema kolizije
	double pos_x = 0;
	double pos_y = 0;

	blablabla(bullet,wall,pos_x,pos_y);

	for (unsigned int i=0;i<amount;i++) addBulletSpark_Grey(pos_x,pos_y,1);
}

void Particles::addFlies_full(float x,float y)
{
	Sprite *range = new Sprite();
	range->setImage(g_engine->gui->default_t);
	range->setSize(64,64);
	range->setPosition(x-32,y-32);
	if(g_engine->getEngineState()==GAME) range->setVisible(false);
	range->setCollidable(false);
	g_engine->addSprite(range,PARTICLES_NON);

	Sprite *sound = new Sprite();
	sound->setClass(-501);
	sound->setType(501);
	sound->setImage(g_engine->gui->default_t);
	sound->setSize(64,64);
	sound->setRenderType(TILE);
	sound->setPosition(x-32,y-32);
	sound->setText("Inflies1 500");
	g_engine->addSprite(sound,11);

	for(int i=0;i<15;i++)
	{
		int angle = rand()%360;
		double vel = 0.5 + (rand()%6)*0.1;

		Sprite *fly = new Sprite();
		fly->setImage(g_engine->gui->default_t);
		fly->setPosition(range->getX() + rand()%(int)range->w(),range->getY() + rand()%(int)range->h());
		fly->setVelocity(
			g_engine->math->LinearVelocityX(angle)*vel,
			g_engine->math->LinearVelocityY(angle)*vel);
		fly->setSize(1,1);
		fly->setColor_Texture(0xFF000000);
		fly->setCollidable(false);
		g_engine->addSprite(fly,PARTICLES_NON);

		range->children.push_back(fly);
	}
}

void Particles::addFlies(Sprite *range)
{
	if(g_engine->getEngineState()==GAME) range->setVisible(false);
	
	for(int i=0;i<15;i++)
	{
		int angle = rand()%360;
		double vel = 0.5 + (rand()%6)*0.1;

		Sprite *fly = new Sprite();
		fly->setImage(g_engine->gui->default_t);
		fly->setPosition(range->getX() + rand()%(int)range->w(),range->getY() + rand()%(int)range->h());
		fly->setVelocity(
			g_engine->math->LinearVelocityX(angle)*vel,
			g_engine->math->LinearVelocityY(angle)*vel);
		fly->setSize(1,1);
		fly->setColor_Texture(0xFF000000);
		g_engine->addSprite(fly,PARTICLES_NON);

		range->children.push_back(fly);
	}

}

void Particles::addLight(float x, float y, float scale)
{
	Sprite *light = new Sprite();
	light->setImage(this->light0);
	light->setScale(scale,scale);
	light->setPosition(x-light->hw(),y-light->hh());
	light->oneframelife = true;	
	light->setColor_Texture(0xFF00FF00);
	light->setCollidable(false);
	g_engine->addSprite(light,SHADERSPRITES);
}
