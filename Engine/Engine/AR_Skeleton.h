#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class Bone2
	{
	public:
		std::string name,texture_path;
		int index;
		int index_skeleton;
		Texture *texture;

		double rot_axis_x,rot_axis_y,scale_x,scale_y;
		bool parent;
		std::string parent_name;
		int parent_att_dot;

		std::vector <double> att_x;
		std::vector <double> att_y;

		Bone2();	

	};

	class Skeleton
	{
	public:
		std::string file_path;
		std::vector <std::string> file_text;

		std::vector <Bone2> bone_data;
		std::vector <Texture*> bone_textures;
		std::vector <Sprite*> bone_sprites;		

		bool Load_BoneData(std::string filename);	

		void BuildSkeletonTemplate();
		void setSkeleton(Sprite *a);
		Sprite* getParent(std::string parent_name, Sprite *a);		

	};

	class SkeletonManager
	{
	public:
		std::vector <Skeleton*> skeletons;

		void ConnectBones(Sprite *a);
		void Walk_Bipedal(Sprite *a);
		void Scale(std::vector <Sprite*> bones, float scalex, float scaley);		

		Skeleton* GetSkeleton(std::string file_path);
		bool Load_SkeletonList(std::string file_path);

	};

}//Advanced2D