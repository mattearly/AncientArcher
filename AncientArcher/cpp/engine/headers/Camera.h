#pragma once
#include <Global.h>
#include <Controls.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Controls;

class Camera {
public:
  Camera(
    const glm::vec3 startingPosition = glm::vec3(0.0f, 1.0f, 0.0f),
    const float lookingDir = 0.f,
    const float pitch = 0.f,
    const float fov = 45.f
  );

  friend class Controls;

  void update(Shader* shader);

  void updateProjectionMatrix(Shader* shader);

  // GETTERS
  glm::mat4 getViewMatrix();
  glm::vec3* getPosition();
  glm::vec3* getFront();
  glm::vec3* getRight();
  float getYaw();
  float getPitch();
  glm::mat4 getProjectionMatrix();

  // SETTERS
  void setPosition(glm::vec3 pos);

  void increaseYawAndPitch(float yawOff, float pitchOff);
  void increasePosition(glm::vec3 amount);

  void setToOrtho(Shader* shader);
  void setToPerspective(Shader* shader, float fov);

private:
  Camera() = delete;    // no default constructor

  glm::vec3 Position;   // point position of the camera.

  float FoV;            // Field of View
  float Yaw;            // left/right looking direction angle
  float Pitch;          // up/down looking direction angle
  
  void updateCameraVectors();
  
  glm::vec3 Front;      // direction from Postion. where the camera is facing
  glm::vec3 Right;      // direction from Postion. 90degree right to the camera's front
  glm::vec3 Up;         // up relative to the camera's lean

  // helper for increaseYawAndPitch()
  void increaseYaw(float offset);
  void increasePitch(float offset);
};
