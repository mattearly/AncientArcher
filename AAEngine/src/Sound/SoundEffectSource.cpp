#include "..\..\include\Sound\SoundEffectSource.h"
#include <iostream>

namespace AA
{

void SoundEffectSource::Play(const ALuint buf)
{
	// update stored buffer to play if we need to
	if (buf != buffer) { 
		buffer = buf; 
		alSourcei(source, AL_BUFFER, (ALint)buf);
	}

	alSourcePlay(source);

	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "error in playing sound effect\n";
	}

	//state = AL_PLAYING;
	//std::cout << "playing sound\n";
	//while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
	//{
	//	std::cout << "currently playing sound\n";
	//	alGetSourcei(source, AL_SOURCE_STATE, &state);
	//}
	//std::cout << "done playing sound\n";
}

SoundEffectSource::SoundEffectSource()
{

	float pitch = 1.f;
	float gain = 1.f;
	float position[3] = { 0,0,0 };
	float velocity[3] = { 0,0,0 };
	bool loop = false;

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, pitch);
	alSourcef(source, AL_GAIN, gain);
	alSource3f(source, AL_POSITION, position[0], position[1], position[2]);
	alSource3f(source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
	alSourcei(source, AL_LOOPING, loop);
	alSourcei(source, AL_BUFFER, buffer);
	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "error setting sound effect source data\n";
	}
}

SoundEffectSource::~SoundEffectSource()
{
	alDeleteSources(1, &source);
}

} //end namespace AA
