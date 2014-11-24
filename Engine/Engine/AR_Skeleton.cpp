#include "Advanced2D.h"

namespace Advanced2D
{

	Bone2::Bone2()
	{
		this->texture = NULL;
		this->texture_path = "";
		this->name = "";
		this->index = 0;
		this->index_skeleton = 0;
		this->rot_axis_x = 0;
		this->rot_axis_y = 0;		
		this->scale_x = 1;
		this->scale_y = 1;

		this->parent = false;
		this->parent_name = "";
		this->parent_att_dot = 0;
	}

	bool Skeleton::Load_BoneData(std::string filename)
	{
		//open file
		std::ifstream file(filename.c_str());
		if(!file.is_open())
		{
			g_engine->log_text.push_back("Error reading \"" + filename + "\" - file not found." );
			return false;
		}

		g_engine->log_text.push_back("Reading 2D model data - \"" + filename + "\" ... " );

		std::vector <std::string> temp_filetext;

		//read file
		std::string line;
		while(std::getline(file,line))
		{
			temp_filetext.push_back(line);//

			if(line[0]=='$')//marks a bone
			{
				//get first row (number of attachments,rotation axis,scale,texture)
				std::string line_info;
				std::getline(file,line_info);
				temp_filetext.push_back(line_info);//

				Bone2 bone_temp;
				double att_num = 0;

				std::stringstream stream_line_info(line_info);
				if(stream_line_info >> att_num >> bone_temp.rot_axis_x >> bone_temp.rot_axis_y >> bone_temp.scale_x >> bone_temp.scale_y >> bone_temp.texture_path >> bone_temp.index >> bone_temp.index_skeleton)
				{
					//find bone texture and save it
					Texture *temp_texture = NULL;
					temp_texture = g_engine->texture_manager->findTexture(bone_temp.texture_path);
					if(!temp_texture)
					{
						g_engine->log_text.push_back("ERROR#2D_MODEL - Failed reading \"" + filename + "\" :" );
						g_engine->log_text.push_back("ERROR#2D_MODEL - Failed to find texture \"" + bone_temp.texture_path + "\" .");
						return false;
					}
					else
					{
						temp_texture->setName(line);
						bone_temp.texture = temp_texture;
						this->bone_textures.push_back(temp_texture);
					}

					//get parent info
					std::string line_parent;
					std::getline(file,line_parent);
					temp_filetext.push_back(line_parent);//

					std::stringstream stream_line_parent(line_parent);
					if(stream_line_parent >> bone_temp.parent >> bone_temp.parent_name >> bone_temp.parent_att_dot)
					{
						//OK
					}
					else
					{
						g_engine->log_text.push_back("Error reading \"" + filename + "\" - wrong parent info." );
						g_engine->log_text.push_back("[2] -> \"" + line_parent + "\"");
						return false;//error
					}				

					bone_temp.name = line;

					//get attachment positions
					for(unsigned int i=0;i<att_num;i++)
					{
						double att_x = 0;
						double att_y = 0;
						std::string line_att;
						std::getline(file,line_att);
						temp_filetext.push_back(line_att);//

						std::stringstream stream_line_att(line_att);
						if(stream_line_att >> att_x >> att_y)
						{
							bone_temp.att_x.push_back(att_x);
							bone_temp.att_y.push_back(att_y);
						}
						else
						{
							g_engine->log_text.push_back("Error reading \"" + filename + "\" - wrong attachment position info." );
							g_engine->log_text.push_back("[3] -> \"" + line_att + "\"");
							return false;//error
						}
					}
				}
				else
				{
					g_engine->log_text.push_back("Error reading \"" + filename + "\" - wrong bone info." );
					g_engine->log_text.push_back("[1] -> \"" + line_info + "\"");
					return false;//error
				}

				//save bone information
				this->bone_data.push_back(bone_temp);

			}//if bone

		}//while read file

		file.close();

		this->file_text = temp_filetext;//save file
		this->file_path = filename;

		g_engine->log_text[g_engine->log_text.size()-1] = g_engine->log_text[g_engine->log_text.size()-1] + "OK";

		this->BuildSkeletonTemplate();	

		return true;

	}//LoadInfo()	

