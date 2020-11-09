#pragma once
#include <AL\al.h>
//class SoundBufferManager;
namespace AA{
/// <summary>
/// Source of playback for a buffer.
/// </summary>
class SoundEffectSource
{
public:
	void Play(const ALuint buffer);

	SoundEffectSource();
	~SoundEffectSource();

	friend class SoundBufferManager;

private:

	ALuint source;
	float pitch = 1.f;
	float gain = 1.f;
	float position[3] = {0,0,0};
	float velocity[3] = {0,0,0};
	bool loop = false;
	ALuint buffer = 0;

	//ALint state = AL_INITIAL;

};

} // end namespace AA
