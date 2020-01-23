#include "AASound.h"
#include <SDL2/SDL.h>
#include <iostream>

AASound::AASound()
{
  // init sound
  SDL_Init(SDL_INIT_AUDIO);
  /* initialize sound */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 4096;
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
    exit(-4);
  }
}

AASound::~AASound()
{
  //un-init sound
  SDL_Quit();
}

void AASound::addSoundEffect(std::string path)
{
  //todo
}

void AASound::addMusicTrack(std::string path)
{
  //todo
}

void AASound::addSoundEffects(std::vector<std::string> paths)
{
  for (auto p : paths)
  {
    Mix_Chunk* tmpchunk = Mix_LoadWAV(p.c_str());

    if (tmpchunk != nullptr)
    {
      mSoundEffects.push_back(tmpchunk);
    }
    else
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        "Couldn't load audio: %s",
        Mix_GetError());
    }

  }

  // check all sound effects after loading them
  int count = 0;
  for (auto se : mSoundEffects)
  {
    std::cout << count++ << ". sound passed check\n";
  }
}

void AASound::addMusicTracks(std::vector<std::string> paths)
{
  //todo
}

void AASound::playSoundEffect(int which)
{
  if (which > mSoundEffects.size() - 1)
  {
    // out of range -- don't attempt to play sound
    std::cout << "Sound out of range: " << which << " > " << mSoundEffects.size() - 1 << '\n';

    return;
  }

  Mix_PlayChannel(-1, mSoundEffects[which], 0);

}

void AASound::playMusicTrack(int which)
{
  if (which > mMusicTrack.size())
  {
    // out of range -- don't attempt to play music
    return;
  }

  if (!mMusicPlaying && !mMusicPaused) {
    Mix_PlayMusic(mMusicTrack[which], -1);
    mMusicPlaying = true;
  }
  else if (mMusicPlaying && !mMusicPaused) {
    Mix_PauseMusic();
    mMusicPaused = true;
  }
  else if (mMusicPlaying && mMusicPaused) {
    Mix_ResumeMusic();
    mMusicPaused = false;
  }
}
