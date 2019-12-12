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

void AAViewport::updateViewMatrix()
{
  mTexShader->use();
  mTexShader->setMat4("view", getViewMatrix());
  mCellShader->use();
  mCellShader->setMat4("view", getViewMatrix());
}

void AAViewport::setToPerspective()
{
  float screen_width = (float)AADisplay::getInstance()->getScreenWidth();
  float screen_height = (float)AADisplay::getInstance()->getScreenHeight();

  if (screen_width == 0 || screen_height == 0)
  {
    std::cout << "perspective setting failed: screen width or height is 0\n";
    return;
  }

  glm::mat4 projection = glm::perspective(
    glm::radians(mFieldOfView),
    screen_width / screen_height,
    0.01f,
    mRenderDistance
  );

  mTexShader->use();
  mTexShader->setMat4("projection", projection);
  mCellShader->use();
  mCellShader->setMat4("projection", projection);

  updateViewMatrix();
}

void AAViewport::setRenderDistance(float distance)
{
  mRenderDistance = distance;

  setToPerspective();

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
  float screen_width = (float)AADisplay::getInstance()->getScreenWidth();
  float screen_height = (float)AADisplay::getInstance()->getScreenHeight();

  if (screen_width == 0 || screen_height == 0)
  {
    std::cout << "perspective setting failed: screen width or height is 0\n";
    return glm::mat4(0);
  }

  glm::mat4 projection = glm::perspective(
    glm::radians(mFieldOfView),
    screen_width / screen_height,
    0.01f,
    mRenderDistance
  );

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
