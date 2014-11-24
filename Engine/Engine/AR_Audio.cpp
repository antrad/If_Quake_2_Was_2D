#include "Advanced2D.h"

namespace Advanced2D
{
	//////////////
	//  SAMPLE  //
	//////////////

	AR_Sound::AR_Sound()
	{
		this->frequency = 1;
		this->name = "";
		this->filepath = "";
		this->type = AR_SOUND_SOUND;
		this->channel = NULL;
		this->sound = NULL;

	}

	AR_Sound::~AR_Sound()
	{
		if (this->sound != NULL)
		{
			this->sound->release();
			this->sound = NULL;
		}
	}

	/////////////
	//  AUDIO  //
	/////////////

	AR_Audio::AR_Audio()
	{
		this->system = NULL;

		this->frequency = 1;
		this->volume_sound = 1;
		this->volume_stream = 1;

		this->max_channels = 25;
	}

	AR_Audio::~AR_Audio()
	{
		for (unsigned int i=0;i<this->sound_vector.size();i++)
		{
			this->sound_vector[i]->getSound()->release();
			this->sound_vector[i] = NULL;
		}	

		for (unsigned int i=0;i<this->stream_vector.size();i++)
		{
			this->stream_vector[i]->getSound()->release();
			this->stream_vector[i] = NULL;
		}	

		this->system->release();
	}

	bool AR_Audio::Init()
	{
		if (FMOD::System_Create(&this->system) != FMOD_OK)
			return false;	

		if (this->system->init(100, FMOD_INIT_NORMAL, 0) != FMOD_OK)
			return false;

		return true;
	}

	void AR_Audio::Update()
	{
		this->system->update();
	}	

	bool AR_Audio::LoadSound(std::string filename, std::string name, bool loop)
	{
		if (filename.length()==0 || name.length()==0)
			return false;

		FMOD::Sound	*sound_temp;
		FMOD_RESULT res;

		if (loop)
		{
			res = this->system->createSound(
				filename.c_str(), 		//filename
				FMOD_LOOP_NORMAL, 		//default audio
				0, 						//additional info
				&sound_temp);			//pointer to sound
		}
		else
		{
			res = this->system->createSound(
				filename.c_str(), 		//filename
				FMOD_DEFAULT, 			//default audio
				0, 						//additional info
				&sound_temp);			//pointer to sound
		}

		if (res != FMOD_OK)
		{
			g_engine->log_text.push_back("ERROR#AUDIO - Failed to load audio file \"" + filename + "\"");
			return false;
		}
		else
			g_engine->log_text.push_back("Loading audio file -> \"" + filename + "\" ... OK");

		AR_Sound *ar_sound = new AR_Sound();
		ar_sound->setName(name);
		ar_sound->setFilepath(filename);
		ar_sound->setType(AR_SOUND_SOUND);	
		ar_sound->setSound(sound_temp);

		this->sound_vector.push_back(ar_sound);

		return true;	
	}	

	bool AR_Audio::LoadStream(std::string filename, std::string name, bool loop)
	{
		if (filename.length()==0 || name.length()==0)
			return false;

		FMOD::Sound	*sound_temp;
		FMOD_RESULT res;

		if (loop)
		{
			res = this->system->createStream(
				filename.c_str(), 		//filename
				FMOD_LOOP_NORMAL, 		//default audio
				0, 						//additional info
				&sound_temp);			//pointer to sound
		}
		else
		{
			res = this->system->createStream(
				filename.c_str(), 		//filename
				FMOD_DEFAULT, 			//default audio
				0, 						//additional info
				&sound_temp);			//pointer to sound
		}

		if (res != FMOD_OK)
		{
			g_engine->log_text.push_back("ERROR#AUDIO - Failed to load audio stream \"" + filename + "\"");
			return false;
		}
		else
			g_engine->log_text.push_back("Loading audio stream -> \"" + filename + "\" ... OK");

		AR_Sound *ar_sound = new AR_Sound();
		ar_sound->setName(name);
		ar_sound->setFilepath(filename);
		ar_sound->setType(AR_SOUND_STREAM);	
		ar_sound->setSound(sound_temp);	

		this->stream_vector.push_back(ar_sound);

		return true;	
	}	

