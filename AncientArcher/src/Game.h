#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Controls.h"
//#include <PrimativeManager.h>
#include "TextureBank.h"
#include "Player.h"
#include "Pickups.h"


class Game {
public:
  Game();
  ~Game();

  float lastFrame = 0.0f;

  float gameTime = 0.0f;

  Camera *camera;
  Controls *control;
  Player *player;
  Pickups pickups;
  TextureBank texBank;
  //PrimativeManager primMan;

  void mainLoop();

  void reshapeWindow(GLFWwindow *window, int w, int h);
  void mouseHandler(GLFWwindow* window, double xpos, double ypos);

private:

  float deltaTime = 0.0f;

  GLFWwindow *window;
  Shader *shader;
 
  void setupReshapeWindow();
  void setupMouseHandler();

};
