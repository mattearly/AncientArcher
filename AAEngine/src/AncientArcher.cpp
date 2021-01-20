/*
AncientArcher
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
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iomanip>
#include <utility>
#include "../include/AncientArcher.h"
#include "../include/Scene/Camera.h"
#include "../include/Window/Display/DisplayCallbacks.h"
#include "../include/Renderer/OpenGL/SceneLoader.h"

namespace AA
{
AncientArcher* AncientArcher::Get()
{
	static AncientArcher* aa = new AncientArcher();
	return aa;
}

int AncientArcher::Run()
{
	begin();
	while (!glfwWindowShouldClose(mWindow)) { deltaUpdate(); render(); }
	teardown();
	return 0;
}

void AncientArcher::Shutdown() noexcept
{
	Display::closeWindow();
}

void AncientArcher::SoftReset()
{
	resetEngine();
}

void AncientArcher::teardown()
{
	for (auto& oTD : onTearDown)
	{
		oTD.second();
	}
	for (const auto& model : mGameObjects)
	{
		SceneLoader::getSceneLoader()->unloadGameObject(model.mMeshes);
	}
}

AncientArcher::AncientArcher()
{
	initEngine();
}

const Camera& AncientArcher::GetCamera(int camId) const
{
	for (const auto& cam : mCameras)
	{
		if (cam.GetID() == camId)
		{
			return cam;
		}
	}

	// if it didn't find it and return above ^^^^^^^^  show error message in console
	//std::cout << "cam ID by the ID [" << camId << "] was not found. unable to get const AACamera\n";

	exit(-1);
}

Camera& AncientArcher::GetCamera(int camId)
{
	for (auto& cam : mCameras)
	{
		if (cam.GetID() == camId)
		{
			return cam;
		}
	}

	// if it didn't find it and return above ^^^^^^^^  show error message in console
	//std::cout << "cam ID by the ID [" << camId << "] was not found. unable to get AACamera\n";
	exit(-1);
}

const OGLShader& AncientArcher::GetShader(int shadId) const
{
	for (const auto& shad : mShaders)
	{
		if (shad.GetID() == shadId)
		{
			return shad;
		}
	}

	// if it didn't find it and return above ^^^^^^^^  show error message in console
	//std::cout << "shad ID by the ID [" << shadId << "] was not found.\n";
	exit(-1);
}

OGLShader& AncientArcher::GetShader(int shadId)
{
	for (auto& shad : mShaders)
	{
		if (shad.GetID() == shadId)
		{
			return shad;
		}
	}

	// if it didn't find it and return above ^^^^^^^^  show error message in console
	//std::cout << "shad ID by the ID [" << shadId << "] was not found.\n";
	exit(-1);
}

const GameObject& AncientArcher::GetGameObject(int objId) const
{
	for (const auto& obj : mGameObjects)
	{
		if (obj.getObjectId() == objId)
		{
			return obj;
		}
	}

	// if it didn't find it and return above ^^^^^^^^  show error message in console
	//std::cout << "game object ID by the ID [" << objId << "] was not found.\n";
	exit(-1);
}

GameObject& AncientArcher::GetGameObject(int objId)
{
	//todo optimize
	for (auto& obj : mGameObjects)
	{
		if (obj.getObjectId() == objId)
		{
			return obj;
		}
	}

	// if it didn't find it and return above ^^^^^^^^  show error message in console
	//std::cout << "game object ID by the ID [" << objId << "] was not found.\n";
	exit(-1);
}

int AncientArcher::AddCamera(int w, int h)
{
	Camera tmpCam(w, h);
	const int return_id = tmpCam.GetID();

	mCameras.push_back(tmpCam);

	return return_id;
}

int AncientArcher::AddShader(const char* vert, const char* frag, const bool isPath)
{
	mShaders.emplace_back(OGLShader(vert, frag, isPath));
	return mShaders.back().GetID();
}

int AncientArcher::AddShader(const SHADERTYPE& type)
{
	mShaders.emplace_back(OGLShader(type));
	return mShaders.back().GetID();
}

int AncientArcher::AddObject(const char* path, int camId, int shadId)
{
	GameObject tmpObject(path, camId, shadId);
	const int return_id = tmpObject.getObjectId();

	mGameObjects.push_back(tmpObject);

	return return_id;
}

int AncientArcher::AddObject(const char* path, int cam_id, int shad_id, const std::vector<InstanceDetails>& details)
{
	// todo: optimize. check if it is an object we already have loaded and use it again if so

	GameObject tmpObject(path, cam_id, shad_id, details);

	const int return_id = tmpObject.getObjectId();

	mGameObjects.push_back(tmpObject);

	return return_id;
}

void AncientArcher::SetSkybox(const std::shared_ptr<Skybox>& skybox) noexcept
{
	mSkybox = skybox;
}

uint32_t AncientArcher::AddToOnBegin(void(*function)())
{
	static uint32_t next_begin_id = 0;
	next_begin_id++;
	onBegin.emplace(next_begin_id, function);
	return next_begin_id;
}

uint32_t AncientArcher::AddToDeltaUpdate(void(*function)(float))
{
	static uint32_t next_deltaupdate_id = 0;
	next_deltaupdate_id++;
	onDeltaUpdate.emplace(next_deltaupdate_id, function);
	return next_deltaupdate_id;
}

uint32_t AncientArcher::AddToUpdate(void(*function)())
{
	static uint32_t next_update_id = 0;
	next_update_id++;
	onUpdate.emplace(next_update_id, function);
	return next_update_id;
}

uint32_t AncientArcher::AddToSlowUpdate(void(*function)())
{
	static uint32_t next_slowupdate_id = 0;
	next_slowupdate_id++;
	onSlowUpdate.emplace(next_slowupdate_id, function);
	return next_slowupdate_id;
}

uint32_t AncientArcher::AddToTimedOutKeyHandling(bool(*function)(KeyboardInput&))
{
	static uint32_t next_timedout_id = 0;
	next_timedout_id++;
	onTimeoutKeyHandling.emplace(next_timedout_id, function);
	return next_timedout_id;
}

uint32_t AncientArcher::AddToScrollHandling(void(*function)(ScrollInput&))
{
	static uint32_t next_scrollhandling_id = 0;
	next_scrollhandling_id++;
	onScrollHandling.emplace(next_scrollhandling_id, function);
	return next_scrollhandling_id;
}

uint32_t AncientArcher::AddToKeyHandling(void(*function)(KeyboardInput&))
{
	static uint32_t next_keyhandling_id = 0;
	next_keyhandling_id++;
	onKeyHandling.emplace(next_keyhandling_id, function);
	return next_keyhandling_id;
}

uint32_t AncientArcher::AddToMouseHandling(void(*function)(MouseInput&))
{
	static uint32_t next_mousehandling_id = 0;
	next_mousehandling_id++;
	onMouseHandling.emplace(next_mousehandling_id, function);
	return next_mousehandling_id;
}

uint32_t AncientArcher::AddToOnTeardown(void(*function)())
{
	static uint32_t next_teardown_id = 0;
	next_teardown_id++;
	onTearDown.emplace(next_teardown_id, function);
	return next_teardown_id;
}

bool AncientArcher::RemoveFromOnBegin(uint32_t r_id)
{
	return static_cast<bool>(onBegin.erase(r_id));
}

bool AncientArcher::RemoveFromDeltaUpdate(uint32_t r_id)
{
	return static_cast<bool>(onDeltaUpdate.erase(r_id));
}

bool AncientArcher::RemoveFromUpdate(uint32_t r_id)
{
	return static_cast<bool>(onUpdate.erase(r_id));
}

bool AncientArcher::RemoveFromSlowUpdate(uint32_t r_id)
{
	return static_cast<bool>(onSlowUpdate.erase(r_id));
}

bool AncientArcher::RemoveFromTimedOutKeyHandling(uint32_t r_id)
{
	return static_cast<bool>(onTimeoutKeyHandling.erase(r_id));
}

bool AncientArcher::RemoveFromScrollHandling(uint32_t r_id)
{
	return static_cast<bool>(onScrollHandling.erase(r_id));
}

bool AncientArcher::RemoveFromKeyHandling(uint32_t r_id)
{
	return static_cast<bool>(onKeyHandling.erase(r_id));
}

bool AncientArcher::RemoveFromMouseHandling(uint32_t r_id)
{
	return static_cast<bool>(onMouseHandling.erase(r_id));
}

bool AncientArcher::RemoveFromTeardown(uint32_t r_id)
{
	return static_cast<bool>(onTearDown.erase(r_id));
}

void AncientArcher::SetCursorToEnabled(bool isHardwareRendered)
{
	if (isHardwareRendered)
	{
		SetCursorToHidden();
	}
	else
	{
		SetCursorToVisible();
	}
}

//void AncientArcher::SetCursorToDisabled()
//{
//	SetCursorToDisabled();
//}
//void AncientArcher::SetToPerspectiveMouseHandling()
//{
//	SetCurorReadToFPPCalc();
//}
//void AncientArcher::SetToStandardMouseHandling()
//{
//	SetCurorReadToStandardCalc();
//}
//void AncientArcher::SetWindowTitle(const char* title)
//{
//	SetWindowTitle(title);
//}
//void AncientArcher::SetWindowSize(const char to, int width, int height, bool center)
//{
//	SetWindowSize(to);
//	switch (to)
//	{
//	case 'f':
//	case 'm':
//	case 'b':
//		return;
//	default:
//		break;
//	}
//
//	assert(width > 0);
//	assert(height > 0);
//
//	if (center)
//	{
//		SetWindowSize(width, height, center);
//	}
//	else
//	{
//		SetWindowSize(width, height, center);
//	}
//}


void AncientArcher::SetMaxRenderDistance(int camId, float amt) noexcept
{
	for (auto& cam : mCameras)
	{
		if (cam.GetID() == camId)
		{
			cam.SetMaxRenderDistance(amt);
			return;
		}
	}

	// if code gets here there is an error, camId not found
	//std::cout << "cam not found for id [" << camId << "].\n";
}

void AncientArcher::SetProjectionMatrix(int shadId, int camId)
{
	GetShader(shadId).use();
	GetShader(shadId).setMat4("projection", GetCamera(camId).GetProjectionMatrix());
}

void AncientArcher::SetSlowUpdateTimeoutLength(const float& newtime)
{
	// !! warning, no checking, set at your own risk
	mSlowUpdateWaitLength = newtime;
}

// -- PRIVATE FUNCTIONS --

void AncientArcher::begin()
{
	keepWindowOpen();

	for (const auto& oB : onBegin)
	{
		oB.second();
	}

	__setProjectionMatToAllShadersFromFirstCam_hack();
}

void AncientArcher::deltaUpdate()
{
	pullButtonStateEvents();

	// update engine run delta times
	mCurrentFrameTime = static_cast<float>(glfwGetTime());
	mDeltaTime = mCurrentFrameTime - mLastFrameTime;
	mLastFrameTime = mCurrentFrameTime;

	// go through all updates that need access to delta time
	for (auto& oDU : onDeltaUpdate)
	{
		oDU.second(mDeltaTime);
	}

	// process keyboard input
	for (auto& oKH : onKeyHandling) { oKH.second(mButtonState); }

	// absorb scroll wheel
	for (auto& oSH : onScrollHandling) { oSH.second(mMouseWheelScroll); }

	// reset scroll wheel to 0's after processing scroll wheel
	mMouseWheelScroll.xOffset = 0;
	mMouseWheelScroll.yOffset = 0;

	// handle mouse position
	for (auto& oMH : onMouseHandling) { oMH.second(mMousePosition); }

	// Snap cursor to the middle of the screen if it is in perspective and cursor is disabled (FPP mode)
	if (GetMouseReportingMode() == MouseReporting::PERSPECTIVE && GetCursorMode() == GLFW_CURSOR_DISABLED)
	{
		//deb(mMousePosition.xOffset);
		//deb(mMousePosition.yOffset);

		mMousePosition.xOffset = 0;
		mMousePosition.yOffset = 0;
	}
	//else if (getMouseReportingMode() == MouseReporting::STANDARD && getCursorMode() == GLFW_CURSOR_NORMAL) // standard and normal
	//{
		// dont snap the position
		//showmousepos(PointerXOffset, PointerYOffset);
		//std::cout << "X: " << PointerXOffset << " Y: " << PointerYOffset << '\n';
	//}

	// run through user prefered updates
	for (auto& oU : onUpdate) { oU.second(); }

	// delayed updates for things you don't want spammed.
	// update accum time for delayed updates
	mSlowUpdateTimeout += mDeltaTime;
	// check to see if its time to process delayed updates
	if (mSlowUpdateTimeout > mSlowUpdateWaitLength)
	{
		// process all delayed updates
		for (auto& oSU : onSlowUpdate)
		{
			oSU.second();
		}
		// we should also process whether the window size changed here
		if (mWindowSizeDirty)
		{
			// notify shader/cams
			__setProjectionMatToAllShadersFromFirstCam_hack();

			// don't process again
			mWindowSizeDirty = false;
		}
		// reset timeout length to 0
		mSlowUpdateTimeout = 0.f;
	}

	// needs updated, we'll use it in update with keyboard functions before the AncientArcher is done.
	// note that the keyboard processing cant be here because we have yet to run processSystemKeys()
	mNonSpammableKeysTimeout += mDeltaTime;

	// only be executable after a timeout has been met, sort of like a cooldown
	if (mNonSpammableKeysTimeout > mNoSpamWaitLength)
	{
		// process unspammable keys
		for (auto& oTOK : onTimeoutKeyHandling)
		{
			// if we get a true we stop processing
			if (oTOK.second(mButtonState)) {
				//std::cout << "timeout key press detected. reseting timeoutkeytimer\n";
				mNonSpammableKeysTimeout = 0.f;
				break;
			}
		}
	}
}

void AncientArcher::render()
{
	clearBackBuffer();

	for (auto& obj : mGameObjects)
	{
		// get the id of the shader for this object
		const int shaderID = obj.getShaderId();

		// switch to the shader
		GetShader(shaderID).use();

		// get the camera id for this object
		const int cameraID = obj.getCameraId();

		// set the view matrix from the cam for this object
		GetShader(shaderID).setMat4("view", GetCamera(cameraID).GetViewMatrix());

		// draw using the shader for it
		obj.draw(GetShader(shaderID));
	}

	// draw skybox if one was specified
	if (mSkybox) { mSkybox->render(mCameras.front()); }

	swapWindowBuffers();
}

void AncientArcher::initEngine()
{
	resetEngine();

	initDisplayFromEngine();

	setReshapeWindowHandler();

	SetCurorReadToStandardCalc();

	setScrollWheelHandler();

	// Init OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context) -- must be after initGet()
	{
		//std::cout << "failed to init glad @ file " __FILE__ << " line " << __LINE__ << '\n';
		exit(-1);
	}
}

void AncientArcher::resetEngine() noexcept
{
	// process anything the user requested and unload all meshes and textures
	teardown();

	//clear all vectors
	mCameras.clear();
	mShaders.clear();
	mGameObjects.clear();
	onBegin.clear();
	onDeltaUpdate.clear();
	onKeyHandling.clear();
	onTimeoutKeyHandling.clear();
	onScrollHandling.clear();
	onMouseHandling.clear();
	onUpdate.clear();
	onSlowUpdate.clear();
	onTearDown.clear();
	// reset all state data
	mNonSpammableKeysTimeout = 0.f;
	mSlowUpdateTimeout = 0.f;
	mNoSpamWaitLength = .5667f;
	//mSlowUpdateWaitLength = .5667f;
	mSlowUpdateWaitLength = .3337f;

	mLastFrameTime = 0.f;
	mCurrentFrameTime = 0.f;
	mDeltaTime = 0.f;

}

void AncientArcher::__setProjectionMatToAllShadersFromFirstCam_hack()
{
	// set proj matries hack from first cam
	for (auto& shad : mShaders)
	{
		//shad.setMat4("projection", mCameras.front().GetProjectionMatrix());
		const int sID = shad.GetID();
		//std::cout << "shad id: " << sID << '\n';

		const int cID = mCameras.front().GetID();
		//std::cout << "shad id: " << sID << '\n';

		SetProjectionMatrix(sID, cID);  //set shader (sID) from cam (cID)
	}

	// if there is a skybox
	if (mSkybox)
	{
		// if there is a camera
		if (mCameras.size() > 0) {
			// set the projection matrix on the skybox from the first cam proj matrix
			mSkybox->setProjectionMatrix(mCameras.front());
		}
	}
}

///////////  CALLBACKS ////////////////

//---------------------------------------------------------------------------------------
// RESHAPE WINDOW CALLBACK SETUP
void AncientArcher::ReshapeWindowHandler(GLFWwindow* window, int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
	glViewport(0, 0, mWindowWidth, mWindowHeight);
	mWindowSizeDirty = true;
//#ifdef _DEBUG
//	std::cout << "window reshape called\n";
//#endif
}

void AncientArcher::setReshapeWindowHandler() noexcept
{
	::glfwSetFramebufferSizeCallback(mWindow, DisplayCallbacks::reshapeCallback);
}

//---------------------------------------------------------------------------------------
// PERSPECTIVE MOUSE REPORTING CALLBACK SETUP
void AncientArcher::PerspectiveMouseHandler(GLFWwindow* window, float xpos, float ypos)
{
	perspectiveMouseMovement(xpos, ypos);
}


void AncientArcher::SetCurorReadToFPPCalc() noexcept
{
	mRenewFPP = true;
	::glfwSetCursorPosCallback(mWindow, DisplayCallbacks::perspectiveMouseCallback);
	mMouseReporting = MouseReporting::PERSPECTIVE;
}

//---------------------------------------------------------------------------------------
// STANDARD MOUSE REPORTING CALLBACK SETUP
void AncientArcher::StandardMouseHandler(GLFWwindow* window, float xpos, float ypos)
{
	standardMouseMovement(xpos, ypos);
}

void AncientArcher::SetCurorReadToStandardCalc() noexcept
{
	::glfwSetCursorPosCallback(mWindow, DisplayCallbacks::standardMouseCallback);
	mMouseReporting = MouseReporting::STANDARD;
}
/**
* Reports the mouse in x y space on the screen: bottom left should be 0,0
*/
void AncientArcher::standardMouseMovement(float xpos, float ypos)
{
	switch (mStandardMouseZeros)
	{
	case STANDARDMOUSEZEROS::BOT_LEFT_0_to_1:
	{
		const float c_xpos = xpos / mWindowWidth;
		const float c_ypos = -(ypos - mWindowHeight) / mWindowHeight;
		mMousePosition.xOffset = c_xpos;
		mMousePosition.yOffset = c_ypos;
	}
	break;
	case STANDARDMOUSEZEROS::TOP_LEFT_0_to_1:
	{
		const float c_xpos = xpos / mWindowWidth;
		const float c_ypos = ypos / mWindowHeight;
		mMousePosition.xOffset = c_xpos;
		mMousePosition.yOffset = c_ypos;
	}
	break;
	case STANDARDMOUSEZEROS::BOT_LEFT_FULL_RANGE:
	{
		const float c_xpos = xpos;
		const float c_ypos = -(ypos - mWindowHeight);
		mMousePosition.xOffset = c_xpos;
		mMousePosition.yOffset = c_ypos;
	}
	break;
	case STANDARDMOUSEZEROS::TOP_LEFT_FULL_RANGE:
	{
		const float c_xpos = xpos;
		const float c_ypos = ypos;
		mMousePosition.xOffset = c_xpos;
		mMousePosition.yOffset = c_ypos;
	}
	break;
	//default:
		//std::cout << "case not handled in standard mouse zeros\n";
	}
}

//---------------------------------------------------------------------------------------
// MOUSE SCROLL REPORTING CALLBACK SETUP
void AncientArcher::ScrollHandler(GLFWwindow* window, float xpos, float ypos)
{
	mouseScrollWheelMovement(xpos, ypos);
}

void AncientArcher::setScrollWheelHandler() noexcept
{
	::glfwSetScrollCallback(mWindow, DisplayCallbacks::scrollCallback);
}


}  // end namespace AA