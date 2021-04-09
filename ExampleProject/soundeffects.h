#pragma once
#include <AncientArcher/AncientArcher.h>
#include <iostream>

void setupsoundeffects()
{
  const char* zap16 = "..\\ExampleProject\\res\\zap16.ogg";
  static int zap16Id = AA::AddSoundEffect(zap16);

  const char* enchant = "..\\ExampleProject\\res\\enchant.ogg";
  static int enchantId = AA::AddSoundEffect(enchant);

  
  AA::AddToKeyHandling([](AA::KeyboardInput& kb) {
    if (kb.mouseButton1) {
      AA::PlaySoundEffect(zap16Id, true);
      return;
    }
    if (kb.mouseButton2) {
      AA::PlaySoundEffect(enchantId, true);
      return;
    }
    });

}