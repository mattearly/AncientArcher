#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static int window_width = 1024;
static int window_height = 800;
static float camstart[] = { 5.0f, 2.5f, 5.0f };

class Camera {
public:
  Camera();
  ~Camera();

  friend class Controls;
  friend class Constraints;

  float FoV;


  glm::mat4 getViewMatrix();

  //return a reference to Position
  glm::vec3* getPosition();

  //return a reference to Front
  glm::vec3* getFront();

  //return a reference to Right
  glm::vec3* getRight();

  //return copy of Yaw
  float getYaw();

  //return copy of Pitch
  float getPitch();


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

