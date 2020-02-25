#pragma once
#include "AALights.h"
#include <glm/glm.hpp>
#include <memory>
#include "AAShaderManager.h"

class AAViewport
{
public:
  static AAViewport* getInstance();

  const glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);

  void setRenderDistance(float distance) noexcept;
  void setToPerspective() noexcept;
  void setToOrtho() noexcept;
  void setOrthoFieldSize(float left, float right, float bottom, float top) noexcept;
  void setOrthoFieldSize(glm::vec4 lrbt) noexcept;

  void setCurrentPosition(glm::vec3 pos);
  void setCurrentPitch(float pitch);
  void setCurrentYaw(float yaw);
  void shiftCurrentPosition(glm::vec3 offset);
  void shiftYawAndPith(float yawOffset, float pitchOffset);

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;
  const glm::vec3* getPosition() const noexcept;
  const glm::vec3* getFront() const noexcept;
  const glm::vec3* getRight() const noexcept;
  float getYaw() const noexcept;
  float getPitch() const noexcept;
  float getRenderDistance() const noexcept;
  bool hasViewportChanged() const noexcept;

  const glm::vec3& getLocation() const noexcept;

  friend class AADisplay;
  friend class AAShaderManager;

private:

  void updateCameraVectors();

  void updateViewMatrix(const Shader& shader);

  void updateProjectionMatrix(const Shader& shader);

  glm::vec3 mFront;
  glm::vec3 mRight;
  glm::vec3 mUp;

  glm::vec3 mPosition = glm::vec3(0);
  float mFieldOfView = 60.f;
  float mYaw = 0.f;
  float mPitch = 0.f;

  float mRenderDistance = 100.f;
  bool mProjectionChanged = false;
  glm::vec4 mOrthoFieldSize = glm::vec4(-1, 1, -1, 1);
  enum class RenderProjection { ORTHO, PERSPECTIVE } mRenderProjection = RenderProjection::PERSPECTIVE;

  void windowViewportChanged() noexcept;
  void windowViewportChangeProcessed() noexcept;

  void resetViewportVars();

};

