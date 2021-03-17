#include "Scene/GameObject.h"
#include "Renderer/SceneLoader.h"
#include "Renderer/OpenGL/OGLShader.h"
#include "../include/AncientArcher/AncientArcher.h"
#include "Sound/SoundDevice.h"
#include "Sound/ShortSound.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <chrono>
#include <memory>
#include <iostream>
#include <algorithm>
#include "Shader/LitShader.h"
#include "Shader/DiffShader.h"
namespace AA
{

extern bool externWindowSizeDirty;

AncientArcher* AncientArcher::Get()
{
	SoundDevice::Init();
	static AncientArcher* aa = new AncientArcher();
	return aa;
}

int AncientArcher::Run()
{
	begin();
	while (!Display::isTryingToClose()) 
	{ 
		deltaUpdate(); 
		render(); 
	}
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
	// std::cout << "cam ID by the ID [" << camId << "] was not found. unable to get AACamera\n";
	throw("u messed up");
}

const Camera& AncientArcher::GetCamera(int camId) const
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
	throw("u messed up");
}

//OGLShader& AncientArcher::GetShader(int shadId)
//{
//	for (auto& shad : mShaders)
//	{
//		if (shad.GetID() == shadId)
//		{
//			return shad;
//		}
//	}
//
//	// if it didn't find it and return above ^^^^^^^^  show error message in console
//	//std::cout << "shad ID by the ID [" << shadId << "] was not found.\n";
//	throw("u messed up");
//}

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
	throw("u messed up");
}

const GameObject& AncientArcher::GetGameObject(int objId) const
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
	throw("u messed up");
}

ShortSound& AncientArcher::GetSpeaker(int speaker_id)
{
	std::cout << "getting speaker\n";
	//todo optimize
	if (speaker_id < mSpeakers.size())
		return mSpeakers[speaker_id];

	// if it gets here you're doing something illegal
	throw("out of range of added speakers");
}

LongSound& AncientArcher::GetMusic()
{
	if (mMusic)
		return *mMusic;
	else
		std::cout << "no music loaded\n";
}

void AncientArcher::PlaySoundEffect(int effect_id, int speaker_id)
{
	mSpeakers[speaker_id].PlayNoOverlap(mLoadedSoundEffects[effect_id].id);
}

