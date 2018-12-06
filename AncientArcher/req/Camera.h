#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static int window_width = 800;
static int window_height = 600;

static struct Movement {
  bool forward = false, back = false;
  bool left = false, right = false;
} movedir;

class Camera {
public:
  Camera();
  ~Camera();

  friend class Controls;

  float FoV;

  glm::mat4 getViewMatrix();

  void move();

private:


  glm::vec3 Position;  // position of the camera
  glm::vec3 Front;  // where the camera is facing
  glm::vec3 Up;  // up relative to the camera's lean
  glm::vec3 Right;  // 90degree right to the camera's front

  glm::vec3 WorldUp;  // 0.0f, 1.0f, 0.0f

  float Yaw;
  float Pitch;

  void updateCameraVectors();

};

