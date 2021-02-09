#include "../include/AncientArcher.h"
#include "../include/Renderer/SceneLoader.h"
#include <vector>
#include <string>
#include <iomanip>
#include <utility>
#include <chrono>
namespace AA
{

extern bool externWindowSizeDirty;

AncientArcher* AncientArcher::Get()
{
	static AncientArcher* aa = new AncientArcher();
	return aa;
}

int AncientArcher::Run()
{
	begin();
	while (!Display::isTryingToClose()) { deltaUpdate(); render(); }
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
		if (obj.GetObjectId() == objId)
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
		if (obj.GetObjectId() == objId)
		{
			return obj;
		}
	}

	// if it didn't find it and return above ^^^^^^^^  show error message in console
	//std::cout << "game object ID by the ID [" << objId << "] was not found.\n";
	exit(-1);
}

AncientArcher::	AncientArcher()
{
	mNonSpammableKeysTimeout = 0.f;
	mSlowUpdateTimeout = 0.f;
	mNoSpamWaitLength = .5667f;
	mSlowUpdateWaitLength = .3337f;
}

void AncientArcher::__updateCamViewMatrices(int width, int height)
{
	for (auto& cam : mCameras)
	{
		cam.SetDimensions(width, height);
	}

	__setProjectionMatToAllShadersFromFirstCam_hack();
}

int AncientArcher::AddCamera(int w, int h)
{
	Camera tmpCam(w, h);
	const int return_id = tmpCam.GetID();

	mCameras.push_back(tmpCam);

	return return_id;
}

int AncientArcher::AddShader(const char* vert_src, const char* frag_src)
{
	mShaders.emplace_back(OGLShader(vert_src, frag_src));
	return mShaders.back().GetID();
}

int AncientArcher::AddObject(const char* path, int camId, int shadId)
{
	GameObject tmpObject(path, camId, shadId);
	const int return_id = tmpObject.GetObjectId();

	mGameObjects.push_back(tmpObject);

	return return_id;
}

int AncientArcher::AddObject(const char* path, int cam_id, int shad_id, const std::vector<InstanceDetails>& details)
{
	// todo: optimize. check if it is an object we already have loaded and use it again if so. this will require the same with textures

	GameObject tmpObject(path, cam_id, shad_id, details);

	const int return_id = tmpObject.GetObjectId();

	mGameObjects.push_back(tmpObject);

	return return_id;
}

// todo: make a managed AddSkybox instead
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

void AncientArcher::begin()
{
	Display::keepWindowOpen();

	for (const auto& oB : onBegin)
	{
		oB.second();
	}

	__setProjectionMatToAllShadersFromFirstCam_hack();
}

void AncientArcher::deltaUpdate()
{
	pullButtonStateEvents();

	// init delta clock on first tap into deltaUpdate
	static std::chrono::system_clock::time_point currTime;
	static std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();
	static std::chrono::duration<float> deltaTime;

	// update engine run delta times
	currTime = std::chrono::system_clock::now();
	deltaTime = currTime - lastTime;
	lastTime  = currTime;

	// go through all updates that need access to delta time
	float elapsedTime = deltaTime.count();
	for (auto& oDU : onDeltaUpdate)
	{
		oDU.second(elapsedTime);
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
	if (isFPP())
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
	mSlowUpdateTimeout += elapsedTime;
	// check to see if its time to process delayed updates
	if (mSlowUpdateTimeout > mSlowUpdateWaitLength)
	{
		// process all delayed updates
		for (auto& oSU : onSlowUpdate)
		{
			oSU.second();
		}
		// we should also process whether the window size changed here
		//if (WindowSizeDirty)
		//{
		//	// notify shader/cams
		//	__setProjectionMatToAllShadersFromFirstCam_hack();

		//	// don't process again
		//	WindowSizeDirty = false;
		//}
		// reset timeout length to 0
		mSlowUpdateTimeout = 0.f;
	}

	// needs updated, we'll use it in update with keyboard functions before the AncientArcher is done.
	// note that the keyboard processing cant be here because we have yet to run processSystemKeys()
	mNonSpammableKeysTimeout += elapsedTime;

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

	if (externWindowSizeDirty)
	{
		__updateCamViewMatrices(Display::GetWindowWidth(), Display::GetWindowHeight());
	}

	for (auto& obj : mGameObjects)
	{
		// get the id of the shader for this object
		const int shaderID = obj.GetShaderId();

		// switch to the shader
		GetShader(shaderID).use();

		// get the camera id for this object
		const int cameraID = obj.GetCameraId();

		// set the view matrix from the cam for this object
		GetShader(shaderID).setMat4("view", GetCamera(cameraID).GetViewMatrix());

		// draw using the shader for it
		obj.draw(GetShader(shaderID));
	}

	// draw skybox if one was specified
	if (mSkybox && !mCameras.empty()) { mSkybox->render(mCameras.front()); }

	swapWindowBuffers();
}

void AncientArcher::teardown()
{
	// run user preferred functions first
	for (auto& oTD : onTearDown)
	{
		oTD.second();
	}
	// delete all the meshes and textures from GPU memory
	for (const auto& model : mGameObjects)
	{
		SceneLoader::Get()->UnloadGameObject(model.mMeshes);
	}
	// delete all the shaders from GPU
	for (auto& shader : mShaders)
	{
		shader.deleteShader();
	}
 }


void AncientArcher::resetEngine() noexcept
{
	// process anything the user Requested and unload all meshes and textures
	teardown();

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

	mNonSpammableKeysTimeout = 0.f;
	mSlowUpdateTimeout = 0.f;
	mNoSpamWaitLength = .5667f;
	mSlowUpdateWaitLength = .3337f;

	SetCursorToEnabled();
	SetReadMouseCurorAsStandard();

	Display::SetClearColor();
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
}  // end namespace AA