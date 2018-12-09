#pragma once
#include <SDL2/SDL_mixer.h>

//todo: improve sound, see https://youtu.be/Vjm--AqG04Y 

static const int NUMFOOTSTEPS = 6;
static Mix_Chunk *footstepsoundeffects[NUMFOOTSTEPS];
void playfootstepsound();

static const int NUMGRUNTS = 4;
static Mix_Chunk *gruntsoundeffects[NUMGRUNTS];
void playgruntsound();

static Mix_Chunk *landingsoundeffect;
void playlandingsound();

//static Mix_Chunk *dooropensoundeffect;
//void playdoorsound();
//
//static Mix_Chunk *doorclosesoundeffect;
//void playdoorclosesound();
//
//static Mix_Chunk *pickupsoundeffect;
//void playpickupsound();
//
//static Mix_Chunk *destroysoundeffect;
//void playdestroysound();
//static Mix_Chunk *switchsoundeffect;
//void playswitchsound();
//static Mix_Chunk *smashsoundeffect;
//void playsmashsound();
//
//static Mix_Chunk *knocksoundeffect;
//void playknocksound();
//
//static Mix_Chunk *demolitionsoundeffect;
//void playdemolitionsound();
//
//static Mix_Chunk *deadsoundeffect;
//void playdeadsound();
//
//static Mix_Music *winsoundeffect;
//void playwinsound();
//
//static Mix_Music *defaultmusic;
//void playdefaultmusic();

void initsound();

