#pragma once

#include "Shader.h"
#include "Camera.h"
#include "Controls.h"
#include "TextureBank.h"
#include "Player.h"
#include "Pickups.h"
#include "displayManager/Display.h"

class Game {
public:
  Game();
  ~Game();

  float lastFrame = 0.0f;

  float gameTime = 0.0f;

  Display display;
  Camera *camera;
  Controls *control;
  Player *player;
  Pickups pickups;
  TextureBank texBank;

  void mainLoop();


private:

  float deltaTime = 0.0f;

  Shader *shader;
 

};
