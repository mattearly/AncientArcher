/*
Camera
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
	copyright notice, this list of conditions and the
	following disclaimer.
* Redistributions in binary form must reproduce the above
	copyright notice, this list of conditions and the
	following disclaimer in the documentation and/or other
	materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
	contributors may be used to endorse or promote products
	derived from this software without specific prior
	written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#include "../include/Camera.h"
#include "../include/Display.h"
#include <glm\ext\matrix_clip_space.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include "../include/WorldUp.h"
namespace AA
{
#define UP WorldUp::getWorldUp()

static int uniqueIDs = 0;

Camera::Camera()
{
	mUniqueViewportID = uniqueIDs++;

	mPosition = glm::vec3(0);
	mFieldOfView = 60.f;
	mYaw = -90.f;
	mPitch = 0.f;
	mMaxRenderDistance = 100.f;
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

void Camera::updateCameraVectors()
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
void Camera::resetViewportVars()
{
	mPosition = glm::vec3(0);
	mFieldOfView = 60.f;
	mYaw = 0.f;
	mPitch = 0.f;
	mMaxRenderDistance = 100.f;
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

void Camera::setToPerspective() noexcept
{
	mRenderProjection = RenderProjection::PERSPECTIVE;
}

void Camera::setToOrtho() noexcept
{
	//Camera::getInstance()->setOrthoFieldSize(
	//  glm::vec4(
	//    0, AADisplay::getInstance()->getWindowWidth(),
	//    0, AADisplay::getInstance()->getWindowHeight()
	//  )
	//);

	mRenderProjection = RenderProjection::ORTHO;
}

void Camera::setOrthoFieldSize(float left, float right, float bottom, float top) noexcept
{
	mOrthoFieldSize = glm::vec4(left, right, bottom, top);
}

void Camera::setOrthoFieldSize(glm::vec4 lrbt) noexcept
{
	mOrthoFieldSize = lrbt;
}

void Camera::setMaxRenderDistance(float distance) noexcept
{
	mMaxRenderDistance = distance;
	//updateViewport();
}

void Camera::setCurrentPosition(glm::vec3 pos)
{
	mPosition = pos;
	updateCameraVectors();
}

void Camera::setCurrentPitch(float pitch)
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

void Camera::setCurrentYaw(float yaw)
{
	mYaw = yaw;
	updateCameraVectors();
}

void Camera::shiftCurrentPosition(const glm::vec3& offset)
{
	mPosition += offset;
	updateCameraVectors();
}

void Camera::shiftYawAndPitch(float yawOffset, float pitchOffset)
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

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

// returns a projection matrix based on the window width and height and the perspective.
glm::mat4 Camera::getProjectionMatrix() const
{
	// todo: other ways of adjusting camera viewport
	const float screen_width = static_cast<float>(Display::getInstance()->getWindowWidth());
	const float screen_height = static_cast<float>(Display::getInstance()->getWindowHeight());

	if (screen_width <= 0 || screen_height <= 0)
	{
		std::cout << "Projection setting failed. File: " << __FILE__ << " Line: " << __LINE__ << "  -Screen width or height is 0.\n";
		return glm::mat4(0);
	}

	glm::mat4 projection = glm::mat4(1);

	switch (mRenderProjection)
	{
	case RenderProjection::PERSPECTIVE:
		projection = glm::perspective(glm::radians(mFieldOfView), screen_width / screen_height, 0.01f, mMaxRenderDistance);
		break;
	case RenderProjection::ORTHO:
		// todo: test and fix ortho
		//projection = glm::ortho(
		//  mOrthoFieldSize.x,
		//  mOrthoFieldSize.y,
		//  mOrthoFieldSize.z,
		//  mOrthoFieldSize.w,
		//  .01f,
		//  mMaxRenderDistance
		//);
		break;
	default:
		break;
	}

	return projection;
}

const glm::vec3* Camera::getPosition() const noexcept
{
	return &mPosition;
}

const glm::vec3* Camera::getFront() const noexcept
{
	return &mFront;
}

const glm::vec3* Camera::getRight() const noexcept
{
	return &mRight;
}

float Camera::getYaw() const noexcept
{
	return mYaw;
}

float Camera::getPitch() const noexcept
{
	return mPitch;
}

float Camera::getRenderDistance() const noexcept
{
	return mMaxRenderDistance;
}

const int& Camera::getID() const noexcept
{
	return mUniqueViewportID;
}

const glm::vec3& Camera::getLocation() const noexcept
{
	return mPosition;
}
}  // end namespace AA