#pragma once
#include <vector>
#include <AL\al.h>

namespace AA {
/// <summary>
/// Stores reference access to the loaded sound buffers.
/// addSound(...) returns id to play the sound.
/// Use SoundSource::Play(...) on the id to play the sound.
/// </summary>
class SoundBuffers
{
public:
	static SoundBuffers* get();

	ALuint addSoundEffect(const char* filename);
	bool removeSoundEffect(const ALuint& buffer);


private:

	SoundBuffers();
	~SoundBuffers();

	std::vector<ALuint> p_SoundEffectBuffers;


};
} // end namespace AA