	void SkeletonManager::ConnectBones(Sprite *a)
	{
		for(unsigned int i=0;i<a->skeleton.size();i++)
		{
			if(!a->skeleton[i]->parent)
			{
				a->skeleton[i]->setPosition(a->cx(),a->getY()+a->skeleton[i]->w());	
			}
			else
			{
				a->skeleton[i]->parent->attach(a->skeleton[i]->parent_att_dot,a->skeleton[i]);
			}
		}

	}//ConnectBones()

	void Skeleton::BuildSkeletonTemplate()
	{
		//create sprites
		for(unsigned int i=0;i<this->bone_data.size();i++)
		{
			Sprite *new_sprite = new Sprite();
			new_sprite->setCollidable(false);
			new_sprite->setColor_Box(0xFFFF8800);
			this->bone_sprites.push_back(new_sprite);
		}

		for(unsigned int i=0;i<this->bone_data.size();i++)
		{
			int index = this->bone_data[i].index;

			//da sacuva ona zadnja dva broja iz skeleton datoteke, privremeno rjesenje
			this->bone_sprites[index]->setText(g_engine->inttostring(this->bone_data[i].index)+" "+g_engine->inttostring(this->bone_data[i].index_skeleton));

			this->bone_sprites[index]->setImage(this->bone_data[i].texture);			
			this->bone_sprites[index]->setName(this->bone_data[i].name);

			//scale before attachment positions
			this->bone_sprites[index]->setScaleX(this->bone_data[i].scale_x);
			this->bone_sprites[index]->setScaleY(this->bone_data[i].scale_y);

			/*this->bone_sprites[index]->setRotationAxis(
				this->bone_data[i].rot_axis_x*this->bone_sprites[index]->getScaleX(),
				this->bone_data[i].rot_axis_y*this->bone_sprites[index]->getScaleY());*/

			this->bone_sprites[index]->setRotationAxis(
				this->bone_data[i].rot_axis_x,
				this->bone_data[i].rot_axis_y);

			//set all attachment points				
			this->bone_sprites[index]->setAtt_num(this->bone_data[i].att_x.size());
			for (unsigned int k=0;k<this->bone_data[i].att_x.size();k++)
			{
				/*this->bone_sprites[index]->setAtt_pos(
					k,
					this->bone_data[i].att_x[k]*this->bone_sprites[index]->getScaleX(),
					this->bone_data[i].att_y[k]*this->bone_sprites[index]->getScaleY());*/

				this->bone_sprites[index]->setAtt_pos(
					k,
					this->bone_data[i].att_x[k],
					this->bone_data[i].att_y[k]);
			}

			//where to connect to
			this->bone_sprites[index]->parent_att_dot = this->bone_data[i].parent_att_dot;
		}	

	}//BuildSkeletonTemplate()

	Sprite* Skeleton::getParent(std::string parent_name, Sprite *a)
	{
		for(unsigned int i=0;i<a->skeleton.size();i++)
		{
			if(parent_name == a->skeleton[i]->getName()) return a->skeleton[i];
		}

		return NULL;
	}

	void Skeleton::setSkeleton(Sprite *a)
	{
		std::vector <Sprite*> temp;

		for(unsigned int i=0;i<this->bone_sprites.size();i++)
		{
			Sprite *new_sprite = this->bone_sprites[i]->Copy();
			new_sprite->setLayer(a->getLayer());		
			g_engine->addSprite(new_sprite,new_sprite->getLayer());
			temp.push_back(new_sprite);
		}

		//set skeleton order
		for (unsigned int i=0;i<this->bone_data.size();i++)
		{
			for (unsigned int j=0;j<this->bone_data.size();j++)
			{
				if(bone_data[j].index_skeleton == i) a->skeleton.push_back(temp[bone_data[j].index]);
			}
		}		

		//set parents
		for (unsigned int i=0;i<this->bone_data.size();i++)
		{
			temp[bone_data[i].index]->parent = this->getParent(bone_data[i].parent_name,a);
		}

		a->skeleton_name = this->file_path;

	}//getSkeleton()

