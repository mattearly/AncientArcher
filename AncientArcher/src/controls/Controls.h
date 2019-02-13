#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../player/Player.h"
#include "../pickups/Pickups.h"

static float TIMEBETWEENFOOTSTEPS = 1.0f;

static struct Movement {
  bool positionChanged = false; // for footsteps

  bool forward = false, back = false;
  bool left = false, right = false;
  bool boost = false;

  bool jumped = false;
  bool canJumpAgain = true;
  bool onGround = true;
  bool falling = false;

  float timeSinceLastStep = 0;

} movedir;

class Controls {

public:
  Controls();
  ~Controls();

  void mouseMovement(float xpos, float ypos);

  void keyboardInput(Player *player, Pickups *pickups, float dtime, float gametime);

private:

  bool firstMouse;

  float lastX, lastY;

  float mouseSensitivity;



};

