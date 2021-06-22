#pragma once
#include "MouseReporting.h"
#include "../include/AncientArcher/Types.h"
#include "Scene/Lights.h"
#include "Scene/Prop.h"
#include "Scene/Skybox.h"
#include "Renderer/OpenGL/OGLShader.h"
#include "Sound/Speaker.h"
#include "Sound/SoundEffect.h"
#include "Sound/LongSound.h"
#include "GUI/PlainGUI.h"
#include "GUI/imGUI.h"
#include <glm/glm.hpp>
#include <functional>

namespace AA{
namespace Core {
extern void SetMouseReadToFPP() noexcept;
extern void SetMouseReadToNormal() noexcept;
extern i32 GetWindowWidth() noexcept;
extern i32 GetWindowHeight() noexcept;
extern void SetWindowClearColor(glm::vec3 color) noexcept;
extern void SetWindowTitle(const char* name) noexcept;
extern void SetMouseToHidden() noexcept;
extern void SetMouseToDisabled() noexcept;
extern void SetMouseToNormal() noexcept;
extern void Init();
extern i32 Run();
extern void Shutdown();
}

extern bool isEngineInit;
extern bool isWindowSizeDirty;
extern GLFWwindow* mWindow;
extern const f32 mDefaultFPPMouseSensitivity;
extern f32 mFPPMouseSensitivity;
extern bool mSwitchedToFPP;
extern MouseReporting mMouseReporting;
extern PlainGUI* mGUI;
extern imGUI* mimGUI;
extern const char* vert_path;
extern const char* frag_lit_path;
extern const char* frag_diff_path;
extern OGLShader* mDiffShader;
extern OGLShader* mLitShader;
extern void setupLitShader();
extern void setupDiffShader();
extern DirectionalLight* mDirectionalLight;
extern std::vector<PointLight> mPointLights;
extern i32 mNumPointLightsInUse;
extern const i32 MAXPOINTLIGHTS;
extern std::vector<SpotLight> mSpotLights;
extern i32 mNumSpotLightsInUse;
extern const i32 MAXSPOTLIGHTS;
extern std::vector<Camera> mCameras;
extern void updateProjectionFromCam(OGLShader* shader_to_update, const Camera& from_cam);
extern std::vector<Prop> mProps;
extern std::shared_ptr<Skybox> mSkybox;
extern std::vector<Speaker*> mSpeakers;
extern std::vector<SoundEffect*> mSoundEffects;
extern LongSound* mMusic;
extern f32 mNonSpammableKeysTimeout;
extern f32 mNoSpamWaitLength;
extern f32 mSlowUpdateTimeout;
extern f32 mSlowUpdateWaitLength;
extern std::unordered_map<u32, std::function<void()> > onBegin;
extern std::unordered_map<u32, std::function<void(f32)> > onDeltaUpdate;
extern std::unordered_map<u32, std::function<void()> > onUpdate;
extern std::unordered_map<u32, std::function<void()> > onSlowUpdate;
extern std::unordered_map<u32, std::function<bool(KeyboardInput&)> > onTimeoutKeyHandling;
extern std::unordered_map<u32, std::function<void(ScrollInput&)> > onScrollHandling;
extern std::unordered_map<u32, std::function<void(KeyboardInput&)> > onKeyHandling;
extern std::unordered_map<u32, std::function<void(MouseInput&)> > onMouseHandling;
extern std::unordered_map<u32, std::function<void()> > onTearDown;
extern KeyboardInput mButtonState;
extern MouseInput mMousePosition ;
extern ScrollInput mMouseWheelScroll;
extern bool mNewKeyReads;
extern const i32 MINSCREENWIDTH;
extern const i32 MINSCREENHEIGHT;
extern const i32 MAXSCREENWIDTH;
extern const i32 MAXSCREENHEIGHT;
extern void begin();
extern void update();
extern void render();
extern void teardown();
} // end namespace AA
