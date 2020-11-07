#include "../../include/Sound/SoundMusicSource.h"
#include "../../include/Sound/SoundBufferManager.h"
#include <cassert>
namespace AA {
void SoundMusicSource::Play(const uint32_t id)
{
	// fill buffer queue
	SoundBufferManager::get()->StartLongSoundPlay(id, p_Source);

}

void SoundMusicSource::Pause(const uint32_t id)
{
	//todo
}

void SoundMusicSource::Stop(const uint32_t id)
{
	//todo
}

void SoundMusicSource::UpdatePlayingBuffers(const uint32_t id)
{
	SoundBufferManager::get()->UpdateLongPlayingBuffers(id, p_Source);
}

SoundMusicSource::SoundMusicSource()
{
	alGenSources(1, &p_Source);
	assert(alGetError() == AL_NO_ERROR && "Could not create source");

	/* Set parameters so mono sources play out the front-center speaker and
		* won't distance attenuate. */
	alSource3i(p_Source, AL_POSITION, 0, 0, -1);
	alSourcei(p_Source, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcei(p_Source, AL_ROLLOFF_FACTOR, 0);
	assert(alGetError() == AL_NO_ERROR && "Could not set source parameters");
}

SoundMusicSource::~SoundMusicSource()
{
	alDeleteSources(1, &p_Source);
}

};