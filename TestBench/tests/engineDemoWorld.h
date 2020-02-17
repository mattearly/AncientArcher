/*
  Test building a small world with different shaders.
  Test Collision.
  Test basic sound effects functionality.
  Test demo asssets.
*/
#pragma once
#include "dry/addRandomColliderWorld.h"
#include "dry/addGodMovement.h"
#include <string>
#include "dry/addSkybox.h"
#include "../world/AAWorld.h"

void  testEngineDemoWorld()
{
  AAWorld worldEngine;
  DISPLAY->setWindowTitle("testEngineDemoWorld, use ALT+F4 to quit");

  addDemoSkybox(worldEngine);

  addGodMovement(worldEngine);  //todo: collidable movement

  addRandomWorld(worldEngine);

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
