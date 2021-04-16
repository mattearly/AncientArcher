#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtx\transform.hpp>
#include "../Scene/WorldSettings.h"
#include "../Scene/Camera.h"
namespace AA {

#define UP World->GetUpDir()

Camera::Camera(int width, int height) {
  resetViewportVars();
  Width = width;
  Height = height;
  updateProjectionMatrix();
  updateViewMatrix();
}

void Camera::updateCameraVectors() {
  glm::vec3 front{};
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, UP));
  Up = glm::normalize(glm::cross(Right, Front));

  updateViewMatrix();  // every time?
}

// same as default constructor but doesn't touch the uniqueId's
void Camera::resetViewportVars() {
  TopLeftPositionOnScreen = glm::vec2(0.f);
  RatioToScreen = glm::vec2(1);
  Position = glm::vec3(0.f);
  FOV = 45.f;
  Yaw = -90.f;
  Pitch = 0.f;
  MaxRenderDistance = 2500.f;
  RenderProjection = RenderProjection::PERSPECTIVE;
  Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front.y = sin(glm::radians(Pitch));
  Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Right = glm::cross(Front, UP);
  Up = glm::cross(Right, Front);
  Front = glm::normalize(Front);
  Right = glm::normalize(Right);
  Up = glm::normalize(Up);
}

void Camera::updateProjectionMatrix() {
  switch (RenderProjection) {
  case RenderProjection::PERSPECTIVE:
  {
    float aspectRatio = static_cast<float>(Width) / static_cast<float>(Height);
    Projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.0167f, MaxRenderDistance);
  }
  break;
  case RenderProjection::ORTHO:
    Projection = glm::ortho(
      0.f,
      static_cast<float>(Width),
      0.f,
      static_cast<float>(Height),
      0.0167f,
      MaxRenderDistance
    );
    break;
  default:
    break;
  }
}

void Camera::updateViewMatrix() {
  View = glm::lookAt(Position, Position + Front, Up);
}

//void Camera::__setToOrtho() noexcept
//{
//	mRenderProjection = RenderProjection::ORTHO;
//	externWindowSizeDirty = true;
//}
//glm::mat4 Camera::GetViewMatrix() const
//{
//	return glm::lookAt(mPosition, mPosition + mFront, mUp);
//}
//
//glm::mat4 Camera::GetProjectionMatrix() const
//{
//	glm::mat4 projection = glm::mat4(1);
//	switch (mRenderProjection)
//	{
//	case RenderProjection::PERSPECTIVE:
//	{
//		float aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
//		projection = glm::perspective(glm::radians(mPerspectiveFieldOfView), aspectRatio, 0.0167f, mMaxRenderDistance);
//	}
//	break;
//	case RenderProjection::ORTHO:
//		projection = glm::ortho(
//			0.f,
//			static_cast<float>(mWidth),
//			0.f,
//			static_cast<float>(mHeight),
//			0.0167f,
//			mMaxRenderDistance
//		);	
//
//
//		break;
//	default:
//		break;
//	}
//	return projection;
//}
//float Camera::GetYaw() const noexcept
//{
//	return mYaw;
//}
//
//float Camera::GetPitch() const noexcept
//{
//	return mPitch;
//}
//
//float Camera::GetRenderDistance() const noexcept
//{
//	return mMaxRenderDistance;
//}
//
//const int& Camera::GetID() const noexcept
//{
//	return mUniqueViewportID;
//}
//
//const glm::vec3& Camera::GetLocation() const noexcept
//{
//	return mPosition;
//}
}  // end namespace AA