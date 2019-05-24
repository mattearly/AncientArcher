#pragma once
#include <Camera.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Entity.h>

class Controls {

public:

  Controls();

  void FPPmouseMovement(float xpos, float ypos);

  void SSmouseMovement(float xpos, float ypos);

  void FPPplayerKeyboardInput();

  void sideScrollPlayerKeyboardInput(Entity* entity);

  void sideScrollPlayerKeyboardInput(Entity* entity, unsigned int numEntities );

private:

  bool firstMouse;

  float lastX, lastY;

  float mouseSensitivity;

};
