#pragma once
#include <AncientArcher/AncientArcher.h>
void addbackgroundtalking(){
  AA::AddMusic("..\\ExampleProject\\res\\talking_stereo.wav");

  AA::AddToKeyHandling([](AA::KeyboardInput& kb){
    if (kb.p){
      AA::PlayMusic();
      return;
      }
  });
}