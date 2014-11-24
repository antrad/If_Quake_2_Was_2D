#include "Advanced2D.h"

#pragma once

namespace Advanced2D
{
	class TextureLayer
	{
	public:
		std::vector <Texture*> texture_vector;

		Texture* findTexture(std::string filepath);
		void addTexture(Texture *texture);
	};

	class TextureManager
	{
	public:
		TextureManager(int layers);
		std::vector <TextureLayer*> texture_layers;
		
		void addTextureLayer(int amount);
		void addTexture(Texture *texture,int layer_index);

		bool loadTextures(std::string file_path, int layer_index);
		Texture* findTexture(std::string filepath);
	};

}//namespace
