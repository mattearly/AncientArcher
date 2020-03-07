#pragma once
#include <glm/glm.hpp>
#include <memory>

enum class RenderProjection { ORTHO, PERSPECTIVE };

class AACamera
{
public:
  AACamera();

  void updateCameraVectors();

  // setters
  void setToPerspective() noexcept;
  void setCurrentPosition(glm::vec3 pos);
  void setCurrentPitch(float pitch);
  void setCurrentYaw(float yaw);
  void shiftCurrentPosition(const glm::vec3& offset);
  void shiftYawAndPith(float yawOffset, float pitchOffset);

  // experimental
  void setToOrtho() noexcept;
  void setOrthoFieldSize(float left, float right, float bottom, float top) noexcept;
  void setOrthoFieldSize(glm::vec4 lrbt) noexcept;

  // getters
  const glm::vec3& getLocation() const noexcept;
  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix() const;
  const glm::vec3* getPosition() const noexcept;
  const glm::vec3* getFront() const noexcept;
  const glm::vec3* getRight() const noexcept;
  float getYaw() const noexcept;
  float getPitch() const noexcept;
  float getRenderDistance() const noexcept;
  const int& getID() const noexcept;

  friend class AAWorld;
  
private:

  glm::vec3        mFront;
  glm::vec3        mRight;
  glm::vec3        mUp;
  glm::vec3        mPosition;
  float            mFieldOfView;
  float            mYaw;
  float            mPitch;
  float            mRenderDistance;
  bool             mProjectionChanged;
  glm::vec4        mOrthoFieldSize;
  int              mUniqueViewportID;
  RenderProjection mRenderProjection;

  void resetViewportVars();

  void setRenderDistance(float distance) noexcept;

};
