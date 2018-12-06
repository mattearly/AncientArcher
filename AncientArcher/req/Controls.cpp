#include "Controls.h"
#include <glm/glm.hpp>

Controls::Controls() {
  firstMouse = true;
  sensitivity = 0.1f;
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

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  cam->Yaw += xoffset;
  cam->Pitch += yoffset;

  if (cam->Pitch > 89.0f) {
    cam->Pitch = 89.0f;
  } else if (cam->Pitch < -89.0f) {
    cam->Pitch = -89.0f;
  }

  cam->updateCameraVectors();

}

void Controls::keyboardInput(GLFWwindow * window, Camera *cam) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  //float cameraSpeed = 2.5 * deltaTime;

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

  cam->move();
}