#include "AAViewport.h"
#include "AADisplay.h"
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>

AAViewport* AAViewport::getInstance()
{
  static AAViewport* viewport = new AAViewport();
  return viewport;
}

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
  shader.stop();
}

void AAViewport::updateProjectionMatrix(const Shader& shader)
{
  shader.use();
  shader.setMat4("projection", getProjectionMatrix());
  shader.stop();
}

void AAViewport::resetViewportVars()
{
  mPosition = glm::vec3(0);
  mFieldOfView = 60.f;
  mYaw = 0.f;
  mPitch = 0.f;
  mFront = glm::vec3(0, 0, -1);
  mRight = glm::vec3(1, 0, 0);
  mUp = glm::vec3(0, 1, 0);
  mRenderDistance = 100.f;
  mRenderProjection = RenderProjection::PERSPECTIVE;
}

void AAViewport::windowSizeChanged()
{
  switch (mRenderProjection)
  {
  case RenderProjection::PERSPECTIVE:

    break;
  case RenderProjection::ORTHO:
    AAViewport::getInstance()->setOrthoFieldSize(
      glm::vec4(
        0, AADisplay::getInstance()->getWindowWidth(),
        0, AADisplay::getInstance()->getWindowHeight()
      )
    );
    break;
  }
}

void AAViewport::setToPerspective()
{
  mRenderProjection = RenderProjection::PERSPECTIVE;
}

void AAViewport::setToOrtho()
{
  AAViewport::getInstance()->setOrthoFieldSize(
    glm::vec4(
      0, AADisplay::getInstance()->getWindowWidth(),
      0, AADisplay::getInstance()->getWindowHeight()
    )
  );

  mRenderProjection = RenderProjection::ORTHO;
}

void AAViewport::setOrthoFieldSize(float left, float right, float bottom, float top)
{
  mOrthoFieldSize = glm::vec4(left, right, bottom, top);
}

void AAViewport::setOrthoFieldSize(glm::vec4 lrbt)
{
  mOrthoFieldSize = lrbt;
}

void AAViewport::setRenderDistance(float distance)
{
  mRenderDistance = distance;
  std::cout << "mRenderDistance changed: check function @" << __FILE__ << " : " << __LINE__ << '\n';

  //setToPerspective(); // need to update projection matrix after a render distance change

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
  float screen_width = (float)AADisplay::getInstance()->getWindowWidth();
  float screen_height = (float)AADisplay::getInstance()->getWindowHeight();

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

const glm::vec3* AAViewport::getPosition() const
{
  return &mPosition;
}

const glm::vec3* AAViewport::getFront() const
{
  return &mFront;
}

const glm::vec3* AAViewport::getRight() const
{
  return &mRight;
}

float AAViewport::getYaw() const
{
  return mYaw;
}

float AAViewport::getPitch() const
{
  return mPitch;
}

float AAViewport::getRenderDistance() const
{
  return mRenderDistance;
}
