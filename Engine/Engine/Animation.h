#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class Animation
	{
	public:
		std::string name;
		std::string path;

		int totalbones;
		int totalframes;
		int timer;	
		std::vector < std::vector <int>> frame_rotation;
		std::vector <string> bone_names;

		bool Load(std::string path,std::string name);

		int getBoneIndex(std::string name);
		void setRotation(Sprite *a);

	};

	class AnimationManager
	{
	public:

		std::vector <Animation*> animations;

		Animation* getAnimation(std::string path);

		bool Load_AnimationList(std::string file_path);

	};
}