#pragma once
#include <vector>
#include <AL\al.h>
#include <sndfile.h>
#include "SoundSource.h"

namespace AA {


//static constexpr int NUM_MUSIC_BUFFERS = 4;
//static constexpr int BUFFER_SAMPLES = 8192;
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
/// addSound(...) returns id to play the sound.
/// Use SoundSource::Play(...) on the id to play the sound.
/// </summary>
class SoundBufferManager
{
public:
	static SoundBufferManager* get();

	ALuint addSoundEffect(const char* filename);
	bool removeSoundEffect(const ALuint& buffer);
	
	//void InitPlayer(SoundSource& source);
	//void DeletePlayer();
	//int OpenPlayerFile(const char *filename);
	//void ClosePlayerFile();
	//int StartPlayer(SoundSource& source);
	//int UpdatePlayer(SoundSource& source);

private:
	SoundBufferManager();
	~SoundBufferManager();
	std::vector<ALuint> p_SoundEffectBuffers;



	//StreamPlayer *p_StreamPlayer;


};
} // end namespace AA
