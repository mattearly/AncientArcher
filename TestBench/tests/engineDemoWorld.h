/*
  Test building a small world with different shaders.
  Test Collision.
  Test basic sound effects functionality.
  Test demo asssets.
*/
#pragma once
#include "dry/addGodMovement.h"
#include "dry/addSkybox.h"
#include "../world/AAWorld.h"
#include <string>
#include "dry/addRandomWorld.h"

int testEngineDemoWorld()
{
  AAWorld worldEngine("Test Engine Demo World");
  addCubeSkybox(worldEngine);
  addGodMovement(worldEngine);
  addRandomWorld(worldEngine);
  return worldEngine.run();
}
