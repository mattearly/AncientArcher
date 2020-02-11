/*
  Test building a small world with different shaders.
  Test Collision.
  Test basic sound effects functionality.
*/
#pragma once
#include "dry/addRandomColliderWorld.h"
#include "dry/addGodMovement.h"

void  testEngineFuncsDemo_4()
{
  AAWorld worldEngine;
  DISPLAY->setWindowTitle("AA Demo 4 (sound=num keys) - Close Window To Continue");

  //objs.loadConfig("../TestBench/config/sounds.txt");
  //static AASound sound;
  //sound.addSoundEffects(objs.getAllSounds());

  //addSkybox(worldEngine, "drakeq");

  addGodMovement(worldEngine);  //todo: collidable movement

  addRandomColliderworld(worldEngine);

  //auto soundHotkeys = [](AAKeyBoardInput& keys)
  //{
  //  if (keys.n1)
  //  {
  //    sound.playSoundEffect(0);
  //    return true;
  //  }
  //  if (keys.n2)
  //  {
  //    sound.playSoundEffect(1);
  //    return true;
  //  }
  //  if (keys.n3)
  //  {
  //    sound.playSoundEffect(2);
  //    return true;
  //  }
  //  if (keys.n4)
  //  {
  //    sound.playSoundEffect(3);
  //    return true;
  //  }
  //  if (keys.n5)
  //  {
  //    sound.playSoundEffect(4);
  //    return true;
  //  }
  //  if (keys.n6)
  //  {
  //    sound.playSoundEffect(5);
  //    return true;
  //  }
  //  if (keys.n7)
  //  {
  //    sound.playSoundEffect(6);
  //    return true;
  //  }
  //  if (keys.n8)
  //  {
  //    sound.playSoundEffect(7);
  //    return true;
  //  }
  //  if (keys.n9)
  //  {
  //    sound.playSoundEffect(8);
  //    return true;
  //  }
  //  if (keys.n0)
  //  {
  //    sound.playSoundEffect(9);
  //    return true;
  //  }
  //  return false;
  //};
  //worldEngine.addToTimedOutKeyHandling(soundHotkeys);

  switch (worldEngine.run())
  {
  case -1:
    std::cout << "Keys not set.\n";
    break;
  case -2:
    std::cout << "Mouse not set.\n";
    break;
  case -3:
    std::cout << "Scroll not set.\n";
    break;
  default:
    break;
  }

}
