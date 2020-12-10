/*
Loop
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
#pragma once
#include "Display.h"
#include "Input.h"
#include "Controls.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameObject.h"
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

//todo: put these in namespace and cleanup subprojects
#define LOOP           AA::Loop::getMainLoop()
#define DISPLAY        AA::Display::getInstance()
#define CONTROLS       AA::Controls::getInstance()
#define PointerXOffset AA::Controls::getInstance()->mMousePosition.xOffset
#define PointerYOffset AA::Controls::getInstance()->mMousePosition.yOffset

namespace AA
{
/// <summary>
/// The main game loop.
/// </summary>
class Loop
{
public:
	static Loop* getMainLoop();
	int runMainLoop();
	void shutdown();

private:
	Loop();  // SINGLETON: only accessable through getMainLoop
	//Loop(const char* title);

public:
	const Camera& getCamera(int camId) const;
	Camera& getCamera(int camId);

	const OGLShader& getShader(int shadId) const;
	OGLShader& getShader(int shadId);

	const GameObject& getGameObject(int objId) const;
	GameObject& getGameObject(int objId);

	const MouseReporting getMouseReportingMode() const;
	const int getCursorMode() const;

	int addCamera();
	int addShader(const char* vert_path, const char* frag_path, const bool isPath = true);
	int addShader(const SHADERTYPE& type);
	int addObject(const char* path, int cam_id, int shad_id);
	int addObject(const char* path, int cam_id, int shad_id, const std::vector<InstanceDetails>& details);

	void setSkybox(const std::shared_ptr<Skybox>& skybox) noexcept;

	uint32_t addToOnBegin(void(*function)());
	uint32_t addToDeltaUpdate(void(*function)(float));
	uint32_t addToUpdate(void(*function)());
	uint32_t addToSlowUpdate(void(*function)());
	uint32_t addToTimedOutKeyHandling(bool(*function)(KeyboardInput&));
	uint32_t addToScrollHandling(void(*function)(ScrollInput&));
	uint32_t addToKeyHandling(void(*function)(KeyboardInput&));
	uint32_t addToMouseHandling(void(*function)(MouseInput&));
	uint32_t addToOnTeardown(void(*function)());

	bool removeFromOnBegin(uint32_t r_id);
	bool removeFromDeltaUpdate(uint32_t r_id);
	bool removeFromUpdate(uint32_t r_id);
	bool removeFromSlowUpdate(uint32_t r_id);
	bool removeFromTimedOutKeyHandling(uint32_t r_id);
	bool removeFromScrollHandling(uint32_t r_id);
	bool removeFromKeyHandling(uint32_t r_id);
	bool removeFromMouseHandling(uint32_t r_id);
	bool removeFromTeardown(uint32_t r_id);

	void setCursorToEnabled(bool isHardwareRendered = false);
	void setCursorToDisabled();
	void setToPerspectiveMouseHandling();
	void setToStandardMouseHandling();
	void setWindowTitle(const char* title);
	void setMaxRenderDistance(int camId, float amt) noexcept;
	void setProjectionMatrix(int shadId, int camId);
	void setSlowUpdateTimeoutLength(const float& newtime);

	friend class Controls;

private:

	float mLastFrameTime;                                                     ///< keeps track of the previous frame's time
	float mCurrentFrameTime;                                                  ///< keeps track of the current frame's time
	float mDeltaTime;                                                         ///< the delta difference between previous and current frame
	float mNonSpammableKeysTimeout;                                           ///< keeps track of how long the keys have timed out
	float mNoSpamWaitLength;                                                  ///< how long the non-spammable keys are to time out for at least
	float mSlowUpdateTimeout;                                                 ///< keeps track of how how long the slow update has been timed out
	float mSlowUpdateWaitLength;                                              ///< ms length the slow update times out for at least

	std::vector<Camera>     mCameras;                                         ///< list of available cameras
	std::vector<OGLShader>  mShaders;                                         ///< list of available shaders
	std::vector<GameObject> mGameObjects;                                     ///< list of available objects
	std::shared_ptr<Skybox> mSkybox;                                          ///< the main skybox

	std::unordered_map<uint32_t, std::function<void()> > onBegin;              ///< list of functions to run once when runMainLoop is called
	std::unordered_map<uint32_t, std::function<void(float)> > onDeltaUpdate;   ///< list of functions that rely on deltatime in the main loop
	std::unordered_map<uint32_t, std::function<void()> > onUpdate;             ///< list of functions that run every frame in the main loop
	std::unordered_map<uint32_t, std::function<void()> > onSlowUpdate;         ///< list of functions to run every only every mSlowUpdateWaitLength in the main loop
	std::unordered_map<uint32_t, std::function<bool(KeyboardInput&)> > onTimeoutKeyHandling; ///< list of functions to handle key presses that time out for mNoSpamWaitLength after press
	std::unordered_map<uint32_t, std::function<void(ScrollInput&)> > onScrollHandling;       ///< list of functions to handle mouse wheel scroll every frame in the main loop
	std::unordered_map<uint32_t, std::function<void(KeyboardInput&)> > onKeyHandling;        ///< list of functions to handle keypresses every frame in the main loop
	std::unordered_map<uint32_t, std::function<void(MouseInput&)> > onMouseHandling;         ///< list of functions to handle mouse movement every frame in the main loop
	std::unordered_map<uint32_t, std::function<void()> > onTearDown;                         ///< list of functions to run when destroying

	// called during runMainLoop
	void begin();
	void deltaUpdate();
	void render();

	// helpers
	void initEngine();
	void resetEngine() noexcept;
	void setProjectionMatToAllShadersFromFirstCam_hack();  //used for testing purposes until more elegant solution appears
};
}  // end namespace AA