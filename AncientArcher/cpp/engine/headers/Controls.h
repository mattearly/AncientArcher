#pragma once
#include <Camera.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Controls {

public:

  struct KBKeys {
    bool keys[256] = keyDown;
  };


  enum class ControlType { FPS, TOP_DOWN };

  Controls();

  void mouseMovement(float xpos, float ypos, Camera *camera, ControlType type = ControlType::FPS);

  void playerKeyboardInput(ControlType type = ControlType::FPS);

private:

  bool firstMouse;

  float lastX, lastY;

  float mouseSensitivity;

};
