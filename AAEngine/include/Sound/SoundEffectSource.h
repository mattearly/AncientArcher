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

private:

	ALuint source;

	ALuint buffer = 0;

	//ALint state = AL_INITIAL;

};

} // end namespace AA
