#include "Camera.h"

Camera::Camera() {
  Front = glm::vec3(0.0f, 0.0f, -1.0f);
  MovementSpeed = 0.005f;
  MouseSensitivity = 0.0325f;
  Zoom = 45.0f;

  Position = glm::vec3(0.0f, 1.0f, 0.0f);
  WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  Yaw = -45.0f;;
  Pitch = 0.0f;

  updateCameraVectors();
}


Camera::~Camera() {}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() {
  // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  Up = glm::normalize(glm::cross(Right, Front));
}
