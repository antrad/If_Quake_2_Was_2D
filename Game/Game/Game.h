#include "..\..\Engine\Engine\Advanced2D.h"
#include <iostream>
#include <string.h>

using namespace Advanced2D;
using namespace std;

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
#define LAYER_SOUND 11

#define DYNAMIC -575
#define LIGHT 1331

//sprite state
#define INAIR 1
#define ROLLING 2
#define STILL 3

//player1
#define IGRAC_1 20
#define P1_WEAPON 21

//misc
#define FEATHER 700
#define OBJECT_EXP 502
#define KRV 506
#define MEX 508
#define SHELL 509
#define PLAMEN 510
#define GIBS 511
#define DIM 513
#define DEADHARPY 520
#define DROP 1122

//header files
#include "misc.h"
//#include "AR_Physics.h"
#include "Editor.h"
#include "Animator.h"
#include "PlayerCollision.h"

//default screen resolution
int SCREENW = 800;
int SCREENH = 600;

double tick, lasttick;
float TimerA, TimerAOld;

//menu triggers
bool console_state = false;
bool cmd = false;
double player_posx=0;

//misc
int PixelShader_ID=-1;
int sudar=99;
int p1sudar=999;

Editor *editor=NULL;
Animator *animator=NULL;