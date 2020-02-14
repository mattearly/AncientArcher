#include "AAViewport.h"
#include "AADisplay.h"
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>

void AAViewport::updateCameraVectors()
{
  glm::vec3 front;
  front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  front.y = sin(glm::radians(mPitch));
  front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

  mFront = glm::normalize(front);
  mRight = glm::normalize(glm::cross(mFront, WORLD_UP));
  mUp = glm::normalize(glm::cross(mRight, mFront));
}

void AAViewport::updateViewMatrix(const Shader& shader)
{
  shader.use();
  shader.setMat4("view", getViewMatrix());
}

void AAViewport::updateProjectionMatrix(const Shader& shader)
{
  shader.use();
  shader.setMat4("projection", getProjectionMatrix());
}

void AAViewport::resetViewportVars()
{
  mPosition = glm::vec3(0);
  mFieldOfView = 60.f;
  mYaw = 0.f;
  mPitch = 0.f;
  updateCameraVectors();
  mRenderDistance = 100.f;
  mRenderProjection = RenderProjection::PERSPECTIVE;
  mProjectionChanged = false;
  mOrthoFieldSize = glm::vec4(-1, 1, -1, 1);
}

void AAViewport::windowViewportChanged() noexcept
{
  mProjectionChanged = true;
}

void AAViewport::windowViewportChangeProcessed() noexcept
{
  mProjectionChanged = false;
}

void AAViewport::setToPerspective() noexcept
{
  mRenderProjection = RenderProjection::PERSPECTIVE;
}

void AAViewport::setToOrtho() noexcept
{
  //AAViewport::getInstance()->setOrthoFieldSize(
  //  glm::vec4(
  //    0, AADisplay::getInstance()->getWindowWidth(),
  //    0, AADisplay::getInstance()->getWindowHeight()
  //  )
  //);

  mRenderProjection = RenderProjection::ORTHO;
}

void AAViewport::setOrthoFieldSize(float left, float right, float bottom, float top) noexcept
{
  mOrthoFieldSize = glm::vec4(left, right, bottom, top);
}

void AAViewport::setOrthoFieldSize(glm::vec4 lrbt) noexcept
{
  mOrthoFieldSize = lrbt;
}

AAViewport* AAViewport::getInstance()
{
  static std::unique_ptr<AAViewport> viewport = std::make_unique<AAViewport>();
  return viewport.get();
}

void AAViewport::setRenderDistance(float distance) noexcept
{
  mRenderDistance = distance;
  windowViewportChanged();
}

void AAViewport::setCurrentPosition(glm::vec3 pos)
{
  mPosition = pos;
  updateCameraVectors();
}

void AAViewport::setCurrentPitch(float pitch)
{
  mPitch = pitch;
  if (mPitch > 89.9f)
  {
    mPitch = 89.9f;
  }
  else if (mPitch < -89.9f)
  {
    mPitch = -89.9f;
  }
  updateCameraVectors();
}

void AAViewport::setCurrentYaw(float yaw)
{
  mYaw = yaw;
  updateCameraVectors();
}

void AAViewport::shiftCurrentPosition(glm::vec3 offset)
{
  mPosition += offset;
  updateCameraVectors();
}

void AAViewport::shiftYawAndPith(float yawOffset, float pitchOffset)
{
  mYaw += yawOffset;
  mPitch += pitchOffset;
  if (mPitch > 89.9f)
  {
    mPitch = 89.9f;
  }
  else if (mPitch < -89.9f)
  {
    mPitch = -89.9f;
  }
  updateCameraVectors();
}

glm::mat4 AAViewport::getViewMatrix() const
{
  return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glm::mat4 AAViewport::getProjectionMatrix() const
{
  const float screen_width = static_cast<float>(AADisplay::getInstance()->getWindowWidth());
  const float screen_height = static_cast<float>(AADisplay::getInstance()->getWindowHeight());

  if (screen_width == 0 || screen_height == 0)
  {
    std::cout << "Projection setting failed. File: " << __FILE__ << " Line: " << __LINE__ << "  -Screen width or height is 0.\n";
    return glm::mat4(0);
  }

  glm::mat4 projection = glm::mat4(1);

  switch (mRenderProjection)
  {
  case RenderProjection::PERSPECTIVE:
    projection = glm::perspective(glm::radians(mFieldOfView), screen_width / screen_height, 0.01f, mRenderDistance);
    break;
  case RenderProjection::ORTHO:
    projection = glm::ortho(
      mOrthoFieldSize.x,
      mOrthoFieldSize.y,
      mOrthoFieldSize.z,
      mOrthoFieldSize.w,
      .01f,
      mRenderDistance
    );
    break;
  default:
    break;
  }

  return projection;
}

const glm::vec3* AAViewport::getPosition() const noexcept
{
  return &mPosition;
}

const glm::vec3* AAViewport::getFront() const noexcept
{
  return &mFront;
}

const glm::vec3* AAViewport::getRight() const noexcept
{
  return &mRight;
}

float AAViewport::getYaw() const noexcept
{
  return mYaw;
}

float AAViewport::getPitch() const noexcept
{
  return mPitch;
}

float AAViewport::getRenderDistance() const noexcept
{
  return mRenderDistance;
}

bool AAViewport::hasViewportChanged() const noexcept
{
  return mProjectionChanged;
}
