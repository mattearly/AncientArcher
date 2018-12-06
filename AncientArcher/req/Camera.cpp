#include "Camera.h"
#include "../src/Player.h"
#include "../src/Game.h"

Camera::Camera() {
  FoV = 45.0f;
  
  Front = glm::vec3(0.0f, 0.0f, -1.0f);

  Position = glm::vec3(0.0f, 1.0f, 0.0f);
  WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  Yaw = -45.0f;;
  Pitch = 0.0f;

  updateCameraVectors();
}

Camera::~Camera() {}

void Camera::move() {
  bool moved = false;

  float velocity = MoveSpeed * deltaTime;

  if (movedir.forward) {
    Position += Front * velocity;
    moved = true;
  } 

  if (movedir.back) {
    Position -= Front * velocity;

    moved = true;

  }

  if (movedir.left) {
    Position -= Right * velocity;

    moved = true;

  }

  if (movedir.right) {
    Position += Right * velocity;

    moved = true;

  }

  updateCameraVectors();

}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);  
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
