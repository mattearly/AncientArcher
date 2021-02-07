#pragma once
#include "Window/Display/Display.h"
#include "Scene/Camera.h"
#include "Renderer/OpenGL/Skybox.h"
#include "Scene/GameObject.h"
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>
#include "Settings/Settings.h"

class OGLShader;
enum class SHADERTYPE;
struct InstanceDetails;

namespace AA
{
#define Engine AncientArcher::Get()
#define Cam(X) AncientArcher::Get()->GetCamera(X)
#define Obj(X) AncientArcher::Get()->GetGameObject(X)
///
/// AncientArcher Class essentially ties everything together for render scenes
///
class AncientArcher : public Display
{
public:
	static AncientArcher* Get();
	int Run();
	void Shutdown() noexcept;
	void SoftReset();

public:
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

	void SetProjectionMatrix(int shadId, int camId);
	void SetCursorToEnabled(bool isHardwareRendered = false);

	const Camera& GetCamera(int camId) const;
	Camera& GetCamera(int camId);
	const OGLShader& GetShader(int shadId) const;
	OGLShader& GetShader(int shadId);
	const GameObject& GetGameObject(int objId) const;
	GameObject& GetGameObject(int objId);

private:
	AncientArcher();
	
	//float mLastFrameTime;                  ///< keeps track of the previous frame's time
	//float mCurrentFrameTime;               ///< keeps track of the current frame's time
	//float mDeltaTime;                      ///< the delta difference between previous and current frame
	float mNonSpammableKeysTimeout;        ///< keeps track of how long the keys have timed out
	float mNoSpamWaitLength;               ///< how long the non-spammable keys are to time out for at least
	float mSlowUpdateTimeout;              ///< keeps track of how how long the slow update has been timed out
	float mSlowUpdateWaitLength;           ///< ms length the slow update times out for at least

	std::vector<Camera>     mCameras;      ///< list of available cameras
	std::vector<OGLShader>  mShaders;      ///< list of available shaders
	std::vector<GameObject> mGameObjects;  ///< list of available objects
	std::shared_ptr<Skybox> mSkybox;       ///< the main skybox

	std::unordered_map<uint32_t, std::function<void()> >               onBegin;               ///< list of functions to run once when runMainAncientArcher is called
	std::unordered_map<uint32_t, std::function<void(float)> >          onDeltaUpdate;         ///< list of functions that rely on deltatime in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void()> >               onUpdate;              ///< list of functions that run every frame in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void()> >               onSlowUpdate;          ///< list of functions to run every only every mSlowUpdateWaitLength in the main AncientArcher
	std::unordered_map<uint32_t, std::function<bool(KeyboardInput&)> > onTimeoutKeyHandling;  ///< list of functions to handle key presses that time out for mNoSpamWaitLength after press
	std::unordered_map<uint32_t, std::function<void(ScrollInput&)> >   onScrollHandling;      ///< list of functions to handle mouse wheel scroll every frame in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void(KeyboardInput&)> > onKeyHandling;         ///< list of functions to handle keypresses every frame in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void(MouseInput&)> >    onMouseHandling;       ///< list of functions to handle mouse movement every frame in the main AncientArcher
	std::unordered_map<uint32_t, std::function<void()> >               onTearDown;            ///< list of functions to run when destroying

	// called during Run
	void begin();
	void deltaUpdate();
	void render();
	void teardown();

	// helpers
	void initEngine();
	void resetEngine() noexcept;

	// todo: refactor - used for testing purposes until more elegant solution appears
	void __updateCamViewMatrices(int width, int height);
	void __setProjectionMatToAllShadersFromFirstCam_hack();
};
}  // end namespace AA