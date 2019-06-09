#pragma once

#include <Camera.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Entity.h>

#include "../../pckgs/sideScrollerPlayer/SideScrollPlayer.h"

#include "../../pckgs/firstPersonPlayer/FirstPersonPlayer.h"

class FirstPersonPlayer;
class SideScrollPlayer;

class Controls {

public:

  Controls();

  void FPPmouseMovement(float xpos, float ypos);

  void SSmouseMovement(float xpos, float ypos);

  void fppKeyboardIn(FirstPersonPlayer* fpp);

  void sideScrollPlayerKeyboardInput(SideScrollPlayer* ssp);

  void entityKeyboardInput(Entity* entity);

  void entitiesKeyboardInput(Entity* entity, unsigned int numEntities);

private:

  bool firstMouse;

  float lastX, lastY;

  float mouseSensitivity;

};
