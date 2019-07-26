#pragma once
#include <World.h>
#include <keys.h>
#include <mouse.h>
#include "../pckgs/skybox/SkyboxRenderer.h"
#include "../pckgs/firstPersonPlayer/FirstPersonPlayer.h"

class Game {
public:

  World* world;

  FirstPersonPlayer* player;

  SkyboxRenderer* sky;

  Game();

  void mainLoop();

  keys keypress;

  mouse mousepos;

private:

  float lastFrame = 0.0f;

  float deltaTime = 0.0f;

};
