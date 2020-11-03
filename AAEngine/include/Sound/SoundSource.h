#pragma once
#include <AL\al.h>
namespace AA{
/// <summary>
/// Source of playback for a buffer.
/// </summary>
class SoundSource
{
public:
	void Play(const ALuint buffer);
	SoundSource();
	~SoundSource();
private:
	ALuint source;
	float pitch = 1.f;
	float gain = 1.f;
	float position[3] = {0,0,0};
	float velocity[3] = {0,0,0};
	bool loop = false;
	ALuint buffer = 0;

	ALint state = AL_INITIAL;
	
};

} // end namespace AA
