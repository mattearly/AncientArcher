#include <glm/glm.hpp>

#include "../globals.h"
#include "Controls.h"

#include "../Constraints.h"
#include "../player/Player.h"
#include "../sound/Sound.h"
#include "../game/Game.h"

#include <iostream>
#include <math.h>

Movement movedir;

Controls::Controls() {
  firstMouse = true;
  mouseSensitivity = 0.09f;
  lastX = 0.f;
  lastY = 0.f;
}

Controls::~Controls() {}

void Controls::mouseMovement(float xpos, float ypos) {

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  camera.Yaw += xoffset;
  camera.Pitch += yoffset;

  if (camera.Pitch > 89.0f) {
    camera.Pitch = 89.0f;
  }
  else if (camera.Pitch < -89.0f) {
    camera.Pitch = -89.0f;
  }

  camera.updateCameraVectors();

}

void Controls::keyboardInput(Player *player) {

  if (glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // main left click action 
    //if (player->getLastAttackTime() + player->getAttackSpeed() < gameTime) {
      //player->attack(gametime);
    //}
  }

  if (glfwGetKey(display.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(display.window, true);
  }

  if (glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.boost = true;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_W) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.back = false;
      movedir.forward = true;
      movedir.positionChanged = true;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_S) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.forward = false;
      movedir.back = true;
      movedir.positionChanged = true;

    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_A) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.right = false;
      movedir.left = true;
      movedir.positionChanged = true;

    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_D) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.left = false;
      movedir.right = true;
      movedir.positionChanged = true;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (movedir.onGround && movedir.canJumpAgain) {  //can jump again is to make the spacebar spam by holding it down not work
      movedir.jumped = true;
      movedir.canJumpAgain = false;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_0) == GLFW_PRESS) {
    player->selectWeapon(0);
  }

  if (glfwGetKey(display.window, GLFW_KEY_1) == GLFW_PRESS) {
    player->selectWeapon(1);
  }

  if (glfwGetKey(display.window, GLFW_KEY_2) == GLFW_PRESS) {
    player->selectWeapon(2);
  }

  if (glfwGetKey(display.window, GLFW_KEY_W) == GLFW_RELEASE) {
    movedir.forward = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_S) == GLFW_RELEASE) {
    movedir.back = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_A) == GLFW_RELEASE) {
    movedir.left = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_D) == GLFW_RELEASE) {
    movedir.right = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    movedir.boost = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    movedir.canJumpAgain = true;
  }
}