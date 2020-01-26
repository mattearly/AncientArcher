#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>

class AASound
{
public:
  AASound();
  ~AASound();

  void addSoundEffect(std::string path);
  void addMusicTrack(std::string path);

  void addSoundEffects(std::vector<std::string> paths);
  void addMusicTracks(std::vector<std::string> paths);

  void playSoundEffect(int which) const;
  void playMusicTrack(int which);

private:

  std::vector<Mix_Chunk*> mSoundEffects;
  std::vector<Mix_Music*> mMusicTracks;

  bool mMusicPlaying = false;
  bool mMusicPaused = false;

};

