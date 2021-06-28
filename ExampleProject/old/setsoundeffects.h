#pragma once
#include <AncientArcher/AncientArcher.h>
#include <iostream>

bool is_sound_effects_setup = false;
void setupsoundeffects() {
  if (is_sound_effects_setup)
    return;

  is_sound_effects_setup = true;
  const char* zap16 = "..\\ExampleProject\\res\\zap16.ogg";
  static int zap16Id = AA::AddSoundEffect(zap16);

  const char* enchant = "..\\ExampleProject\\res\\enchant.ogg";
  static int enchantId = AA::AddSoundEffect(enchant);

  std::cout << ", 1,2 = ability sounds";
  AA::AddToKeyHandling([](AA::KeyboardInput& kb) {
    if (kb.n1) {
      AA::PlaySoundEffect(zap16Id, false);
      return;
    }
    if (kb.n2) {
      AA::PlaySoundEffect(enchantId, false);
      return;
    }
  });
}