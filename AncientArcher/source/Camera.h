#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera();
  ~Camera();

  friend class Controls;
  friend class Player;

  float FoV;

  const float camstart[3] = { 1.5f, 0.5f, 1.5f };

  void update();

  void updateProjectionMatrix();

  // GETTERS
  glm::mat4 getViewMatrix();
  glm::vec3* getPosition();
  glm::vec3* getFront();
  glm::vec3* getRight();
  float getYaw();
  float getPitch();
  glm::mat4 getProjectionMatrix();

private:

  glm::vec3 Position; 

  // left/right looking direction angle
  float Yaw;

  // up/down looking direction angle
  float Pitch;

  // where the camera is facing
  glm::vec3 Front; 

  // up relative to the camera's lean
  glm::vec3 Up;  

  // 90degree right to the camera's front
  glm::vec3 Right;  

  // world's up direction: 0.0f, 1.0f, 0.0f
  glm::vec3 WorldUp;  

  void updateCameraVectors();

};
