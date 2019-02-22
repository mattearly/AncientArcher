#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Player;

static float TIMEBETWEENFOOTSTEPS = 2.0f;

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
