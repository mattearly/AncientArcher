#include "..\..\include\Sound\SoundSource.h"
#include <iostream>

namespace AA
{
void LoadSound(const char* soundfile)
{
}

void SoundSource::Play(const ALuint buf)
{
	if (buf != buffer)
	{
		buffer = buf;
	}
	alSourcei(source, AL_BUFFER, (ALint)buf);

	alSourcePlay(source);

	state = AL_PLAYING;
	std::cout << "playing sound\n";
	while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	{
		std::cout << "currently playing sound\n";
		alGetSourcei(source, AL_SOURCE_STATE, &state);
	}
	std::cout << "done playing sound\n";
}

SoundSource::SoundSource()
{
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);
	alSource3f(source, AL_POSITION, position[0], position[1], position[2]);
	alSource3f(source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
	alSourcei(source, AL_LOOPING, loop);
	alSourcei(source, AL_BUFFER, buffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(1, &source);
}

} //end namespace AA
