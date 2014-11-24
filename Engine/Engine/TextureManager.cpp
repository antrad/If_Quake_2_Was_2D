#include "Advanced2D.h"

namespace Advanced2D
{
	/////////////////////
	//  TEXTURE LAYER  //
	/////////////////////

	Texture* TextureLayer::findTexture(std::string filepath)
	{
		std::vector<Texture*>::iterator iter = this->texture_vector.begin();		

		while (iter != this->texture_vector.end())
		{
			if((*iter)->getFilePath()==filepath)
				return *iter;
			else
				iter++;				
		}

		return NULL;
	}

	void TextureLayer::addTexture(Texture *texture)
	{
		this->texture_vector.push_back(texture);
	}

	///////////////////////
	//  TEXTURE MANAGER  //
	///////////////////////

	TextureManager::TextureManager(int layers)
	{
		this->addTextureLayer(layers);
	}

	void TextureManager::addTextureLayer(int amount)
	{
		for(int i=0;i<amount;i++)
		{
			TextureLayer *layer = new TextureLayer();
			this->texture_layers.push_back(layer);
		}
	}

	Texture* TextureManager::findTexture(std::string filepath)
	{
		Texture *a = NULL;
		for(unsigned int i=0;i<this->texture_layers.size();i++)
		{
			a = this->texture_layers[i]->findTexture(filepath);
			if(a) return a;
		}

		g_engine->log_text.push_back("ERROR#TEXTURE - Texture not preloaded \"" + filepath + "\".");
		return NULL;
	}	

	void TextureManager::addTexture(Texture *texture,int layer_index)
	{
		this->texture_layers[layer_index]->addTexture(texture);
	}	

	bool TextureManager::loadTextures(std::string file_path,int layer_index)
	{
		std::ifstream filein(file_path.c_str());
		if(!filein.is_open())
		{
			g_engine->log_text.push_back("ERROR#TEXTURE - Failed to open texture list file \"" + file_path + "\"");
			return false;
		}
		else
			g_engine->log_text.push_back("Loading texture list file -> \"" + file_path + "\" ... OK");

		std::string line;	
		while(std::getline(filein,line))
		{
			Texture *new_texture = new Texture();			

			//texture info
			std::string path;
			int w = 1;
			int h = 1;
			int columns = 1;
			int offset_x = 0;
			int offset_y = 0;

			std::stringstream stream(line);

			if (stream >> path >> w >> h >> columns >> offset_x >> offset_y)//load tilemap
			{
				if (new_texture->Load(path))
				{
					new_texture->setTilemap(true);				
					new_texture->tile_width = w;
					new_texture->tile_height = h;
					new_texture->tile_columns = columns;
					new_texture->tile_offset_x = offset_x;
					new_texture->tile_offset_y = offset_y;				
					this->addTexture(new_texture,layer_index);
				}
				else
				{
					new_texture = NULL;
					g_engine->gui->messagebox_add("ERROR",path + " not found !",10);
					return false;
				}
			}
			else//load normal texture
			{
				if (new_texture->Load(path))
				{
					this->addTexture(new_texture,layer_index);
				}
				else
				{
					new_texture = NULL;
					g_engine->gui->messagebox_add("ERROR",path + " not found !",10);
					return false;
				}
			}

		}//while()

		filein.close();

		g_engine->log_text.push_back("Loading textures from list -> \"" + file_path + "\" ... COMPLETED");

		return true;

	}

}//Advanced2D
