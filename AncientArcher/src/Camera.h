#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera();
  ~Camera();

  friend class Controls;
  friend class Constraints;

  float FoV;

  float camstart[3] = { 0.0f, 2.0f, 0.0f };

  void update();

  void updateProjectionMatrix();

  // GETTERS
  //return the calculated view matrix
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

