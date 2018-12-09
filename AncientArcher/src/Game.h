#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <Shader.h>
#include <Camera.h>
#include <Controls.h>
#include <Lighting.h>
#include "TextureBank.h"

static float lastFrame = 0.0f;

class Game {
public:
  Game();
  ~Game();

  Camera *camera;
  Controls *control;
  TextureBank texBank;
  Lighting light;

  void mainLoop();

  void reshapeWindow(GLFWwindow *window, int w, int h);
  void mouseHandler(GLFWwindow* window, double xpos, double ypos);

private:

  float deltaTime = 0.0f;

  GLFWwindow *window;
  Shader *shader;
  Shader *lightShader;

  unsigned int cubeVAO, lightVAO, cubeVBO, lightVBO;
 
  void setupReshapeWindow();
  void setupMouseHandler();

};
