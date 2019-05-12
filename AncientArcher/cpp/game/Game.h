#pragma once

#include "../../packages/masterRenderer/MasterRenderer.h"
#include "../../packages/skybox/SkyboxRenderer.h"
#include "../../packages/primatives/PrimativeRenderer.h"


class Game {
public:

  Player* player;
  PrimativeRenderer* primativeRenderer;
  SkyboxRenderer* skyboxRenderer;
  MasterRenderer masterRenderer;

  Game();

  void mainLoop();

private:

  float lastFrame = 0.0f;

  float deltaTime = 0.0f;

};
