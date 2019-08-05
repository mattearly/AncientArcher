#pragma once
#include <World.h>
#include <keys.h>
#include <mouse.h>
#include "../pckgs/skybox/Skybox.h"
#include "../pckgs/firstPersonPlayer/FirstPersonPlayer.h"

class Game {
public:

  World* world;

  FirstPersonPlayer* player;

  Skybox* sky;

  Game();

  void mainLoop();

private:

  float lastFrame = 0.0f;
  float deltaTime = 0.0f;

  keys keypress;
  mouse mousepos;

  void update(float dt);
  void render();

};