	FMOD::Channel* AR_Audio::Play(std::string name)
	{
		//limit the number of channels
		if(this->channels_playing.size()>=this->max_channels) return NULL;

		if (name.length()==0)
			return NULL;

		AR_Sound *ar_sound = this->FindSound(name);

		if (!ar_sound)
		{
			//g_engine->log_text.push_back("ERROR#AUDIO - Failed to find sound by name \"" + name +"\".");
			return NULL;
		}

		FMOD_RESULT res = system->playSound(
			FMOD_CHANNEL_FREE,
			ar_sound->getSound(),
			true,
			&ar_sound->channel);

		////misc settings
		float Hz;
		ar_sound->channel->getFrequency(&Hz);
		ar_sound->setFrequency_original(Hz);
		ar_sound->channel->setFrequency(ar_sound->getFrequency_original()*this->getFrequencyGlobal());

		if(ar_sound->getType()==AR_SOUND_SOUND)
			ar_sound->channel->setVolume(this->getVolumeSound());
		else
			ar_sound->channel->setVolume(this->getVolumeStream());

		ar_sound->channel->setPaused(false);
		////

		this->channels_playing.push_back(ar_sound->channel);

		if (res != FMOD_OK)
			return NULL;

		return ar_sound->channel;
	}

	FMOD::Channel* AR_Audio::Play_path(std::string filepath)
	{
		//limit the number of channels
		if(this->channels_playing.size()>=this->max_channels) return NULL;

		if (filepath.length()==0)
			return NULL;

		AR_Sound *ar_sound = this->FindSound_path(filepath);

		if (!ar_sound)
		{
			//g_engine->log_text.push_back("ERROR#AUDIO - Failed to find sound by path \"" + filepath +"\".");
			return NULL;
		}

		FMOD_RESULT res = system->playSound(
			FMOD_CHANNEL_FREE,
			ar_sound->getSound(),
			true,
			&ar_sound->channel);

		////misc settings
		float Hz;
		ar_sound->channel->getFrequency(&Hz);
		ar_sound->setFrequency_original(Hz);
		ar_sound->channel->setFrequency(ar_sound->getFrequency_original()*this->getFrequencyGlobal());

		if(ar_sound->getType()==AR_SOUND_SOUND)
			ar_sound->channel->setVolume(this->getVolumeSound());
		else
			ar_sound->channel->setVolume(this->getVolumeStream());

		ar_sound->channel->setPaused(false);
		////

		this->channels_playing.push_back(ar_sound->channel);

		if (res != FMOD_OK)
			return NULL;

		return ar_sound->channel;
	}

	bool AR_Audio::IsPlaying(std::string name)
	{
		if (name.length()==0)
			return false;

		AR_Sound *ar_sound = this->FindSound(name);
		if (ar_sound == NULL)
			return false;

		bool is_playing = false;
		ar_sound->getChannel()->isPlaying(&is_playing);

		return is_playing;
	}

	AR_Sound *AR_Audio::FindSound(std::string name)
	{
		if (name.length()==0)
			return NULL;

		for (unsigned int i=0;i<this->sound_vector.size();i++)
		{
			if (this->sound_vector[i]->getName() == name) return this->sound_vector[i];
		}

		for (unsigned int i=0;i<this->stream_vector.size();i++)
		{
			if (this->stream_vector[i]->getName() == name) return this->stream_vector[i];
		}
		
		return NULL;
	}

	AR_Sound *AR_Audio::FindSound_path(std::string filepath)
	{
		if (filepath.length()==0)
			return NULL;

		for (unsigned int i=0;i<this->sound_vector.size();i++)
		{
			if (this->sound_vector[i]->getFilepath() == filepath) return this->sound_vector[i];
		}

		for (unsigned int i=0;i<this->stream_vector.size();i++)
		{
			if (this->stream_vector[i]->getFilepath() == filepath) return this->stream_vector[i];
		}
		
		return NULL;
	}

	void AR_Audio::Stop(std::string name)
	{
		if (!this->IsPlaying(name))
			return;

		AR_Sound *ar_sound = this->FindSound(name);
		if (ar_sound == NULL) return;

		ar_sound->getChannel()->stop();
	}

