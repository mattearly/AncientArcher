#pragma once
#include <Camera.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Controls {

public:

  enum class ControlType { FPS, TOP_DOWN };

  Controls();

  void mouseMovement(float xpos, float ypos);

  void playerKeyboardInput();

private:

  bool firstMouse;

  float lastX, lastY;

  float mouseSensitivity;

};
