#pragma once
#include "../player/Player.h"
#include "../pickups/Pickups.h"
#include "../renderer/Renderer.h"
#include "../models/PrimativeManager.h"
#include "../models/Collision.h"

class Game {
public:

  Player *player;
  Collision collision;
  PrimativeManager primativeManager;
  Pickups pickups;
  Renderer renderer;

  Game();
  ~Game();

  void mainLoop();

private:

  float lastFrame = 0.0f;

  float deltaTime = 0.0f;

};
