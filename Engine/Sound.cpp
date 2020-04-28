/*
Sound
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.
* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#include "Sound.h"
#include <SDL2/SDL.h>
namespace AA
{

Sound::Sound() noexcept
{
  // init sound
  SDL_Init(SDL_INIT_AUDIO);
  /* initialize sound */
  const int audio_rate = 22050;
  const Uint16 audio_format = AUDIO_S16SYS;
  const int audio_channels = 2;
  const int audio_buffers = 4096;
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
    exit(-4);
  }
}

Sound::~Sound()
{
  //un-init sound
  SDL_Quit();
}

void Sound::addSoundEffect(std::string path)
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

void Sound::addMusicTrack(std::string path)
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

void Sound::addSoundEffects(const std::string& dir, const std::vector<std::string>& paths)
{
  for (auto p : paths)
  {
    std::string soundPath = dir + p;
    Mix_Chunk* tmpchunk = Mix_LoadWAV(soundPath.c_str());
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

  ////output list of sound effects
  //int count = 0;
  //std::cout << "Sound Effects\n";
  //for (auto se : mSoundEffects)
  //{
  //  std::cout << '\t' << count++ << ". " << se << '\n';
  //}
}

void Sound::addMusicTracks(std::vector<std::string> paths)
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

  //int count = 0;
  //std::cout << "Music Tracks\n";
  //for (auto mt : mMusicTracks)
  //{
  //  std::cout << '\t' << count++ << ". " << mt << '\n';
  //}
}

void Sound::playSoundEffect(int which) const
{
  if (which > mSoundEffects.size() - 1)
  {
    // out of range -- don't attempt to play sound
    //std::cout << "Sound out of range: " << which << " > " << mSoundEffects.size() - 1 << '\n';
    return;
  }

  Mix_PlayChannel(-1, mSoundEffects.at(which), 0);

}

void Sound::playMusicTrack(int which)
{
  if (which > mMusicTracks.size())
  {
    // out of range -- don't attempt to play music
    return;
  }

  if (!mMusicPlaying && !mMusicPaused) {
    Mix_PlayMusic(mMusicTracks.at(which), -1);
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
}  // end namespace AA