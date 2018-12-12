#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <Shader.h>
#include <Camera.h>
#include <Controls.h>
#include "TextureBank.h"
#include "Player.h"
#include "Pickups.h"

static float lastFrame = 0.0f;

class Game {
public:
  Game();
  ~Game();

  Camera *camera;
  Controls *control;
  Player *player;
  Pickups pickups;
  TextureBank texBank;

  void mainLoop();

  void reshapeWindow(GLFWwindow *window, int w, int h);
  void mouseHandler(GLFWwindow* window, double xpos, double ypos);

private:

  float deltaTime = 0.0f;

  GLFWwindow *window;
  Shader *shader;

  unsigned int cubeVAO, cubeVBO;
 
  void setupReshapeWindow();
  void setupMouseHandler();

};
