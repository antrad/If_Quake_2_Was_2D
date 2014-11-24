#include "..\..\Engine\Engine\Advanced2D.h"
using namespace Advanced2D;

#define BUTTON_BONE 4321
#define BONE_ADDFRAME 4322
#define BONE_FRAME 4323
#define ANIMATOR_TIMER 4324
#define ANIMATOR_MIDFRAME 4325

#define BUTTON_ANIMATOR_PLAY 4400
#define BUTTON_ANIMATOR_DOUBLE 4401
#define BUTTON_ANIMATOR_RESET 4402
#define BUTTON_ANIMATOR_NEW 4403
#define BUTTON_ANIMATOR_SAVE 4404
#define BUTTON_ANIMATOR_SAVEAS 4405
#define BUTTON_ANIMATOR_SAVETOFILE 4406
#define BUTTON_ANIMATOR_LOAD 4407
#define BUTTON_ANIMATOR_EXIT 4408
#define BUTTON_ANIMATOR_SKELETON 4409

#pragma once

namespace Advanced2D
{
	class Animator_Bone
	{
	public:
		Animator_Bone();
		
		Button *button_bone;
		Sprite *sprite_bone;
		std::vector <Button*> frame_rotation;		
	};

	class Animator
	{
	public:
		Animator();		

		void Reset();

		float scale_bone;
		double frame_tick;
		bool play_animation;
		int totalframes;
		int currentframe;
		std::string current_animation;
		
		Sprite *sprite;
		std::vector <Animator_Bone*> bone_vector;
		std::vector <Button*> frame_button;

		std::string current_skeleton;
		
		Button *animator_form;
		Button *animator_timer;
		Button *frame_add;

		//ANIMATION LIST
		std::vector <std::string> animation_list;
		bool Animation_ListLoad();
		bool Animation_ListSave();
		bool Animation_ListOn(std::string filename);

		void addLoadButtons(int button_type);
		
		bool LoadTextures();
		void setAnimator();

		void Update();		
		void Draw();
		void Update_sprite(int currentframe);

		void Animation_new();
		bool Animation_save(std::string filename);
		void Animation_load(std::string filename);	

		void addSkeletonButtons();

		void setAnimationForm();
		Button* addBone(Sprite *a, int x, int y);
		void addFrameColumn();		
		void double_frames();

		void SaveSkeleton();


		//controls
		void keyPress(int key);
		void keyRelease(int key);
		void mousePress(int button);
		void mouseRelease(int button);
		void mouseWheel(int wheel);
		void buttonPress(Button *button, int mousebutton);
		void buttonRelease(Button *button, int mousebutton);		

	};//Animator

}//namespace

