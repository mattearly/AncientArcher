#include "Camera.h"

Camera::Camera() {
  FoV = 45.0f;

  Front = glm::vec3(0.0f, 0.0f, -1.0f);

  Position = glm::vec3(camstart[0], camstart[1], camstart[2]); // starting position

  WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  Yaw = 45.0f;;
  Pitch = 0.0f;

  updateCameraVectors();
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3* Camera::getPosition() {  
  return &Position;
}

glm::vec3 * Camera::getFront() {
  return &Front;
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
