#pragma once
#include <AL\al.h>
#include <glm/glm.hpp>
//class SoundBufferManager;
namespace AA{
/// <summary>
/// Source of playback for a buffer.
/// </summary>
class SoundEffectSource
{
public:
	void Play(const ALuint buffer);

	void SetPosition(const float& x, const float& y, const float& z);

	void SetLocation(const float& x, const float& y, const float& z);

	void SetPosition(const glm::vec3& loc);

	void SetLocation(const glm::vec3& loc);

	SoundEffectSource();
	~SoundEffectSource();

private:

	ALuint p_Source;
	ALuint p_Buffer = 0;

	//ALint state = AL_INITIAL;

};

} // end namespace AA
