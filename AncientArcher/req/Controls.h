#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Camera.h>
#include "../src/Player.h"
#include "../src/Pickups.h"


class Controls {

public:
  Controls();
  ~Controls();

  void mouseMovement(double xpos, double ypos, Camera *cam);

  void keyboardInput(GLFWwindow *window, Camera *cam, Player *player, Pickups *pickups, float time);

private:

  bool firstMouse;

  float lastX, lastY;

  float mouseSensitivity;

  float TIMEBETWEENFOOTSTEPS = 1.0f;

  struct Movement {
    bool positionChanged; // for footsteps

    bool forward = false, back = false;
    bool left = false, right = false;
    bool boost = false;

    bool jumped = false;
    bool canJumpAgain = true;
    bool onGround = true;
    bool falling = false;

    float timeSinceLastStep = 0;

  } movedir;

};

