#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtx\transform.hpp>
#include "../Scene/WorldSettings.h"
#include "../Scene/Camera.h"
namespace AA
{

extern bool externWindowSizeDirty;

#define UP World->GetUpDir()

static int uniqueCamIDs = 0;

Camera::Camera(int width, int height)
{
	mUniqueViewportID = uniqueCamIDs++;

	resetViewportVars();

	mWidth = width;
	mHeight = height;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front{};
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);
	mRight = glm::normalize(glm::cross(mFront, UP));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}

// same as default constructor but doesn't touch the uniqueId's
void Camera::resetViewportVars()
{
	mPosition = glm::vec3(0.f);
	mPerspectiveFieldOfView = 45.f;
	mYaw = -90.f;
	mPitch = 0.f;
	mMaxRenderDistance = 2500.f;
	//mProjectionChanged = false;
	//mOrthoFieldSize = glm::vec4(-1, 1, -1, 1);
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

void Camera::SetToPerspective() noexcept
{
	mRenderProjection = RenderProjection::PERSPECTIVE;
}

void Camera::SetDimensions(int width, int height) noexcept
{
	if (width < 1 || height < 1)
	{
		return;  // just some janky hack to make sure it doesn't try to divide by 0 later
	}

	mWidth = width;
	mHeight = height;
}

void Camera::__setToOrtho() noexcept
{
	mRenderProjection = RenderProjection::ORTHO;
	externWindowSizeDirty = true;
}

void Camera::SetMaxRenderDistance(float distance) noexcept
{
	mMaxRenderDistance = distance;
}

void Camera::SetCurrentLocation(glm::vec3 pos)
{
	mPosition = pos;
	updateCameraVectors();
}

void Camera::SetCurrentPitch(float pitch)
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

void Camera::SetCurrentYaw(float yaw)
{
	mYaw = yaw;
	updateCameraVectors();
}

void Camera::ShiftCurrentLocation(const glm::vec3& offset)
{
	mPosition += offset;
	updateCameraVectors();
}

void Camera::ShiftYawAndPitch(float yawOffset, float pitchOffset)
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

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	glm::mat4 projection = glm::mat4(1);
	switch (mRenderProjection)
	{
	case RenderProjection::PERSPECTIVE:
	{
		float aspectRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);
		projection = glm::perspective(glm::radians(mPerspectiveFieldOfView), aspectRatio, 0.0167f, mMaxRenderDistance);
	}
	break;
	case RenderProjection::ORTHO:
		projection = glm::ortho(
			0.f,
			static_cast<float>(mWidth),
			0.f,
			static_cast<float>(mHeight),
			0.0167f,
			mMaxRenderDistance
		);	


		break;
	default:
		break;
	}
	return projection;
}

const glm::vec3* Camera::GetFront() const noexcept
{
	return &mFront;
}

const glm::vec3* Camera::GetRight() const noexcept
{
	return &mRight;
}

float Camera::GetYaw() const noexcept
{
	return mYaw;
}

float Camera::GetPitch() const noexcept
{
	return mPitch;
}

float Camera::GetRenderDistance() const noexcept
{
	return mMaxRenderDistance;
}

const int& Camera::GetID() const noexcept
{
	return mUniqueViewportID;
}

const glm::vec3& Camera::GetLocation() const noexcept
{
	return mPosition;
}
}  // end namespace AA