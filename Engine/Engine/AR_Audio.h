#include "Advanced2D.h"

#define AR_SOUND_SOUND 0
#define AR_SOUND_STREAM 1

#pragma once

namespace Advanced2D
{
	class AR_Sound
	{
	private:
		int type;
		float frequency;//because it gets changed, and we need initial frequency
		std::string name;
		std::string filepath;
		FMOD::Sound	*sound;

	public:
		//zbog nekog vraga ne radi ako se stavi nek temp pokazivac, pa se on dalje koristi u play
		FMOD::Channel *channel;	

		AR_Sound(void);
		~AR_Sound(void);

		std::string getName() {return this->name;}
		void setName(std::string value) {this->name = value;}

		std::string getFilepath() {return this->filepath;}
		void setFilepath(std::string value) {this->filepath = value;}

		FMOD::Sound* getSound() {return this->sound;}
		void setSound(FMOD::Sound* sound) {this->sound=sound;}

		FMOD::Channel* getChannel() {return this->channel;}
		void setChannel(FMOD::Channel* channel) {this->channel=channel;}

		int getType(){return this->type;};
		void setType(int value){this->type=value;};

		float getFrequency_original(){return this->frequency;};
		void setFrequency_original(float value){this->frequency=value;};	

	};

	class AR_Audio
	{
	private:
		FMOD::System *system;

		std::vector <AR_Sound*> sound_vector;
		std::vector <AR_Sound*> stream_vector;

		float frequency;
		float volume_sound;
		float volume_stream;

	public:
		AR_Audio();
		~AR_Audio();

		int max_channels;

		std::list<FMOD::Channel*> channels_playing;

		FMOD::System* getSystem() {return this->system;}

		bool Init();
		void Update();//must be called once per frame

		void Load_SoundList(std::string file_path);

		bool LoadSound(std::string filename, std::string name, bool loop);
		bool LoadStream(std::string filename, std::string name, bool loop);

		FMOD::Channel* Play(std::string name);
		FMOD::Channel* Play_path(std::string filepath);
		void Stop(std::string name);
		void StopAll();

		AR_Sound* FindSound(std::string name);
		AR_Sound* FindSound_path(std::string filepath);
		bool IsPlaying(std::string name);

		//global
		float getFrequencyGlobal(){return this->frequency;};
		void setFrequencyGlobal(float value){this->frequency=value;};

		float getVolumeSound(){return this->volume_sound;};
		void setVolumeSound(float value){this->volume_sound=value;};

		float getVolumeStream(){return this->volume_stream;};
		void setVolumeStream(float value){this->volume_stream=value;};

		void UpdateFrequencyGlobal();	
		void UpdateVolumeSound();
		void UpdateVolumeStream();

		//private
		float getVolumePrivate(std::string name);
		void setVolumePrivate(std::string name,float value);
		float getFrequencyPrivate(std::string name);
		void setFrequencyPrivate(std::string name,float Hz);

		void burySound();

	};

}