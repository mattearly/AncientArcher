#include "AACamera.h"
#include "FPPWorldLocation.h"
#include "../winSys/AADisplay.h"
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>

#define UP mearly::FPPWorldLocation::getWorldUp()

static int uniqueIDs = 0;

AACamera::AACamera()
{
  mUniqueViewportID = uniqueIDs++;

  mPosition = glm::vec3(0);
  mFieldOfView = 60.f;
  mYaw = 0.f;
  mPitch = 0.f;
  mRenderDistance = 100.f;
  mProjectionChanged = false;
  mOrthoFieldSize = glm::vec4(-1, 1, -1, 1);
  mRenderProjection = RenderProjection::PERSPECTIVE;

  mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  mFront.y = sin(glm::radians(mPitch));
  mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

  mRight = glm::cross(mFront, UP);
  mUp = glm::cross(mRight, mFront);

  mFront = glm::normalize(mFront);
  mRight = glm::normalize(mRight);
  mUp = glm::normalize(mUp);

  //updateCameraVectors();
}

void AACamera::updateCameraVectors()
{
  glm::vec3 front;
  front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  front.y = sin(glm::radians(mPitch));
  front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  mFront = glm::normalize(front);
  mRight = glm::normalize(glm::cross(mFront, UP));
  mUp = glm::normalize(glm::cross(mRight, mFront));
}

// same as default constructor but doesn't touch the uniqueId's
void AACamera::resetViewportVars()
{
  mPosition = glm::vec3(0);
  mFieldOfView = 60.f;
  mYaw = 0.f;
  mPitch = 0.f;
  mRenderDistance = 100.f;
  mProjectionChanged = false;
  mOrthoFieldSize = glm::vec4(-1, 1, -1, 1);
  mRenderProjection = RenderProjection::PERSPECTIVE;

  mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
  mFront.y = sin(glm::radians(mPitch));
  mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

  mRight = glm::cross(mFront, UP);
  mUp = glm::cross(mRight, mFront);

  mFront = glm::normalize(mFront);
  mRight = glm::normalize(mRight);
  mUp = glm::normalize(mUp);
}

void AACamera::setToPerspective() noexcept
{
  mRenderProjection = RenderProjection::PERSPECTIVE;
}

void AACamera::setToOrtho() noexcept
{
  //AACamera::getInstance()->setOrthoFieldSize(
  //  glm::vec4(
  //    0, AADisplay::getInstance()->getWindowWidth(),
  //    0, AADisplay::getInstance()->getWindowHeight()
  //  )
  //);

  mRenderProjection = RenderProjection::ORTHO;
}

void AACamera::setOrthoFieldSize(float left, float right, float bottom, float top) noexcept
{
  mOrthoFieldSize = glm::vec4(left, right, bottom, top);
}

void AACamera::setOrthoFieldSize(glm::vec4 lrbt) noexcept
{
  mOrthoFieldSize = lrbt;
}

void AACamera::setRenderDistance(float distance) noexcept
{
  mRenderDistance = distance;
  //updateViewport();
}

void AACamera::setCurrentPosition(glm::vec3 pos)
{
  mPosition = pos;
  updateCameraVectors();
}

void AACamera::setCurrentPitch(float pitch)
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

void AACamera::setCurrentYaw(float yaw)
{
  mYaw = yaw;
  updateCameraVectors();
}

void AACamera::shiftCurrentPosition(const glm::vec3& offset)
{
  mPosition += offset;
  updateCameraVectors();
}

void AACamera::shiftYawAndPith(float yawOffset, float pitchOffset)
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

glm::mat4 AACamera::getViewMatrix() const
{
  return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

// returns a projection matrix based on the window width and height and the perspective.
glm::mat4 AACamera::getProjectionMatrix() const  
{
  // todo: other ways of adjusting camera viewport
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
    // todo: test and fix ortho
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

const glm::vec3* AACamera::getPosition() const noexcept
{
  return &mPosition;
}

const glm::vec3* AACamera::getFront() const noexcept
{
  return &mFront;
}

const glm::vec3* AACamera::getRight() const noexcept
{
  return &mRight;
}

float AACamera::getYaw() const noexcept
{
  return mYaw;
}

float AACamera::getPitch() const noexcept
{
  return mPitch;
}

float AACamera::getRenderDistance() const noexcept
{
  return mRenderDistance;
}

const int& AACamera::getID() const noexcept
{
  return mUniqueViewportID;
}

const glm::vec3& AACamera::getLocation() const noexcept
{
  return mPosition;
}
