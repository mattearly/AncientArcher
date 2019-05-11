#pragma once

#include "../../packages/masterRenderer/Renderer.h"
#include "../../packages/skybox/SkyboxRenderer.h"
#include "../../packages/primatives/PrimativeRenderer.h"
#include <Player.h>

class Game {
public:

  Player* player;
  PrimativeRenderer* primativeRenderer;
  SkyboxRenderer* skyboxRenderer;
  Renderer renderer;

  Game();

  void mainLoop();

private:

  float lastFrame = 0.0f;

  float deltaTime = 0.0f;

};
