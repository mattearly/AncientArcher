#include "Camera.h"
#include "../globals.h"

Camera::Camera() {
  FoV = 85.0f;
  Front = glm::vec3(0.0f, 0.0f, -1.0f);
  Position = glm::vec3(camstart[0], camstart[1], camstart[2]); // starting position
  Up = { 0.f, 1.f, 0.f };
  Right = { 0.f, 0.f, 0.f };
  WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  Yaw = 45.0f;  // looking direction
  Pitch = 0.0f;
  updateCameraVectors();
}

Camera::~Camera() {}

void Camera::update() {
  glm::mat4 view = getViewMatrix();
  shader.setMat4("view", view);
}

// needs called to update the FoV and/or window_width window_height, and draw distances
void Camera::updateProjectionMatrix() {
  glm::mat4 projection = glm::perspective(glm::radians(FoV), (float)display.window_width / (float)display.window_height, 0.01f, 100.0f);
  shader.setMat4("projection", projection);
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3* Camera::getPosition() {  
  return &Position;
}

glm::vec3 * Camera::getFront() {
  return &Front;
}

glm::vec3 * Camera::getRight() {
  return &Right;
}

float Camera::getYaw() {
  return Yaw;
}

float Camera::getPitch() {
  return Pitch;
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}
