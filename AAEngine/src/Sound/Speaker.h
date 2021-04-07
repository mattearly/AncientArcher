#pragma once
#include "../Base/UniqueInstance.h"
#include <AL\al.h>
#include <glm/glm.hpp>
#include <vector>

namespace AA
{
/// <summary>
/// Source of playback for a buffer.
/// </summary>
class Speaker : public UniqueInstance
{
public:
  void Play(const ALuint buffer);
  void PlayInterrupt(const ALuint buf);
  void PlayNoOverlap(const ALuint buf);

  void SetPosition(const float& x, const float& y, const float& z);
  void SetPosition(const glm::vec3& loc);
  void SetLocation(const float& x, const float& y, const float& z);
  void SetLocation(const glm::vec3& loc);

  void SetVelocity(float velocity);

  void SetDirection(const float& x, const float& y, const float& z);
  void SetDirection(const glm::vec3& dir);

  void SetVolume(const float gain);

  void SetLooping(const bool& opt);
  void SetRelative(const bool& opt);

  Speaker();
  ~Speaker();

private:

  ALuint p_Source;
  ALuint p_LastPlayedBuffer = 0;

  bool isPlaying();

};

} // end namespace AA
