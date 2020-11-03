#pragma once
#include <vector>
#include <AL\al.h>
namespace AA{
/// <summary>
/// Stores reference access to the loaded sound buffers. 
/// Use SoundSource::Play on a ALuint that addSound(...) returns to play the sound.
/// </summary>
class SoundBuffers
{
public:
	static SoundBuffers* get();

	ALuint addSound(const char* filename);
	bool removeSound(const ALuint& buffer);

private:
	SoundBuffers();
	~SoundBuffers();
	std::vector<ALuint> buffers;

};
} // end namespace AA
