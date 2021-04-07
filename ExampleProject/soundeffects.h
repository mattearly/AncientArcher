#pragma once
#include <AncientArcher/AncientArcher.h>
#include <iostream>


void setupsoundeffects()
{
  const char* zap16 = "..\\ExampleProject\\res\\zap16.ogg";
  static int speakerId = AA::AddSpeaker();
  static int zap16Id = AA::AddSoundEffect(zap16);

  AA::AddToKeyHandling([](AA::KeyboardInput& kb){
    if (kb.mouseButton1)
      AA::PlaySoundEffect(speakerId, zap16Id, false);
  });

}