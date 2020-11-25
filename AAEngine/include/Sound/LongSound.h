#pragma once
#include <AL/al.h>
#include <cstdint>
#include <sndfile.h>
namespace AA {
class LongSound
{
public:

	LongSound(const char* filename);
	~LongSound();

	void Play();
	void Pause();
	void Resume();
	void Stop();

	void SetVolume(const float& val);

	void UpdatePlayBuffer();

	ALint GetPlayingState();

private:

	static constexpr int BUFFER_COUNT = 4;
	static constexpr int BUFFER_SAMPLES = 8192;
	
	//uint32_t p_SlowUpdateLoopId;

	ALuint p_Source;
	ALuint p_Buffers[BUFFER_COUNT];
	SNDFILE* p_Sndfile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;

	LongSound() = delete;
};

}