#pragma once
#include "Shader.h"
#include "Camera.h"
#include "Controls.h"
#include "TextureBank.h"
#include "Player.h"
#include "Pickups.h"
#include "displayManager/Display.h"
#include "Renderer.h"
#include "primatives/PrimativeManager.h"
class Game {
public:
  Game();
  ~Game();

  Display *display;
  Camera *camera;
  Controls *control;
  Player *player;

  PrimativeManager primativeManager;
  Pickups pickups;
  TextureBank texBank;
  Renderer renderer;

  void mainLoop();

private:

  float lastFrame = 0.0f;

  float gameTime = 0.0f;

  float deltaTime = 0.0f;

  Shader *shader;
 

};
