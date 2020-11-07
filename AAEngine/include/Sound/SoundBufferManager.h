#pragma once
#include <vector>
#include <AL\al.h>
#include <sndfile.h>
#include "SoundSource.h"
#include <unordered_map>

namespace AA {

//
//typedef struct StreamPlayer {
//	ALuint buffers[NUM_MUSIC_BUFFERS];
//	SNDFILE* sndfile;
//	SF_INFO sfinfo;
//	short* membuf;
//	ALenum format;
//} StreamPlayer;

/// <summary>
/// Stores reference access to the loaded sound buffers.
/// addSoundEffect(...) returns id to play the sound.
/// Use SoundSource::Play(...) on the id to play the sound.
/// </summary>
class SoundBufferManager
{
public:
	static SoundBufferManager* get();

	ALuint addSoundEffect(const char* filename);
	bool removeSoundEffect(const ALuint& buffer);

	uint32_t addLongPlaySound(const char* filename);
	bool removeLongPlaySound(const uint32_t id);
	
	friend class SoundSource;

private:
	SoundBufferManager();
	~SoundBufferManager();
	std::vector<ALuint> p_SoundEffectBuffers;

	const int NUM_MUSIC_BUFFERS = 4;
	const int BUFFER_SAMPLES = 8192;

	struct StreamBuffer {
		ALuint buffers[4];
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		ALenum format;
		//int loopid;
	};

	std::unordered_map<uint32_t, StreamBuffer*> p_LongSounds; 
	int PlayMusic(const uint32_t id, SoundSource& src);
	int UpdatePlay(const uint32_t id, SoundSource& src);


	//StreamPlayer *p_StreamPlayer;


};
} // end namespace AA
