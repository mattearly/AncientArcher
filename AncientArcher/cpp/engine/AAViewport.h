#pragma once
#include "AALights.h"
#include <glm/glm.hpp>
#include <mearly\Shader.h>
#include <memory>

class AAViewport
{
public:

  static AAViewport* getInstance();
  const glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);

  void setRenderDistance(float distance);
  void setToPerspective();
  void setToOrtho();
  void setOrthoFieldSize(float left, float right, float bottom, float top);
  void setOrthoFieldSize(glm::vec4 lrbt);

  void setCurrentPosition(glm::vec3 pos);
  void setCurrentPitch(float pitch);
  void setCurrentYaw(float yaw);
  void shiftCurrentPosition(glm::vec3 offset);
  void shiftYawAndPith(float yawOffset, float pitchOffset);

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;
  const glm::vec3* getPosition() const;
  const glm::vec3* getFront() const;
  const glm::vec3* getRight() const;
  float getYaw() const;
  float getPitch() const;
  float getRenderDistance() const;
  bool hasViewportChanged() const;

  friend class AADisplay;
  friend class AAShaderManager;

private:

  void updateCameraVectors();

  void updateViewMatrix(const Shader& shader);

  void updateProjectionMatrix(const Shader& shader);

  glm::vec3 mPosition = glm::vec3(0);
  float mFieldOfView = 60.f;
  float mYaw = 0.f;
  float mPitch = 0.f;
  glm::vec3 mFront = glm::vec3(0, 0, -1);
  glm::vec3 mRight = glm::vec3(1, 0, 0);
  glm::vec3 mUp = glm::vec3(0, 1, 0);

  float mRenderDistance = 100.f;
  bool mProjectionChanged = false;
  glm::vec4 mOrthoFieldSize = glm::vec4(-1, 1, -1, 1);
  enum class RenderProjection { ORTHO, PERSPECTIVE } mRenderProjection = RenderProjection::PERSPECTIVE;

  void windowViewportChanged();
  void windowViewportChangeProcessed();

  void resetViewportVars();

};

