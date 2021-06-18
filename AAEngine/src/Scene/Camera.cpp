#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtx\transform.hpp>
#include "WorldSettings.h"
#include "Camera.h"
#include "../../include/AncientArcher/AncientArcher.h"
namespace AA {

#define UP World->GetUpDir()

Camera::Camera() {
  resetViewportVars();
  Width = GetWindowWidth();
  Height = GetWindowHeight();
  updateProjectionMatrix();
  updateViewMatrix();
}

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
  Position = glm::vec3(0.f);
  FOV = 45.f;
  Yaw = -90.f;  // look down the -z axis
  Pitch = 0.f;
  MaxRenderDistance = 2500.f;
  mProjectionType = ProjectionType::PERSPECTIVE;
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
  switch (mProjectionType) {
  case ProjectionType::PERSPECTIVE:
  {
    float aspectRatio = static_cast<float>(Width) / static_cast<float>(Height);
    mProjectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, 0.0167f, MaxRenderDistance);
  }
  break;
  case ProjectionType::ORTHO:
  {
    // we are going to use an aspect corrected view
    float aspect_ratio = (float)Width * Height;
    float ortho_height = (float)Height / 2.f;   // or ortho size
    float ortho_width = Width * ortho_height;
    mProjectionMatrix = glm::ortho(
      -ortho_width,
      ortho_width,
      -ortho_height,
      ortho_height,
      0.0167f,
      MaxRenderDistance
    );
  }
    break;
  default:
    break;
  }
}

void Camera::updateViewMatrix() {
  mViewMatrix = glm::lookAt(Position, Position + Front, Up);
}

void Camera::changeProjection(ProjectionType choice) {
  mProjectionType = choice;
}

//void Camera::__setToOrtho() noexcept
//{
//	mRenderProjection = ProjectionType::ORTHO;
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
//	case ProjectionType::PERSPECTIVE:
//	{
//		float aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
//		projection = glm::perspective(glm::radians(mPerspectiveFieldOfView), aspectRatio, 0.0167f, mMaxRenderDistance);
//	}
//	break;
//	case ProjectionType::ORTHO:
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