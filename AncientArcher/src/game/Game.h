#pragma once
#include "../player/Player.h"
#include "../Pickups.h"
#include "../renderer/Renderer.h"
#include "../models/PrimativeManager.h"

class Game {
public:

  Player *player;

  PrimativeManager primativeManager;
  Pickups pickups;
  Renderer renderer;

  Game();
  ~Game();

  void mainLoop();

private:

  float lastFrame = 0.0f;

  float gameTime = 0.0f;

  float deltaTime = 0.0f;

};
