#pragma once
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera();
  ~Camera();

  friend class Controls;

  float FoV;

  const float camstart[3] = { 3,.5,3 };

  void update(Shader* shader);

  void updateProjectionMatrix(Shader* shader);

  // GETTERS
  glm::mat4 getViewMatrix();
  glm::vec3* getPosition();
  glm::vec3* getFront();
  glm::vec3* getRight();
  glm::vec3* getWorldUp();
  float getYaw();
  float getPitch();
  glm::mat4 getProjectionMatrix();

  // SETTERS
  void setPosition(glm::vec3 pos);

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
