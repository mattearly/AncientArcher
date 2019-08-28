#pragma once
#include <Model.h>
#include <memory.h>
#include <keys.h>
#include <mouse.h>
#include <scroll.h>
#include <Scene.h>
#include <Lighting.h>

class Game {
public:

  Game();

  void mainLoop();

private:

  std::shared_ptr<keys>     _keypress;
  std::shared_ptr<mouse>    _mousepos;
  std::shared_ptr<scroll>   _scrolling;
  std::shared_ptr<Camera>   _camera;
  std::shared_ptr<Shader>   _shader;
  std::shared_ptr<Lighting> _lighting;

  Scene scene;

  void update();
  void update(float dt);
  void render();

  void moveCamHelper(float dt);

};
