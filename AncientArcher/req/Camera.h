#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera();
  ~Camera();

  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  float Yaw;
  float Pitch;

  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  glm::mat4 getViewMatrix();

  /* graphics card locatoins of the model, view, location */
  unsigned int model, view, location;

private:
  void updateCameraVectors();
};

