#include "../../include/Sound/SoundMusicSource.h"
#include "../../include/Sound/SoundBufferManager.h"
#include "../../include/Sound/SoundDevice.h"
#include <cassert>
#include <iostream>
#include <AL\alc.h>
namespace AA {
void SoundMusicSource::Play(const uint32_t id)
{
	// fill buffer queue
	SoundBufferManager::get()->StartLongSoundPlay(id, p_Source);
}

void SoundMusicSource::Pause(const uint32_t id)
{
	SoundBufferManager::get()->PauseLongSoundPlay(id, p_Source);
}

void SoundMusicSource::Stop(const uint32_t id)
{
	SoundBufferManager::get()->StopLongSoundPlay(id, p_Source);
}

void SoundMusicSource::SetVolume(const float& val)
{
	float newvolume = val;
	if (newvolume < 0.f)  //can't be negative
	{
		newvolume = 0.f;
	}
	else if (newvolume > 5.f)
	{
		// cap at 5
		newvolume = 5.f;
	}

	//alcSuspendContext(SoundDevice::get()->p_ALCContext);

	alSourcef(p_Source, AL_GAIN, newvolume);


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
		}; };
	std::cout << geterrorstring(ALerror) << '\n';
	//std::cout << getALErrorString(ALerror) << std::endl;
	//std::cout << getALErrorString(ALerror) << std::endl;
	//throw("error setting suond on music source\n");


//alcProcessContext(SoundDevice::get()->p_ALCContext);
}

//void SoundMusicSource::UpdatePlayingBuffers(const uint32_t id)
//{
//	SoundBufferManager::get()->UpdateLongPlayingBuffers(id, p_Source);
//}

SoundMusicSource::SoundMusicSource()
{
	alGenSources(1, &p_Source);
	assert(alGetError() == AL_NO_ERROR && "Could not create source");

	/* Set parameters so mono sources play out the front-center speaker and
		* won't distance attenuate. */
		//alSource3i(p_Source, AL_POSITION, 0, 0, -1);
	//alSourcei(p_Source, AL_SOURCE_RELATIVE, AL_TRUE);
	//alSourcei(p_Source, AL_ROLLOFF_FACTOR, 0);
	//assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");
}

SoundMusicSource::~SoundMusicSource()
{
	alDeleteSources(1, &p_Source);
}

};