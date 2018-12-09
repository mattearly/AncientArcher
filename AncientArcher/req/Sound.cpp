#include "Sound.h"
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>
#include "mearly.h"

bool soundReady = false;

void playfootstepsound() {

  if (!soundReady) initsound();

  int dist = mearly::NTKR(0, NUMFOOTSTEPS - 1);

  Mix_PlayChannel(-1, footstepsoundeffects[dist], 0);
}

void playgruntsound() {

  if (!soundReady) initsound();

  int dist = mearly::NTKR(0, NUMGRUNTS - 1);

  Mix_PlayChannel(-1, gruntsoundeffects[dist], 0);
}

void playlandingsound() {

  if (!soundReady) initsound();

  Mix_PlayChannel(-1, landingsoundeffect, 0);
}

//void playdoorsound() {
//
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, dooropensoundeffect, 0);
//}
//
//void playdoorclosesound() {
//
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, doorclosesoundeffect, 0);
//}
//
//void playpickupsound() {
//
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, pickupsoundeffect, 0);
//}
//
//void playdestroysound() {
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, destroysoundeffect, 0);
//}
//
//void playswitchsound() {
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, switchsoundeffect, 0);
//}
//
//void playsmashsound() {
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, smashsoundeffect, 0);
//}
//
//void playknocksound() {
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, knocksoundeffect, 0);
//}
//
//void playdemolitionsound() {
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, demolitionsoundeffect, 0);
//}
//
//void playdeadsound() {
//  if (!soundReady) initsound();
//
//  Mix_PlayChannel(-1, deadsoundeffect, 0);
//}
//
//
//void playwinsound() {
//  if (!soundReady) initsound();
//
//  Mix_PlayMusic(winsoundeffect, -1);
//}
//
//void playdefaultmusic() {
//  if (!soundReady) initsound();
//
//  Mix_PlayMusic(defaultmusic, -1);
//}

//--------------------init sound---------------
void initsound() {
  SDL_Init(SDL_INIT_AUDIO);
  /* initialize sound */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 4096;
  if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
    exit(-3);
  }

  /* load sound assets */
  // load footstep variations
  footstepsoundeffects[0] = Mix_LoadWAV("../AncientArcher/res/sfx/footstep01.wav");  // load footsteps
  footstepsoundeffects[1] = Mix_LoadWAV("../AncientArcher/res/sfx/footstep02.wav");  // load footsteps
  footstepsoundeffects[2] = Mix_LoadWAV("../AncientArcher/res/sfx/footstep03.wav");  // load footsteps
  footstepsoundeffects[3] = Mix_LoadWAV("../AncientArcher/res/sfx/footstep04.wav");  // load footsteps
  footstepsoundeffects[4] = Mix_LoadWAV("../AncientArcher/res/sfx/footstep05.wav");  // load footsteps
  footstepsoundeffects[5] = Mix_LoadWAV("../AncientArcher/res/sfx/footstep06.wav");  // load footsteps
  for (auto &se : footstepsoundeffects) {  //test footstep load
    if (se == nullptr) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        "Couldn't load audio: %s",
        Mix_GetError());
    }
  }

  // load jump sound effect (grunts)
  gruntsoundeffects[0] = Mix_LoadWAV("../AncientArcher/res/sfx/grunt01.wav");  // load grunts
  gruntsoundeffects[1] = Mix_LoadWAV("../AncientArcher/res/sfx/grunt02.wav");
  gruntsoundeffects[2] = Mix_LoadWAV("../AncientArcher/res/sfx/grunt03.wav");
  gruntsoundeffects[3] = Mix_LoadWAV("../AncientArcher/res/sfx/grunt04.wav");
  for (auto &se : gruntsoundeffects) {  //test grunt load
    if (se == nullptr) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
        "Couldn't load audio: %s",
        Mix_GetError());
    }
  }
  
  landingsoundeffect = Mix_LoadWAV("../AncientArcher/res/sfx/feetlanding.wav");
  if (landingsoundeffect == nullptr) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
      "Couldn't load audio: %s",
      Mix_GetError());
  }



  //// load door sound effect
  //dooropensoundeffect = Mix_LoadWAV("dooropen.wav");
  //if (dooropensoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}
  //doorclosesoundeffect = Mix_LoadWAV("doorclose.wav");
  //if (doorclosesoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  //// load pickup sound effect
  //pickupsoundeffect = Mix_LoadWAV("pickup.wav");
  //if (pickupsoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}


  //// load destory sound effect
  //destroysoundeffect = Mix_LoadWAV("explode.wav");
  //if (destroysoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  //// load switch sound effect
  //switchsoundeffect = Mix_LoadWAV("crank.wav");
  //if (switchsoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  //// load smash sound effect
  //smashsoundeffect = Mix_LoadWAV("chop.wav");
  //if (smashsoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  //// load knock sound effect
  //knocksoundeffect = Mix_LoadWAV("knock.wav");
  //if (knocksoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  //// load demolition sound effect
  //demolitionsoundeffect = Mix_LoadWAV("demo.wav");
  //if (demolitionsoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  //// load demolition sound effect
  //deadsoundeffect = Mix_LoadWAV("dead.wav");
  //if (deadsoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  //// load win music
  //winsoundeffect = Mix_LoadMUS("guiletheme.wav");
  //if (winsoundeffect == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  //// load default music
  //defaultmusic = Mix_LoadMUS("musicbox.wav");
  //if (defaultmusic == nullptr) {
  //  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
  //    "Couldn't load audio: %s",
  //    Mix_GetError());
  //}

  soundReady = true;  //kk sound is good to go

}
