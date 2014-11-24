#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define ENEMY 300
#define IGRAC_1 20

#define BACKGROUND_0 0
#define BACKGROUND_1 1 
#define WORLD_BACK_0 2
#define WORLD_BACK_1 3
#define ENEMIES 4
#define PLAYER 5
#define PARTICLES_COLL 6
#define PARTICLES_NON 7
#define WORLD_FRONT_COLL 8
#define WORLD_FRONT_NON 9
#define SHADERSPRITES 10

#pragma once

class PlayerCollision
{
public:
	std::vector <bool> done;

	//zbog elastic
	bool left;
	bool right;
	bool up;
	bool down;

	std::vector <bool> s_A;
	std::vector <bool> s_B;
	std::vector <bool> s_C;
	std::vector <bool> s_D;

	std::vector <bool> s_A2;
	std::vector <bool> s_B2;
	std::vector <bool> s_C2;
	std::vector <bool> s_D2;

	double p1_velx;
	double p1_vely; 

	std::vector <Sprite*> my_colliders;
	
	void PlatformPlayerCollision(Sprite *test_me, int entity_list);
	void HitWhere_Count2(Sprite* player,Sprite* b);
	bool TestPlayerCollision_br(Sprite *a, Sprite *b);
	bool DotInsideSpriteBR(float dot_x, float dot_y, Sprite* a);
	bool setVertices_CollBox2(Sprite* a, Sprite* b, bool &A, bool &B, bool &C, bool &D,bool &A2, bool &B2, bool &C2, bool &D2);
	bool setVertices_CollBox3(Sprite* a, Sprite* b, bool &A, bool &B, bool &C, bool &D,bool &A2, bool &B2, bool &C2, bool &D2);

	bool TwoVertexPlayerCollision(Sprite *test_me,Sprite *b,int i);
	
	void setUp(Sprite *test_me);
	void setDown(Sprite *test_me);
	void setLeft(Sprite *test_me);
	void setRight(Sprite *test_me);

	int Bounce(Sprite* player , Sprite* b,int i);
	int Bounce_Dot(Sprite* player , Sprite* b,int i);

	void setLeft_P(Sprite *player, Sprite *b,int i);
	void setRight_P(Sprite *player, Sprite *b,int i);
	void setUp_P(Sprite *player, Sprite *b,int i);
	void setDown_P(Sprite *player, Sprite *b,int i);

	void setLeft_B(Sprite *player, Sprite *b);
	void setRight_B(Sprite *player, Sprite *b);
	void setUp_B(Sprite *player, Sprite *b);
	void setDown_B(Sprite *player, Sprite *b);

	bool AlreadyHit(Sprite *test_me);

	void DynamicCollision(Sprite *test_me, int entity_list);

};