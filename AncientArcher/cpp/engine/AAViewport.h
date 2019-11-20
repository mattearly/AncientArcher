#pragma once
#include <glm/glm.hpp>
#include <mearly\Shader.h>
#include <memory>

class AAViewport
{
public:
  static AAViewport* getInstance();

  void setCurrentPosition(glm::vec3 pos);
  void shiftCurrentPosition(glm::vec3 offset);
  void shiftYawAndPith(float yawOffset, float pitchOffset);

  glm::mat4 getViewMatrix();
  glm::mat4 getProjectionMatrix();
  glm::vec3* getPosition();
  glm::vec3 getFront();
  glm::vec3* getRight();
  float getYaw();
  float getPitch();

private:

  void updateCameraVectors();

  glm::vec3 mPosition = glm::vec3(0);
  float mFieldOfView = 60.f;
  float mYaw = 0.f;
  float mPitch = 0.f;
  glm::vec3 mFront = glm::vec3(0, 0, -1);
  glm::vec3 mRight = glm::vec3(1, 0, 0);
  glm::vec3 mUp = glm::vec3(0, 1, 0);

  const glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);

  std::shared_ptr<Shader> mShader = std::make_shared<Shader>("../AncientArcher/shader/VertexShader.glsl", "../AncientArcher/shader/FragmentShader.glsl");
};

