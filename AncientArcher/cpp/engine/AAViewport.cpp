#include "AAViewport.h"
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

glm::vec3 AAViewport::getFront()
{
  return mFront;
}
