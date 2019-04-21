#pragma once
#include "Player.h"
#include "Renderer.h"
#include "PrimativeManager.h"

class Game {
public:

  Player *player;
  PrimativeManager primativeManager;
  Renderer renderer;

  Game();

  void mainLoop();

private:

  float lastFrame = 0.0f;

  float deltaTime = 0.0f;

};
