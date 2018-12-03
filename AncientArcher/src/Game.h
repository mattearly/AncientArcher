#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <Shader.h>

class Game {
public:
  Game();
  ~Game();
  void mainLoop();

  void reshapeWindow(GLFWwindow *window, int w, int h);
  void processInput(GLFWwindow *window);

private:
  GLFWwindow *window;
  Shader *shader;

  unsigned int VAO, VBO, EBO;
  unsigned int texture001, texture002;

  void setupReshapeWindow();

};