AncientArcher::AncientArcher()
{
	mNonSpammableKeysTimeout = 0.f;
	mSlowUpdateTimeout = 0.f;
	mNoSpamWaitLength = .5667f;
	mSlowUpdateWaitLength = .3337f;
	mDiffShader = NULL;
	mLitShader = NULL;
	mMusic = NULL;
	mDirectionalLight = NULL;
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
//
//int AncientArcher::AddShader(const char* vert_src, const char* frag_src)
//{
//	mShaders.emplace_back(OGLShader(vert_src, frag_src));
//	return mShaders.back().GetID();
//}

int AncientArcher::AddObject(const char* path, int camId, bool is_lit)
{
	if (is_lit)
		setupLitShader();
	else
		setupDiffShader();

	GameObject tmpObject(path, camId, is_lit);
	const int return_id = tmpObject.GetObjectId();

	mGameObjects.push_back(tmpObject);

	return return_id;
}

int AncientArcher::AddObject(const char* path, int cam_id, bool is_lit, const std::vector<InstanceDetails>& details)
{
	// todo: optimize. check if it is an object we already have loaded and use it again if so. this will require the same with textures

	if (is_lit)
		setupLitShader();
	else
		setupDiffShader();

	GameObject tmpObject(path, cam_id, is_lit, details);

	const int return_id = tmpObject.GetObjectId();

	mGameObjects.push_back(tmpObject);

	return return_id;
}

void AncientArcher::SetDirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
{
	if (!mLitShader)
		setupLitShader();

	if (!mDirectionalLight)
	{
		mDirectionalLight = new DirectionalLight(dir, amb, diff, spec);
	}
	else
	{
		//update whatever it was
		mDirectionalLight->Direction = dir;
		mDirectionalLight->Ambient = amb;
		mDirectionalLight->Diffuse = diff;
		mDirectionalLight->Specular = spec;
	}


	{
		mLitShader->use();
		mLitShader->setVec3("directionalLight.Direction", mDirectionalLight->Direction);
		mLitShader->setVec3("directionalLight.Ambient", mDirectionalLight->Ambient);
		mLitShader->setVec3("directionalLight.Diffuse", mDirectionalLight->Diffuse);
		//mLitShader->setVec3("directionalLight.Specular", mDirectionalLight->Specular); // ?? not used or optimized away i guess
	}
}


int AncientArcher::AddSpotLight(glm::vec3 pos, glm::vec3 dir, float inner, float outer, float constant,
	float linear, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
{
	if (mSpotLights.size() > MAXSPOTLIGHTS)
	{
		throw("too many spot lights");
	}
	
	if (!mLitShader)
		setupLitShader();

	mSpotLights.emplace_back(SpotLight(pos, dir, inner, outer, constant, linear, quad, amb, diff, spec));
	int new_loc = mSpotLights.size() - 1;
	
	// push changes to shader
	{
		std::string pos, ambient, constant, cutoff, ocutoff, diffuse, direction, linear, quadrat, specular;
		ambient = constant = cutoff = ocutoff = diffuse = direction = linear = quadrat = specular = pos = "spotLight[";
		std::stringstream ss;
		ss << new_loc;
		pos += ss.str();
		constant += ss.str();
		cutoff += ss.str();
		ocutoff += ss.str();
		direction += ss.str();
		linear += ss.str();
		quadrat += ss.str();
		ambient += ss.str();
		diffuse += ss.str();
		specular += ss.str();
		pos += "].";
		constant += "].";
		cutoff += "].";
		ocutoff += "].";
		direction += "].";
		linear += "].";
		quadrat += "].";
		ambient += "].";
		diffuse += "].";
		specular += "].";
		pos += "Position";
		constant += "Constant";
		cutoff += "CutOff";
		ocutoff += "OuterCutOff";
		direction += "Direction";
		linear += "Linear";
		quadrat += "Quadratic";
		ambient += "Ambient";
		diffuse += "Diffuse";
		specular += "Specular";

		mLitShader->use();
		mLitShader->setVec3(pos,       mSpotLights.back().Position);
		mLitShader->setFloat(cutoff,   mSpotLights.back().CutOff);
		mLitShader->setFloat(ocutoff,  mSpotLights.back().OuterCutOff);
		mLitShader->setVec3(direction, mSpotLights.back().Direction);
		mLitShader->setFloat(linear,   mSpotLights.back().Linear);
		mLitShader->setFloat(quadrat,  mSpotLights.back().Quadratic);
		mLitShader->setVec3(ambient,   mSpotLights.back().Ambient);
		mLitShader->setVec3(diffuse,   mSpotLights.back().Diffuse);
		mLitShader->setVec3(specular,  mSpotLights.back().Specular);
		mLitShader->setInt("NUM_SPOT_LIGHTS", new_loc+1);
	}
	
	return mSpotLights.back().id;  // unique id
}

void AncientArcher::MoveSpotLight(int which, glm::vec3 new_pos, glm::vec3 new_dir)
{
	if (which < 0)
		throw("dont");

	for (auto& sl : mSpotLights)
	{
		if (sl.id == which)
		{
			sl.Position = new_pos;
			sl.Direction = new_dir;
			mLitShader->use();
			mLitShader->setVec3("spotLight[0].Position", sl.Position);  //test
			mLitShader->setVec3("spotLight[0].Direction", sl.Direction);  //test
			std::cout << "updated spot light pos and dir\n";
			return;
		}
	}

	throw("u messed up");
}

// returns true if it removed the spot light, false otherwise
bool AncientArcher::RemoveSpotLight(int which_by_id)
{
	if (mSpotLights.empty())
		return false;

	int before_size = mPointLights.size();

	auto ret_it = mSpotLights.erase(
		std::remove_if(mSpotLights.begin(), mSpotLights.end(), [&](const SpotLight sl) { return sl.id == which_by_id; }),
		mSpotLights.end());

	int after_size = mPointLights.size();

	if (before_size != after_size)
		return true;
	else
		return false;
}

/// <summary>
/// adds a new sound effect buffer ready fro playback
/// </summary>
/// <param name="path">logical path to the sound effect file</param>
/// <returns>-1 if already loaded, -2 if failed to load, else returns the location in the vector </returns>
int AncientArcher::AddSoundEffect(const char* path)
{
	SoundDevice::Init();
	for (const auto& pl : mLoadedSoundEffects)
	{
		if (path == pl.path.c_str())
			return -1;  // sound already loaded
	}

	uint32_t tmp_id = ShortSound::AddShortSound(path);
	if (tmp_id != 0)
	{
		SoundEffect e;
		e.id = tmp_id;
		e.path = path;
		mLoadedSoundEffects.push_back(e);
		return (mLoadedSoundEffects.size() - 1);  // the index into mSoundEffectBuffers 
	}
	else
	{
		return -2;  // failed to load
	}

	//return -3;  // should never get here
}

void AncientArcher::RemoveSoundEffect(int effect_id)
{
	bool success = false;
	success = ShortSound::RemoveShortSound(mLoadedSoundEffects[effect_id].id);
	if (success)
	{
		mLoadedSoundEffects.erase(mLoadedSoundEffects.begin() + effect_id);
	}
}


int AncientArcher::AddSpeaker()
{
	mSpeakers.resize(mSpeakers.size()+1);
	return (mSpeakers.size() - 1);  // return index of last added
}

void AncientArcher::ChangeMusic(const char* path) 
{
	if (!mMusic)
		mMusic = new LongSound(path);
}

// todo: make a managed AddSkybox instead
void AncientArcher::SetSkybox(const std::shared_ptr<Skybox>& skybox) noexcept
{
	mSkybox = skybox;
}
//
//void AncientArcher::SetLight(SpotLight light, int shadId, int which)
//{
//	if (which >= MAXSPOTLIGHTS)
//	{
//		std::cout << "couldn't set SpotLight light, too many\n";
//		return;
//	}
//
//	if (which >= NUM_SPOT_LIGHTS)
//	{
//		std::cout << "couldn't set SpotLight light("
//			<< which 
//			<< "), not enough in use(" 
//			<< NUM_SPOT_LIGHTS 
//			<< ")\n";
//		return;
//	}
//
//	//shader.use();
//
//	GetShader(shadId).setInt("NUM_SPOT_LIGHTS", NUM_SPOT_LIGHTS);
//
//	std::string pos, ambient, constant, cutoff, ocutoff, diffuse, direction, linear, quadrat, specular;
//	ambient = constant = cutoff = ocutoff = diffuse = direction = linear = quadrat = specular = pos = "spotLight[";
//
//	std::stringstream ss;
//	ss << which;
//
//	pos += ss.str();
//	constant += ss.str();
//	cutoff += ss.str();
//	ocutoff += ss.str();
//	direction += ss.str();
//	linear += ss.str();
//	quadrat += ss.str();
//	ambient += ss.str();
//	diffuse += ss.str();
//	specular += ss.str();
//
//	pos += "].";
//	constant += "].";
//	cutoff += "].";
//	ocutoff += "].";
//	direction += "].";
//	linear += "].";
//	quadrat += "].";
//	ambient += "].";
//	diffuse += "].";
//	specular += "].";
//
//	pos += "Position";
//	constant += "Constant";
//	cutoff += "CutOff";
//	ocutoff += "OuterCutOff";
//	direction += "Direction";
//	linear += "Linear";
//	quadrat += "Quadratic";
//	ambient += "Ambient";
//	diffuse += "Diffuse";
//	specular += "Specular";
//
//	GetShader(shadId).setVec3(pos, light.Position);
//	GetShader(shadId).setFloat(cutoff, light.CutOff);
//	GetShader(shadId).setFloat(ocutoff, light.OuterCutOff);
//	GetShader(shadId).setVec3(direction, light.Direction);
//	GetShader(shadId).setFloat(linear, light.Linear);
//	GetShader(shadId).setFloat(quadrat, light.Quadratic);
//	GetShader(shadId).setVec3(ambient, light.Ambient);
//	GetShader(shadId).setVec3(diffuse, light.Diffuse);
//	GetShader(shadId).setVec3(specular, light.Specular);
//}


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
//
//void AncientArcher::SetProjectionMatrix(int shadId, int camId)
//{
//	GetShader(shadId).use();
//	GetShader(shadId).setMat4("projection", GetCamera(camId).GetProjectionMatrix());
//}

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
	lastTime = currTime;

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
		externWindowSizeDirty = false;
	}

	for (auto& obj : mGameObjects)
	{
		// set the view matrix from the cam for this object
		if (obj.mIsLit)  // && mLitShader?
		{
			//GetShader(obj.GetShaderId()).use();
			//std::cout << "setting view for lit shader\n";
			mLitShader->use();
			mLitShader->setMat4("view", GetCamera(obj.GetCameraId()).GetViewMatrix());
		}
		else
		{
			//std::cout << "setting view for diff shader\n";
			mDiffShader->use();
			mDiffShader->setMat4("view", GetCamera(obj.GetCameraId()).GetViewMatrix());
		}
		
		// draw using the shader for it
		obj.draw();
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
	//for (auto& shader : mShaders)
	//{
	//	shader.deleteShader();
	//}
	if (mLitShader) mLitShader->deleteShader();
	if (mDiffShader) mDiffShader->deleteShader();

	for (const auto& ss : mLoadedSoundEffects)
	{
		ShortSound::RemoveShortSound(ss.id);
	}
}

void AncientArcher::resetEngine() noexcept
{
	// process anything the user Requested and unload all meshes and textures
	teardown();

	mCameras.clear();
	//mShaders.clear();
	mGameObjects.clear();
	mLoadedSoundEffects.clear();
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

void AncientArcher::setupLitShader()
{
	if (!mLitShader) {
		mLitShader = new OGLShader(lit_vert_src, lit_frag_src);
		std::cout << "Lit Shader is Live!\n";
	}
}

void AncientArcher::setupDiffShader()
{
	if (!mDiffShader) {
		mDiffShader = new OGLShader(diff_vert_src, diff_frag_src);
		std::cout << "Diff Shader is Live!\n";
	}
}

void AncientArcher::__setProjectionMatToAllShadersFromFirstCam_hack()
{
	// set proj matries hack from first cam
	//for (auto& shad : mShaders)
	//{
		//shad.setMat4("projection", mCameras.front().GetProjectionMatrix());
		//const int sID = shad.GetID();
		//std::cout << "shad id: " << sID << '\n';
		//const int cID = mCameras.front().GetID();
		//std::cout << "shad id: " << sID << '\n';
		//SetProjectionMatrix(sID, cID);  //set shader (sID) from cam (cID)
	//}

	if (mLitShader)
	{
		std::cout << "setting projection for lit shader\n";
		mLitShader->use();
		mLitShader->setMat4("projection", mCameras.front().GetProjectionMatrix());
	}

	if (mDiffShader)
	{
		std::cout << "setting projection for diff shader\n";
		mDiffShader->use();
		mDiffShader->setMat4("projection", mCameras.front().GetProjectionMatrix());
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