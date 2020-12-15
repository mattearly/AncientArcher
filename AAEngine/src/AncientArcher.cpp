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
#include <iostream>
#include <utility>
#include "../include/AncientArcher.h"
#include "../include/Controls/Controls.h"
#include "../include/Scene/Camera.h"
namespace AA
{
AncientArcher* AncientArcher::Get()
{
	static AncientArcher* aa = new AncientArcher();
	return aa;
}

int AncientArcher::Run()
{
	begin();	while (!glfwWindowShouldClose(mWindow)) { deltaUpdate();		render(); }	teardown();	return 0;
}

void AncientArcher::teardown()
{
	for (auto& oTD : onTearDown)
	{
		oTD.second();
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
		if (cam.getID() == camId)
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
		if (cam.getID() == camId)
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
		if (shad.getID() == shadId)
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
		if (shad.getID() == shadId)
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
	const int return_id = tmpCam.getID();

	mCameras.push_back(tmpCam);

	return return_id;
}

int AncientArcher::AddShader(const char* vert, const char* frag, const bool isPath)
{
	mShaders.emplace_back(OGLShader(vert, frag, isPath));
	return mShaders.back().getID();
}

int AncientArcher::AddShader(const SHADERTYPE& type)
{
	mShaders.emplace_back(OGLShader(type));
	return mShaders.back().getID();
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
//	SetCurorPosToPerspectiveCalc();
//}
//void AncientArcher::SetToStandardMouseHandling()
//{
//	SetCurorPosToStandardCalc();
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

void AncientArcher::SetWindowPos(int xpos, int ypos)
{
	SetWindowSize(0, 0, xpos, ypos);
}

void AncientArcher::SetMaxRenderDistance(int camId, float amt) noexcept
{
	for (auto& cam : mCameras)
	{
		if (cam.getID() == camId)
		{
			cam.setMaxRenderDistance(amt);
			return;
		}
	}

	// if code gets here there is an error, camId not found
	//std::cout << "cam not found for id [" << camId << "].\n";
}

void AncientArcher::SetProjectionMatrix(int shadId, int camId)
{
	GetShader(shadId).use();
	GetShader(shadId).setMat4("projection", GetCamera(camId).getProjectionMatrix());
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

	SetProjectionMatToAllShadersFromFirstCam_hack();
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
		if (mWindowSizeChanged)
		{
			// notify shader/cams
			SetProjectionMatToAllShadersFromFirstCam_hack();

			// don't process again
			mWindowSizeChanged = false;
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
		GetShader(shaderID).setMat4("view", GetCamera(cameraID).getViewMatrix());

		// draw using the shader for it
		obj.draw(GetShader(shaderID));
	}

	// draw skybox if one was specified
	if (mSkybox) { mSkybox->render(mCameras.front()); }

	swapWindowBuffers();
}

void AncientArcher::initEngine()
{
	ResetEngine();

	initFromEngine();

	SetReshapeWindowHandler();

	SetCurorPosToStandardCalc();

	SetScrollWheelHandler();

	// Init OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context) -- must be after initGet()
	{
		//std::cout << "failed to init glad @ file " __FILE__ << " line " << __LINE__ << '\n';
		exit(-1);
	}
}

void AncientArcher::ResetEngine() noexcept
{
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

void AncientArcher::SetProjectionMatToAllShadersFromFirstCam_hack()
{
	// set proj matries hack from first cam
	for (auto& shad : mShaders)
	{
		//shad.setMat4("projection", mCameras.front().getProjectionMatrix());
		const int sID = shad.getID();
		//std::cout << "shad id: " << sID << '\n';

		const int cID = mCameras.front().getID();
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
void AncientArcher::reshapeWindowHandler(GLFWwindow* window, int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
	glViewport(0, 0, mWindowWidth, mWindowHeight);
	mWindowSizeChanged = true;
#ifdef _DEBUG
	std::cout << "window reshape called\n";
#endif
}
extern "C" void reshapeCallback(GLFWwindow * window, int w, int h)
{
	Engine->reshapeWindowHandler(window, w, h);
}
void AncientArcher::SetReshapeWindowHandler() noexcept
{
	::glfwSetFramebufferSizeCallback(mWindow, reshapeCallback);
}

//---------------------------------------------------------------------------------------
// PERSPECTIVE MOUSE REPORTING CALLBACK SETUP
void AncientArcher::perspectiveMouseHandler(GLFWwindow* window, float xpos, float ypos)
{
	perspectiveMouseMovement(xpos, ypos);
}
extern "C" void perspectiveMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	Engine->perspectiveMouseHandler(window, (float)xpos, (float)ypos);
}
void AncientArcher::SetCurorPosToPerspectiveCalc() noexcept
{
	mRenewFPP = true;
	::glfwSetCursorPosCallback(mWindow, perspectiveMouseCallback);
	mMouseReporting = MouseReporting::PERSPECTIVE;
}

//---------------------------------------------------------------------------------------
// STANDARD MOUSE REPORTING CALLBACK SETUP
void AncientArcher::standardMouseHandler(GLFWwindow* window, float xpos, float ypos)
{
	standardMouseMovement(xpos, ypos);
}
extern "C" void standardMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	Engine->standardMouseHandler(window, (float)xpos, (float)ypos);
}
void AncientArcher::SetCurorPosToStandardCalc() noexcept
{
	::glfwSetCursorPosCallback(mWindow, standardMouseCallback);
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
	default:
		std::cout << "case not handled in standard mouse zeros\n";
	}
}

//---------------------------------------------------------------------------------------
// MOUSE SCROLL REPROTING CALLBACK SETUP
void AncientArcher::scrollHandler(GLFWwindow* window, float xpos, float ypos)
{
	mouseScrollWheelMovement(xpos, ypos);
}
extern "C" void scrollCallback(GLFWwindow * window, double xpos, double ypos)
{
	Engine->scrollHandler(window, (float)xpos, (float)ypos);
}
void AncientArcher::SetScrollWheelHandler() noexcept
{
	::glfwSetScrollCallback(mWindow, scrollCallback);
}

/////// PULL CONTROLS BUTTON STATE FROM DISPLAY ////////

void AncientArcher::pullButtonStateEvents()
{
	glfwPollEvents();

	// esc
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		mButtonState.esc = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		mButtonState.esc = false;
	}
	// function keys
	if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_PRESS)
	{
		mButtonState.f1 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_RELEASE)
	{
		mButtonState.f1 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F2) == GLFW_PRESS)
	{
		mButtonState.f2 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F2) == GLFW_RELEASE)
	{
		mButtonState.f2 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F3) == GLFW_PRESS)
	{
		mButtonState.f3 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F3) == GLFW_RELEASE)
	{
		mButtonState.f3 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F4) == GLFW_PRESS)
	{
		mButtonState.f4 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F4) == GLFW_RELEASE)
	{
		mButtonState.f4 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F5) == GLFW_PRESS)
	{
		mButtonState.f5 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F5) == GLFW_RELEASE)
	{
		mButtonState.f5 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F6) == GLFW_PRESS)
	{
		mButtonState.f6 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F6) == GLFW_RELEASE)
	{
		mButtonState.f6 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F7) == GLFW_PRESS)
	{
		mButtonState.f7 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F7) == GLFW_RELEASE)
	{
		mButtonState.f7 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F8) == GLFW_PRESS)
	{
		mButtonState.f8 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F8) == GLFW_RELEASE)
	{
		mButtonState.f8 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F9) == GLFW_PRESS)
	{
		mButtonState.f9 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F9) == GLFW_RELEASE)
	{
		mButtonState.f9 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F10) == GLFW_PRESS)
	{
		mButtonState.f10 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F10) == GLFW_RELEASE)
	{
		mButtonState.f10 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F11) == GLFW_PRESS)
	{
		mButtonState.f11 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F11) == GLFW_RELEASE)
	{
		mButtonState.f11 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F12) == GLFW_PRESS)
	{
		mButtonState.f12 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F12) == GLFW_RELEASE)
	{
		mButtonState.f12 = false;
	}
	// number key row
	if (glfwGetKey(mWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
	{
		mButtonState.graveAccent = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
	{
		mButtonState.graveAccent = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_PRESS)
	{
		mButtonState.n1 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_RELEASE)
	{
		mButtonState.n1 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_PRESS)
	{
		mButtonState.n2 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_RELEASE)
	{
		mButtonState.n2 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_PRESS)
	{
		mButtonState.n3 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_RELEASE)
	{
		mButtonState.n3 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_4) == GLFW_PRESS)
	{
		mButtonState.n4 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_4) == GLFW_RELEASE)
	{
		mButtonState.n4 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_5) == GLFW_PRESS)
	{
		mButtonState.n5 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_5) == GLFW_RELEASE)
	{
		mButtonState.n5 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_6) == GLFW_PRESS)
	{
		mButtonState.n6 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_6) == GLFW_RELEASE)
	{
		mButtonState.n6 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_7) == GLFW_PRESS)
	{
		mButtonState.n7 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_7) == GLFW_RELEASE)
	{
		mButtonState.n7 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_8) == GLFW_PRESS)
	{
		mButtonState.n8 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_8) == GLFW_RELEASE)
	{
		mButtonState.n8 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_9) == GLFW_PRESS)
	{
		mButtonState.n9 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_9) == GLFW_RELEASE)
	{
		mButtonState.n9 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_0) == GLFW_PRESS)
	{
		mButtonState.n0 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_0) == GLFW_RELEASE)
	{
		mButtonState.n0 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		mButtonState.minus = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_MINUS) == GLFW_RELEASE)
	{
		mButtonState.minus = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_EQUAL) == GLFW_PRESS)
	{
		mButtonState.equal = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_EQUAL) == GLFW_RELEASE)
	{
		mButtonState.equal = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
	{
		mButtonState.backspace = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_BACKSPACE) == GLFW_RELEASE)
	{
		mButtonState.backspace = false;
	}
	// alphabet keys
	if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		mButtonState.a = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_RELEASE)
	{
		mButtonState.a = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_B) == GLFW_PRESS)
	{
		mButtonState.b = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_B) == GLFW_RELEASE)
	{
		mButtonState.b = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_PRESS)
	{
		mButtonState.c = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_RELEASE)
	{
		mButtonState.c = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		mButtonState.d = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_RELEASE)
	{
		mButtonState.d = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		mButtonState.e = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_RELEASE)
	{
		mButtonState.e = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS)
	{
		mButtonState.f = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_RELEASE)
	{
		mButtonState.f = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_G) == GLFW_PRESS)
	{
		mButtonState.g = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_G) == GLFW_RELEASE)
	{
		mButtonState.g = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_H) == GLFW_PRESS)
	{
		mButtonState.h = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_H) == GLFW_RELEASE)
	{
		mButtonState.h = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_I) == GLFW_PRESS)
	{
		mButtonState.i = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_I) == GLFW_RELEASE)
	{
		mButtonState.i = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_J) == GLFW_PRESS)
	{
		mButtonState.j = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_J) == GLFW_RELEASE)
	{
		mButtonState.j = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_K) == GLFW_PRESS)
	{
		mButtonState.k = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_K) == GLFW_RELEASE)
	{
		mButtonState.k = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_L) == GLFW_PRESS)
	{
		mButtonState.l = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_L) == GLFW_RELEASE)
	{
		mButtonState.l = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_M) == GLFW_PRESS)
	{
		mButtonState.m = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_M) == GLFW_RELEASE)
	{
		mButtonState.m = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_N) == GLFW_PRESS)
	{
		mButtonState.n = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_N) == GLFW_RELEASE)
	{
		mButtonState.n = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_O) == GLFW_PRESS)
	{
		mButtonState.o = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_O) == GLFW_RELEASE)
	{
		mButtonState.o = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS)
	{
		mButtonState.p = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_RELEASE)
	{
		mButtonState.p = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mButtonState.q = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_RELEASE)
	{
		mButtonState.q = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_PRESS)
	{
		mButtonState.r = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_RELEASE)
	{
		mButtonState.r = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		mButtonState.s = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_RELEASE)
	{
		mButtonState.s = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_T) == GLFW_PRESS)
	{
		mButtonState.t = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_T) == GLFW_RELEASE)
	{
		mButtonState.t = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_U) == GLFW_PRESS)
	{
		mButtonState.u = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_U) == GLFW_RELEASE)
	{
		mButtonState.u = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_V) == GLFW_PRESS)
	{
		mButtonState.v = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_V) == GLFW_RELEASE)
	{
		mButtonState.v = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		mButtonState.w = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_RELEASE)
	{
		mButtonState.w = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_PRESS)
	{
		mButtonState.x = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_RELEASE)
	{
		mButtonState.x = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_Y) == GLFW_PRESS)
	{
		mButtonState.y = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_Y) == GLFW_RELEASE)
	{
		mButtonState.y = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_PRESS)
	{
		mButtonState.z = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_RELEASE)
	{
		mButtonState.z = false;
	}
	// tab-shift-control-alt
	if (glfwGetKey(mWindow, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		mButtonState.tab = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_TAB) == GLFW_RELEASE)
	{
		mButtonState.tab = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mButtonState.leftShift = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		mButtonState.leftShift = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
	{
		mButtonState.rightShift = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
	{
		mButtonState.rightShift = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		mButtonState.leftControl = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		mButtonState.leftControl = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
	{
		mButtonState.rightControl = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE)
	{
		mButtonState.rightControl = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		mButtonState.leftAlt = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
	{
		mButtonState.leftAlt = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
	{
		mButtonState.rightAlt = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_ALT) == GLFW_RELEASE)
	{
		mButtonState.rightAlt = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mButtonState.spacebar = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		mButtonState.spacebar = false;
	}
	// brackets
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
	{
		mButtonState.leftSquareBracket = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE)
	{
		mButtonState.leftSquareBracket = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
	{
		mButtonState.rightSquareBracket = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE)
	{
		mButtonState.rightSquareBracket = false;
	}
	// slash-quote-semicolon-enter
	if (glfwGetKey(mWindow, GLFW_KEY_BACKSLASH) == GLFW_PRESS)
	{
		mButtonState.backslash = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_BACKSLASH) == GLFW_RELEASE)
	{
		mButtonState.backslash = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
	{
		mButtonState.semiColon = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_SEMICOLON) == GLFW_RELEASE)
	{
		mButtonState.semiColon = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
	{
		mButtonState.apostrophe = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_APOSTROPHE) == GLFW_RELEASE)
	{
		mButtonState.apostrophe = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		mButtonState.enter = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_ENTER) == GLFW_RELEASE)
	{
		mButtonState.enter = false;
	}
	// comma-period-forwardslash
	if (glfwGetKey(mWindow, GLFW_KEY_COMMA) == GLFW_PRESS)
	{
		mButtonState.comma = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_COMMA) == GLFW_RELEASE)
	{
		mButtonState.comma = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_PERIOD) == GLFW_PRESS)
	{
		mButtonState.period = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PERIOD) == GLFW_RELEASE)
	{
		mButtonState.period = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SLASH) == GLFW_PRESS)
	{
		mButtonState.forwardSlash = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_SLASH) == GLFW_RELEASE)
	{
		mButtonState.forwardSlash = false;
	}
	// printscreen-etc
	if (glfwGetKey(mWindow, GLFW_KEY_PRINT_SCREEN) == GLFW_PRESS)
	{
		mButtonState.printScreen = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PRINT_SCREEN) == GLFW_RELEASE)
	{
		mButtonState.printScreen = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SCROLL_LOCK) == GLFW_PRESS)
	{
		mButtonState.scrollLock = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_SCROLL_LOCK) == GLFW_RELEASE)
	{
		mButtonState.scrollLock = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_PAUSE) == GLFW_PRESS)
	{
		mButtonState.pauseBreak = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PAUSE) == GLFW_RELEASE)
	{
		mButtonState.pauseBreak = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_INSERT) == GLFW_PRESS)
	{
		mButtonState.insert = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_INSERT) == GLFW_RELEASE)
	{
		mButtonState.insert = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_DELETE) == GLFW_PRESS)
	{
		mButtonState.del = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_DELETE) == GLFW_RELEASE)
	{
		mButtonState.del = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		mButtonState.home = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_HOME) == GLFW_RELEASE)
	{
		mButtonState.home = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_END) == GLFW_PRESS)
	{
		mButtonState.end = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_END) == GLFW_RELEASE)
	{
		mButtonState.end = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		mButtonState.pageUp = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PAGE_UP) == GLFW_RELEASE) {
		mButtonState.pageUp = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		mButtonState.pageDown = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PAGE_DOWN) == GLFW_RELEASE) {
		mButtonState.pageDown = false;
	}
	// arrows
	if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_PRESS) {
		mButtonState.upArrow = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_RELEASE) {
		mButtonState.upArrow = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mButtonState.downArrow = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_RELEASE) {
		mButtonState.downArrow = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
		mButtonState.leftArrow = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_RELEASE) {
		mButtonState.leftArrow = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		mButtonState.rightArrow = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
		mButtonState.rightArrow = false;
	}

	// mouse clicks
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		mButtonState.mouseButton1 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		mButtonState.mouseButton1 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		mButtonState.mouseButton2 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		mButtonState.mouseButton2 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		mButtonState.mouseButton3 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
		mButtonState.mouseButton3 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS) {
		mButtonState.mouseButton4 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_4) == GLFW_RELEASE) {
		mButtonState.mouseButton4 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS) {
		mButtonState.mousebutton5 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_5) == GLFW_RELEASE) {
		mButtonState.mousebutton5 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_6) == GLFW_PRESS) {
		mButtonState.mouseButton6 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_6) == GLFW_RELEASE) {
		mButtonState.mouseButton6 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_7) == GLFW_PRESS) {
		mButtonState.mousebutton7 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_7) == GLFW_RELEASE) {
		mButtonState.mousebutton7 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_8) == GLFW_PRESS) {
		mButtonState.mouseButton8 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_8) == GLFW_RELEASE) {
		mButtonState.mouseButton8 = false;
	}
}

}  // end namespace AA