#pragma once
#include <GLFW/glfw3.h>
#include <Camera.h>



class Controls {
public:
  Controls();
  ~Controls();

  void mouseMovement(double xpos, double ypos, Camera *cam);

  void keyboardInput(GLFWwindow *window, Camera *cam);

private:

  bool firstMouse;

  float lastX, lastY;

  float sensitivity;

};

