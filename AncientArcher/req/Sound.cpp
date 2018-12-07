#include "Sound.h"
#include <cmath>


Sound::Sound() {}


Sound::~Sound() {}

float Sound::dBtoVolume(float dB) {
  return powf(10.0f, 0.05f * dB);
}

float Sound::VolumeTodB(float volume) {
  return 20.0f * log10f(volume);
}
