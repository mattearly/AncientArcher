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
  Mix_Chunk* tmpchunk = Mix_LoadWAV(path.c_str());
  // check that sound load succeed before adding to list
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

void AASound::addMusicTrack(std::string path)
{
  Mix_Music* tmpchunk = Mix_LoadMUS(path.c_str());
  // check that sound load succeed before adding to list
  if (tmpchunk != nullptr)
  {
    mMusicTracks.push_back(tmpchunk);
  }
  else
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
      "Couldn't load audio: %s",
      Mix_GetError());
  }
}

void AASound::addSoundEffects(std::vector<std::string> paths)
{
  for (auto p : paths)
  {
    Mix_Chunk* tmpchunk = Mix_LoadWAV(p.c_str());
    // check that sound load succeed before adding to list
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

  //output list of sound effects
  int count = 0;
  std::cout << "Sound Effects\n";
  for (auto se : mSoundEffects)
  {
    std::cout << '\t' << count++ << ". " << se << '\n';
  }
}

void AASound::addMusicTracks(std::vector<std::string> paths)
{
  for (auto p : paths)
  {
    Mix_Music* tmpchunk = Mix_LoadMUS(p.c_str());
    // check that sound load succeed before adding to list
    if (tmpchunk != nullptr)
    {
      mMusicTracks.push_back(tmpchunk);
    }
    else
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        "Couldn't load audio: %s",
        Mix_GetError());
    }
  }

  int count = 0;
  std::cout << "Music Tracks\n";
  for (auto mt : mMusicTracks)
  {
    std::cout << '\t' << count++ << ". " << mt << '\n';
  }
}

void AASound::playSoundEffect(int which) const
{
  if (which > mSoundEffects.size() - 1)
  {
    // out of range -- don't attempt to play sound
    std::cout << "Sound out of range: " << which << " > " << mSoundEffects.size() - 1 << '\n';

    return;
  }

  Mix_PlayChannel(-1, mSoundEffects[which], 0);

}

void AASound::playMusicTrack(int which) const
{
  if (which > mMusicTracks.size())
  {
    // out of range -- don't attempt to play music
    return;
  }

  if (!mMusicPlaying && !mMusicPaused) {
    Mix_PlayMusic(mMusicTracks[which], -1);
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