	void AR_Audio::StopAll()
	{
		/*for (unsigned int i=0;i<this->sound_vector.size();i++)
		{
		this->sound_vector[i]->getChannel()->stop();
		}*/

		std::list<FMOD::Channel*>::iterator iter = this->channels_playing.begin();
		while (iter != this->channels_playing.end())
		{
			(*iter)->stop();
			iter++;
		}
	}

	//MISC
	void AR_Audio::UpdateFrequencyGlobal()
	{
		for (unsigned int i=0;i<this->sound_vector.size();i++)
		{
			this->sound_vector[i]->channel->setFrequency(
				this->sound_vector[i]->getFrequency_original()*this->getFrequencyGlobal());
		}

		for (unsigned int i=0;i<this->stream_vector.size();i++)
		{
			this->stream_vector[i]->channel->setFrequency(
				this->stream_vector[i]->getFrequency_original()*this->getFrequencyGlobal());
		}

		/*std::list<FMOD::Channel*>::iterator iter = this->channels_playing.begin();
		while (iter != this->channels_playing.end())
		{
		(*iter)->stop();
		}*/
	}

	void AR_Audio::UpdateVolumeSound()
	{
		for (unsigned int i=0;i<this->sound_vector.size();i++)
			this->sound_vector[i]->channel->setVolume(this->getVolumeSound());
	}

	void AR_Audio::UpdateVolumeStream()
	{
		for (unsigned int i=0;i<this->stream_vector.size();i++)
			this->stream_vector[i]->channel->setVolume(this->getVolumeStream());
	}

	//MISC PRIVATE
	void AR_Audio::setVolumePrivate(std::string name,float value)
	{
		AR_Sound *ar_sound = this->FindSound(name);
		if (ar_sound == NULL) return;

		ar_sound->channel->setVolume(value);
	}

	float AR_Audio::getVolumePrivate(std::string name)
	{
		AR_Sound *ar_sound = this->FindSound(name);
		if (ar_sound == NULL) return 0;

		float volume;
		ar_sound->channel->getVolume(&volume);

		return volume;
	}

	void AR_Audio::setFrequencyPrivate(std::string name,float Hz)
	{
		AR_Sound *ar_sound = this->FindSound(name);
		if (ar_sound == NULL) return;

		ar_sound->channel->setFrequency(Hz);
	}

	float AR_Audio::getFrequencyPrivate(std::string name)
	{
		AR_Sound *ar_sound = this->FindSound(name);
		if (ar_sound == NULL) return 0;

		float Hz;
		ar_sound->channel->getFrequency(&Hz);

		return Hz;
	}

	void AR_Audio::burySound()
	{
		std::list<FMOD::Channel*>::iterator iter = this->channels_playing.begin();

		while (iter != this->channels_playing.end())
		{
			bool is_playing = false;
			(*iter)->isPlaying(&is_playing);

			if (!is_playing)
			{
				//delete (*iter);
				iter = this->channels_playing.erase(iter);
			}
			else
				iter++;
		}
	}

	std::string getFileName(const string& path)
	{
		return path.substr(path.find_last_of("/\\")+1);
	}

	std::string getFileExtension(const string& path)
	{
		return path.substr(path.size()-4,4);
	}

	void AR_Audio::Load_SoundList(std::string file_path)
	{
		std::ifstream filein(file_path.c_str());
		if(!filein.is_open())
		{
			g_engine->log_text.push_back("ERROR#AUDIO - Failed to open sound list file \"" + file_path + "\"");
			return;
		}
		else
			g_engine->log_text.push_back("Opening sound list file -> \"" + file_path + "\" ... OK");

		std::string line;	
		while(std::getline(filein,line))
		{
			std::stringstream stream(line);
			std::string path;
			bool loop;

			if(stream >> path >> loop)
			{
				if(getFileExtension(path)==".ogg")
					this->LoadStream(path,getFileName(path).erase(getFileName(path).size()-4,4),loop);
				else
					this->LoadSound(path,getFileName(path).erase(getFileName(path).size()-4,4),loop);
			}
		}//while()

		filein.close();

		g_engine->log_text.push_back("Loading audio files from list -> \"" + file_path + "\" ... COMPLETED");

	}//Load_SoundList()

}