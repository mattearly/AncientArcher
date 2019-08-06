#pragma once
#include <World.h>
#include <memory.h>
#include <keys.h>
#include <mouse.h>
#include "../pckgs/skybox/Skybox.h"
#include "../pckgs/firstPersonPlayer/FirstPersonPlayer.h"

class Game {
public:

  World* world;
  FirstPersonPlayer* player;
  Skybox* sky;

  std::shared_ptr<keys> keypress;
  std::shared_ptr<mouse> mousepos;

  Game();

  void mainLoop();

private:

  void update();
  void update(float dt);
  void render();

};
