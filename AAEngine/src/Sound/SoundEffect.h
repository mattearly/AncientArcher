#pragma once
#include "../Base/UniqueInstance.h"
#include <AL\al.h>
#include <string>
namespace AA {
class SoundEffect : public UniqueInstance
{
public:
  SoundEffect(const char* filename);
  ~SoundEffect();
  //ALuint AddSoundEffect(const char* filename);
  //bool RemoveSoundEffect(const ALuint& buffer);
  //void DestroyAllBuffers();
  ALuint _Buffer;
  std::string _FilePath;
private:
};
}  // namespace AA
