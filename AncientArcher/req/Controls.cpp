#include "Controls.h"
#include <glm/glm.hpp>
#include "Constraints.h"

#include "../src/Player.h"

Controls::Controls() {
  firstMouse = true;
  mouseSensitivity = 0.08f;
}

Controls::~Controls() {}

void Controls::mouseMovement(double xpos, double ypos, Camera *cam) {

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

  cam->Yaw += xoffset;
  cam->Pitch += yoffset;

  if (cam->Pitch > 89.0f) {
    cam->Pitch = 89.0f;
  } else if (cam->Pitch < -89.0f) {
    cam->Pitch = -89.0f;
  }

  cam->updateCameraVectors();

}

void Controls::keyboardInput(GLFWwindow * window, Camera *cam, float time) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    movedir.boost = true;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    movedir.back = false;
    movedir.forward = true;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    movedir.forward = false;
    movedir.back = true;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    movedir.right = false;
    movedir.left = true;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    movedir.right = true;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
    movedir.forward = false;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
    movedir.back = false;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
    movedir.left = false;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
    movedir.right = false;
  }  
  
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    movedir.boost = false;
  }

  if (movedir.back || movedir.forward || movedir.left || movedir.right) {

    float velocity = MoveSpeed * time;
    if (movedir.boost && !movedir.back) {
      velocity += velocity;
    }

    if (movedir.forward) cam->Position += cam->Front * velocity;
    if (movedir.back) cam->Position -= cam->Front * velocity;
    if (movedir.right) cam->Position += cam->Right * velocity;
    if (movedir.left) cam->Position -= cam->Right * velocity;

    cam->Position.y = camstart[1];

    /* clamp to level */
    if (cam->Position.x > world_width) {
      if (movedir.forward) cam->Position.x -= cam->Front.x * velocity;
      if (movedir.back)    cam->Position.x += cam->Front.x * velocity;
      if (movedir.right)   cam->Position.x -= cam->Right.x * velocity;
      if (movedir.left)    cam->Position.x += cam->Right.x * velocity;
    } else if (cam->Position.x < 0.0f) {
      if (movedir.forward) cam->Position.x -= cam->Front.x * velocity;
      if (movedir.back)    cam->Position.x += cam->Front.x * velocity;
      if (movedir.right)   cam->Position.x -= cam->Right.x * velocity;
      if (movedir.left)    cam->Position.x += cam->Right.x * velocity;
    } else if (cam->Position.z > world_width) {
      if (movedir.forward) cam->Position.z -= cam->Front.z * velocity;
      if (movedir.back)    cam->Position.z += cam->Front.z * velocity;
      if (movedir.right)   cam->Position.z -= cam->Right.z * velocity;
      if (movedir.left)    cam->Position.z += cam->Right.z * velocity;
    } else if (cam->Position.z < 0.0f) {
      if (movedir.forward) cam->Position.z -= cam->Front.z * velocity;
      if (movedir.back)    cam->Position.z += cam->Front.z * velocity;
      if (movedir.right)   cam->Position.z -= cam->Right.z * velocity;
      if (movedir.left)    cam->Position.z += cam->Right.z * velocity;
    } 
  }

}