	//SKELETON MANAGER
	
	Skeleton* SkeletonManager::GetSkeleton(std::string file_path)
	{
		std::vector <Skeleton*>::iterator iter = this->skeletons.begin();
		while(iter != this->skeletons.end())
		{
			Skeleton *a = *iter;
			if (a->file_path == file_path) return a;
			iter++;
		}

		g_engine->log_text.push_back("Failed to find skeleton \"" + file_path + "\"");

		return NULL ;
	}

	bool SkeletonManager::Load_SkeletonList(std::string file_path)
	{
		std::ifstream filein(file_path.c_str());
		if(!filein.is_open())
		{
			g_engine->log_text.push_back("ERROR#SKELETON - Failed to open skeleton list file \"" + file_path + "\"");
			return false;
		}
		else
			g_engine->log_text.push_back("Opening skeleton list file -> \"" + file_path + "\" ... OK");

		std::string line;	
		while(std::getline(filein,line))
		{
			Skeleton *temp = new Skeleton();
			if (temp->Load_BoneData(line))
			{
				this->skeletons.push_back(temp);
			}
			else
			{
				delete temp;
				return false;	
			}
		}

		filein.close();

		g_engine->log_text.push_back("Loading skeleton files from list -> \"" + file_path + "\" ... COMPLETED");

		return true;

	}//Load_SkeletonList()	

	void SkeletonManager::Scale(std::vector <Sprite*> bones, float scalex, float scaley)
	{
		for (unsigned int i=0;i<bones.size();i++)
		{
			bones[i]->setScale(
				bones[i]->getScaleX()*scalex,
				bones[i]->getScaleY()*scaley);

			if(bones[i]->rotate)
				bones[i]->vector_rect.Set(bones[i]->w(),bones[i]->h(),bones[i]->getRotateX_real(),bones[i]->getRotateY_real());
			else
				bones[i]->vector_rect.Set(bones[i]->w(),bones[i]->h(),bones[i]->hw(),bones[i]->hh());

			//osvjezava att_distance i angle, da s ne racuna svaki frame
			for(unsigned int j=0;j<bones[i]->att_angle.size();j++)
			{
				bones[i]->setAtt_pos(j, bones[i]->att_posx[j], bones[i]->att_posy[j]);
			}			
		}

	}//Scale()

	void SkeletonManager::Walk_Bipedal(Sprite *a)
	{
		Sprite *torso = NULL;
		Sprite *foot1 = NULL;
		Sprite *foot2 = NULL;

		for(unsigned int i=0;i<a->skeleton.size();i++)
		{
			if(!a->skeleton[i]->parent)
			{
				a->skeleton[i]->setPosition(a->cx(),a->getY()+a->skeleton[i]->w());
				torso = a->skeleton[i];
			}
			else
			{
				a->skeleton[i]->parent->attach(a->skeleton[i]->parent_att_dot,a->skeleton[i]);
			}
		}

		foot1 = a->getBone("$FOOT1");
		foot2 = a->getBone("$FOOT2");

		if(foot1==NULL) foot1 = a->getBone("$LEG1_DOWN");
		if(foot2==NULL) foot2 = a->getBone("$LEG2_DOWN");

		if(foot1!=NULL && foot2!=NULL && torso!=NULL)
		{
			double bottom = 0;

			if(foot1->getAtt_posY(0)>foot2->getAtt_posY(0))
				bottom = foot1->getAtt_posY(0);
			else
				bottom = foot2->getAtt_posY(0);

			double delta_height = fabs(torso->getAtt_posY(3)-bottom);//zasto 3?

			for(unsigned int i=0;i<a->skeleton.size();i++)
			{
				if(!a->skeleton[i]->parent)
				{
					a->skeleton[i]->setPosition(a->cx(),a->getY()+a->h()-delta_height);	
				}
				else
				{
					a->skeleton[i]->parent->attach(a->skeleton[i]->parent_att_dot,a->skeleton[i]);
				}
			}
		}

	}//Walk_Bipedal()

}//Advanced2D