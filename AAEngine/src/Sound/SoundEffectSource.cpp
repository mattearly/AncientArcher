#include "..\..\include\Sound\SoundEffectSource.h"
#include <iostream>

namespace AA
{

void SoundEffectSource::Play(const ALuint buf)
{
	// update stored buffer to play if we need to
	if (buf != p_Buffer) {
		p_Buffer = buf;
		alSourcei(p_Source, AL_BUFFER, (ALint)buf);
	}

	alSourcePlay(p_Source);

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

void SoundEffectSource::SetPosition(const float& x, const float& y, const float& z) {
	SetLocation(x, y, z);
}

void SoundEffectSource::SetLocation(const float& x, const float& y, const float& z) {
	alSource3f(p_Source, AL_POSITION, x, y, z);
}

void SoundEffectSource::SetPosition(const glm::vec3& loc)
{
	SetLocation(loc);
}

void SoundEffectSource::SetLocation(const glm::vec3& loc)
{
	alSource3f(p_Source, AL_POSITION, loc.x, loc.y, loc.z);
}

void SoundEffectSource::SetVolume(const float& gain)
{
	float newvol = 0;
	if (gain < 0.f)
	{
		newvol = 0;
	}
	else if (gain > 5.f)
	{
		newvol = 5.f;
	}
	else
	{
		newvol = gain;
	}

	alSourcef(p_Source, AL_GAIN, newvol);

	ALenum ALerror = AL_NO_ERROR;
	ALerror = alGetError();
	auto geterrorstring = [](ALenum ALerror) {
		switch (ALerror) {
		case AL_NO_ERROR:       return std::string("AL_NO_ERROR - (No error)."); break;
		case AL_INVALID_NAME:       return std::string("AL_INVALID_NAME - Invalid Name paramater passed to AL call."); break;
		case AL_INVALID_ENUM:       return std::string("AL_INVALID_ENUM - Invalid parameter passed to AL call."); break;
		case AL_INVALID_VALUE:      return std::string("AL_INVALID_VALUE - Invalid enum parameter value."); break;
		case AL_INVALID_OPERATION:  return std::string("AL_INVALID_OPERATION"); break;
		case AL_OUT_OF_MEMORY:      return std::string("AL_OUT_OF_MEMORY"); break;
		default:            return std::string("AL Unknown Error."); break;
		}
	};
	std::cout << geterrorstring(ALerror) << '\n';
	//std::cout << getALErrorString(ALerror) << std::endl;
	//throw("error setting suond on music source\n");
}



SoundEffectSource::SoundEffectSource()
{

	float pitch = 1.f;
	float gain = 1.f;
	float position[3] = { 0,0,0 };
	float velocity[3] = { 0,0,0 };
	bool loop = false;

	alGenSources(1, &p_Source);
	alSourcef(p_Source, AL_PITCH, pitch);
	alSourcef(p_Source, AL_GAIN, gain);
	alSource3f(p_Source, AL_POSITION, position[0], position[1], position[2]);
	alSource3f(p_Source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
	alSourcei(p_Source, AL_LOOPING, loop);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
	if (alGetError() != AL_NO_ERROR)
	{
		std::cout << "error setting sound effect source data\n";
	}
}

SoundEffectSource::~SoundEffectSource()
{
	alDeleteSources(1, &p_Source);
}

} //end namespace AA
