#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Player;

class Controls {

public:

  Controls();

  void mouseMovement(float xpos, float ypos);

  void playerKeyboardInput();

private:

  bool firstMouse;

  float lastX, lastY;

  float mouseSensitivity;

};
