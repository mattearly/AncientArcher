#pragma once
#include <SDL2/SDL_mixer.h>

class Sound {
public:
  Sound();
  ~Sound();

  /* dB to volume and vol to dB from
     https://youtu.be/Vjm--AqG04Y  */

  float dBtoVolume(float dB);
  float VolumeTodB(float volume);

};

