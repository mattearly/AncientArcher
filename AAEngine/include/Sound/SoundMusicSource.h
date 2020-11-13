#pragma once
#include <AL/al.h>
#include <cstdint>
namespace AA {
class SoundMusicSource
{
public:

	void Play(const uint32_t id);
	void Pause(const uint32_t id);
	void Stop(const uint32_t id);

	void SetVolume(const float& val);

	void UpdatePlayingBuffers(const uint32_t id);

	SoundMusicSource();
	~SoundMusicSource();

private:

	ALuint p_Source;

};
}