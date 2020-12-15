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
#pragma once
#include "Window/Display.h"
#include "Controls/Input.h"
#include "Controls/Controls.h"
#include "Scene/Camera.h"
#include "Scene/Skybox.h"
#include "Scene/GameObject.h"
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

namespace AA
{
#define Engine AncientArcher::Get()
///
/// AncientArcher Class essentially ties everything together for render scenes
///
class AncientArcher : public Display, Controls
{
public:
	static AncientArcher* Get();
	int Run();

public:
	const Camera& GetCamera(int camId) const;
	Camera& GetCamera(int camId);
	const OGLShader& GetShader(int shadId) const;
	OGLShader& GetShader(int shadId);
	const GameObject& GetGameObject(int objId) const;
	GameObject& GetGameObject(int objId);

	int AddCamera(int w, int h);
	int AddShader(const char* vert_path, const char* frag_path, const bool isPath = true);
	int AddShader(const SHADERTYPE& type);
	int AddObject(const char* path, int cam_id, int shad_id);
	int AddObject(const char* path, int cam_id, int shad_id, const std::vector<InstanceDetails>& details);

	void SetSkybox(const std::shared_ptr<Skybox>& skybox) noexcept;

	uint32_t AddToOnBegin(void(*function)());
	uint32_t AddToDeltaUpdate(void(*function)(float));
	uint32_t AddToUpdate(void(*function)());
	uint32_t AddToSlowUpdate(void(*function)());
	uint32_t AddToTimedOutKeyHandling(bool(*function)(KeyboardInput&));
	uint32_t AddToScrollHandling(void(*function)(ScrollInput&));
	uint32_t AddToKeyHandling(void(*function)(KeyboardInput&));
	uint32_t AddToMouseHandling(void(*function)(MouseInput&));
	uint32_t AddToOnTeardown(void(*function)());

	bool RemoveFromOnBegin(uint32_t r_id);
	bool RemoveFromDeltaUpdate(uint32_t r_id);
	bool RemoveFromUpdate(uint32_t r_id);
	bool RemoveFromSlowUpdate(uint32_t r_id);
	bool RemoveFromTimedOutKeyHandling(uint32_t r_id);
	bool RemoveFromScrollHandling(uint32_t r_id);
	bool RemoveFromKeyHandling(uint32_t r_id);
	bool RemoveFromMouseHandling(uint32_t r_id);
	bool RemoveFromTeardown(uint32_t r_id);

	void SetSlowUpdateTimeoutLength(const float& newtime);
	void SetMaxRenderDistance(int camId, float amt) noexcept;
	//void SetWindowSize(const char to, int width = -1, int height = -1, bool center = false);
	void SetWindowPos(int xpos, int ypos);
	void SetProjectionMatrix(int shadId, int camId);
	void SetCursorToEnabled(bool isHardwareRendered = false);
	
	//void SetCursorToDisabled();
	//void SetToPerspectiveMouseHandling();
	//void SetToStandardMouseHandling();
	//void SetWindowTitle(const char* title);

	void reshapeWindowHandler(GLFWwindow* window, int width, int height);
	void perspectiveMouseHandler(GLFWwindow* window, float xpos, float ypos);
	void standardMouseHandler(GLFWwindow* window, float xpos, float ypos);
	void scrollHandler(GLFWwindow* window, float xpos, float ypos);

	void pullButtonStateEvents();

private:
	AncientArcher();

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

	std::unordered_map<uint32_t, std::function<void()> > onBegin;              ///< list of functions to run once when runMainAncientArcher is called
	std::unordered_map<uint32_t, std::function<void(float)> > onDeltaUpdate;   ///< list of functions that rely on deltatime in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void()> > onUpdate;             ///< list of functions that run every frame in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void()> > onSlowUpdate;         ///< list of functions to run every only every mSlowUpdateWaitLength in the main AncientArcher
	std::unordered_map<uint32_t, std::function<bool(KeyboardInput&)> > onTimeoutKeyHandling; ///< list of functions to handle key presses that time out for mNoSpamWaitLength after press
	std::unordered_map<uint32_t, std::function<void(ScrollInput&)> > onScrollHandling;       ///< list of functions to handle mouse wheel scroll every frame in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void(KeyboardInput&)> > onKeyHandling;        ///< list of functions to handle keypresses every frame in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void(MouseInput&)> > onMouseHandling;         ///< list of functions to handle mouse movement every frame in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void()> > onTearDown;                         ///< list of functions to run when destroying

	// called during Run
	void begin();
	void deltaUpdate();
	void render();
	void teardown();

	// helpers
	void initEngine();
	void ResetEngine() noexcept;

	void SetReshapeWindowHandler() noexcept;
	void SetCurorPosToPerspectiveCalc() noexcept;
	void SetCurorPosToStandardCalc() noexcept;
	void SetScrollWheelHandler() noexcept;

	void standardMouseMovement(float xpos, float ypos);

	// todo: refactor - used for testing purposes until more elegant solution appears
	void SetProjectionMatToAllShadersFromFirstCam_hack();
};
}  // end namespace AA