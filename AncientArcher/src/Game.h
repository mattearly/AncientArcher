#pragma once
#include "Player.h"
#include "Pickups.h"
#include "Renderer.h"
#include "primatives/PrimativeManager.h"

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
