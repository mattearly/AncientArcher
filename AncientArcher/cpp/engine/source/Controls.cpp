#include <Camera.h>
#include <Controls.h>
#include <Display.h>
#include <Sound.h>
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include "../../pckgs/firstPersonPlayer/Movement.h"

extern Movement movedir;  // from firstPersonPlayer/Player.cpp

extern Display display;  // from game/main.cpp

extern Camera camera;  // from game/main.cpp

Controls::Controls() {
  firstMouse = true;
  mouseSensitivity = 0.09f;
  lastX = 0.f;
  lastY = 0.f;
}

// ---- FIRST PERSON ---- //
void Controls::FPPmouseMovement(float xpos, float ypos)
{
  /// This block is first person controls
  /// Note: cursor visibility can be toggled in the Display class, it should be OFF for these controls
  ///
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

void Controls::FPPplayerKeyboardInput()
{
  if (glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // left click
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
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_S) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.forward = false;
      movedir.back = true;

    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_A) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.right = false;
      movedir.left = true;

    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_D) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.left = false;
      movedir.right = true;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_M) == GLFW_PRESS) {
    toggleAmbientWindyNight();
  }

  if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (movedir.onGround && movedir.canJumpAgain) {  //can jump again is to make the spacebar spam by holding it down not work
      movedir.jumped = true;
      movedir.canJumpAgain = false;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_0) == GLFW_PRESS) {
  }

  if (glfwGetKey(display.window, GLFW_KEY_1) == GLFW_PRESS) {
  }

  if (glfwGetKey(display.window, GLFW_KEY_2) == GLFW_PRESS) {
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

void Controls::sideScrollPlayerKeyboardInput(Entity* entity)
{

  if (glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    // left click
    playbowsound();
  }

  if (glfwGetKey(display.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(display.window, true);  // closes app
  }

  if (glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_W) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_S) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_A) == GLFW_PRESS)
  {
    entity->moveBy(glm::vec3(-0.1f, 0.0f, 0.0f));  // needs delta time just testing

  }

  if (glfwGetKey(display.window, GLFW_KEY_D) == GLFW_PRESS)
  {
    entity->moveBy(glm::vec3(0.1f, 0.0f, 0.0f));  // needs delta time just testing
  }

  if (glfwGetKey(display.window, GLFW_KEY_M) == GLFW_PRESS)
  {
    toggleAmbientWindyNight();
  }

  if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_0) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_1) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_2) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_W) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_S) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_A) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_D) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_RELEASE)
  {
  }

}

// ---- SIDESCROLLER ---- //
void Controls::SSmouseMovement(float xpos, float ypos)
{

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

  // TODO HANDLE NEW MOUSE POSITION 
  // OR WAIT FOR CLICK TO HANDLE NEW MOUSE POSITION in sideScrollPlayerKeyboardInput()

}
