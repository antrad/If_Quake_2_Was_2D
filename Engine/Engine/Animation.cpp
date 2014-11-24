#include "Advanced2D.h"

namespace Advanced2D
{
	int Animation::getBoneIndex(std::string name)
	{
		for(int i=0;i<this->bone_names.size();i++)
		{
			if(bone_names[i]==name) return i;
		}

		return -1;
	}

	void Animation::setRotation(Sprite *a)
	{
		for(unsigned int i=0;i<a->skeleton.size();i++)
		{
			if(a->skeleton[i]!=NULL)
			{
				int index = this->getBoneIndex(a->skeleton[i]->getName());
				if(!a->skeleton[i]->flip_h)
				{
					if(index!=-1)
						a->skeleton[i]->setRotation(g_engine->math->toRadians(this->frame_rotation[index][a->getFrameCurrent()]));
				}
				else
				{
					if(index!=-1)
						a->skeleton[i]->setRotation(g_engine->math->toRadians(180-this->frame_rotation[index][a->getFrameCurrent()]));
				}
			}
		}		
	}

	bool Animation::Load(std::string path,std::string name)
	{
		std::vector <std::string> file_vector;
		std::string command;
		std::string line;
		int value = 0;

		std::ifstream filein(path.c_str());

		//ovo bi trebalo van izbacit
		if(!filein.is_open())
		{
			g_engine->log_text.push_back("ERROR#ANIMATION - Failed reading animation file \"" + path + "\" - file not found." );
			return false;
		}

		while(std::getline(filein, line))
		{
			file_vector.push_back(line);

			std::stringstream stream(line);
			if (stream >> command >> value)
			{
				if (command=="bones")
				{
					this->totalbones=value;
				}
				else if (command=="frames")
				{
					this->totalframes=value;
				}
				else if (command=="timer")
				{
					this->timer=value;
				}
			}
		}
		filein.close();	

		for(int i=0;i<this->totalbones;i++)		
		{
			int k=0;
			while(k<file_vector.size())		
			{
				std::stringstream stream(file_vector[k]);
				std::string command2;
				if (stream >> command2)
				{
					if(command2[0]=='$')//marks a bone
					{
						//new bone found, add new row for rotations
						std::vector <int> new_row;					
						this->bone_names.push_back(command2);

						k++;//read the rotations below below and fill the new rotation row
						std::stringstream stream2(file_vector[k]);
						for(int j=0;j<this->totalframes;j++)
						{
							stream2 >> value;
							new_row.push_back(value);
						}
						this->frame_rotation.push_back(new_row);
					}
				}
				k++;
			}
		}	

		this->name = name;
		this->path = path;

		g_engine->log_text.push_back("Reading animation file \"" + path + "\" ... OK" );

		/*std::ofstream fileout("animation_check.txt");
		for(int i=0;i<this->animation_walk.totalbones;i++)
		{
		fileout << this->animation_walk.bone_names[i] << std::endl;
		for(int j=0;j<this->animation_walk.totalframes;j++)
		{
		fileout << this->animation_walk.frame_rotation[i][j] << " ";
		}
		fileout << std::endl;
		}
		fileout.close();*/

		return true;

	}

	bool AnimationManager::Load_AnimationList(std::string file_path)
	{
		std::ifstream filein(file_path.c_str());
		if(!filein.is_open())
		{
			g_engine->log_text.push_back("ERROR#ANIMATION - Failed to open animation list file \"" + file_path + "\"");
			return false;
		}
		else
			g_engine->log_text.push_back("Opening animation list file -> \"" + file_path + "\" ... OK");

		std::string line;	
		while(std::getline(filein,line))
		{
			Animation *temp = new Animation();
			if (temp->Load(line,"#"))
			{
				this->animations.push_back(temp);
			}
			else
			{
				delete temp;
				return false;	
			}
		}

		filein.close();

		g_engine->log_text.push_back("Loading animation files from list -> \"" + file_path + "\" ... COMPLETED");

		return true;

	}//Load_AnimationList()

	Animation* AnimationManager::getAnimation(std::string path)
	{
		for(unsigned int i=0;i<this->animations.size();i++)
		{
			if(this->animations[i]->path == path) return this->animations[i];
		}

		//g_engine->log_text.push_back("ERROR#ANIMATION - Preloaded animation \"" + path + "\" not found.");
		return NULL;
	}

}