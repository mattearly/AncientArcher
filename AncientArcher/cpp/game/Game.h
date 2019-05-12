#pragma once

#include "../../pckgs/masterRenderer/MasterRenderer.h"
#include "../../pckgs/skybox/SkyboxRenderer.h"
#include "../../pckgs/primatives/PrimativeRenderer.h"


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
