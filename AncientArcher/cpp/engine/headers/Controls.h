#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <keys.h>
#include <mouse.h>

class Controls {

public:

  void mouseMovement(float x, float y);

  void keyboardInput();
  
private:

  float mouseSensitivity = 0.09f;

};
