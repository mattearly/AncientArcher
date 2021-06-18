#pragma once
#include "Types.h"
#include "../../src/Renderer/OpenGL/Skybox.h"
#include "../../src/Controls/KeyboardInput.h"
#include "../../src/Controls/MouseInput.h"
#include "../../src/Controls/ScrollInput.h"
#include "ShaderType.h"
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

namespace AA {


// Init, Run, Shutdown, Reset
void InitEngine();
i32 Run();
void Shutdown() noexcept;
void SoftReset() noexcept;
void DragAndDropFiles(bool allowed);
// End Init, Run, Shutdown, Reset


// Camera
i32 AddCamera(i32 w, i32 h);
i32 AddCamera();
bool RemoveCamera(i32 camId);
void SetCamMaxRenderDistance(i32 camId, f32 amt);
void SetCamToPerspective(i32 camId);
void SetCamToOrtho(i32 camId);
void SetCamFOV(i32 camId, f32 new_fov);
void SetCamDimensions(i32 camId, i32 w, i32 h);
void SetCamPosition(i32 camId, vec3 new_loc);
void SetCamPitch(i32 camId, f32 new_pitch_degrees);
void SetCamYaw(i32 camId, f32 new_yaw_degrees);
void ShiftCamPosition(i32 camId, vec3 offset);
void ShiftCamPitchAndYaw(i32 camId, f64 pitch_offset, f64 yaw_offset);
vec3 GetCamFront(i32 camId);
vec3 GetCamRight(i32 camId);
vec3 GetCamPosition(i32 camId);
f32 GetCamPitch(i32 camId);
f32 GetCamYaw(i32 camId);
vec2 GetPitchAndYaw(i32 camId);
mat4 GetProjectionMatrix(i32 camId);
// End Camera


// 3d Game Objects
i32 AddProp(const char* path, i32 camId, SHADERTYPE shadertype);
void SetPropTranslation(i32 propId, vec3 new_pos);
void SetPropScale(i32 propId, vec3 new_scale);
void SetPropRotationX(i32 propId, f32 new_x_rot);
void SetPropRotationY(i32 propId, f32 new_y_rot);
void SetPropRotationZ(i32 propId, f32 new_z_rot);
// End 3D Game Objects


// Animated 3d Game Objects
//i32 AddAnimProp(const char* path, i32 camId, SHADERTYPE shadertype);  // a model that has bones and is ready to be animated
//void SetAnimation(i32 anim_prop_id, const char* path);     // sets the mAnimation on animprop from a mAnimation file that fits with anim prop
//void UpdateAnimation(i32 anim_prop_id, f32 dt);  // updates the current mAnimation that is set on animprop
// End Animated


// Skybox
void SetSkybox(const std::shared_ptr<Skybox>& skybox) noexcept;
// End Skybox


// Directional Light
void SetDirectionalLight(vec3 dir, vec3 amb, vec3 diff, vec3 spec);
void RemoveDirectionalLight();
// End Directional Light


// Point Light
i32 AddPointLight(vec3 pos, f32 constant, f32 linear, f32 quad, vec3 amb, vec3 diff, vec3 spec);
bool RemovePointLight(i32 which_by_id);
void MovePointLight(i32 which, vec3 new_pos);
void ChangePointLight(i32 which, vec3 new_pos, f32 new_constant, f32 new_linear, f32 new_quad,
  vec3 new_amb, vec3 new_diff, vec3 new_spec);
// End Point Light


// Spot Light
i32 AddSpotLight(vec3 pos, vec3 dir, f32 inner, f32 outer, f32 constant,
  f32 linear, f32 quad, vec3 amb, vec3 diff, vec3 spec);
bool RemoveSpotLight(i32 which_by_id);
void MoveSpotLight(i32 which, vec3 new_pos, vec3 new_dir);
void ChangeSpotLight(i32 which, vec3 new_pos, vec3 new_dir, f32 new_inner,
  f32 new_outer, f32 new_constant, f32 new_linear, f32 new_quad, vec3 new_amb,
  vec3 new_diff, vec3 new_spec);
// End Spot Light


// Sound Effects
i32 AddSoundEffect(const char* path);
void RemoveSoundEffect(i32 id);
void PlaySoundEffect(i32 id, bool i32errupt = true);
// End Sound Effects


// Music
void AddMusic(const char* path);
void RemoveMusic();
void PlayMusic();
void PauseMusic();
void ResumeMusic();
void StopMusic();
void SetMusicVolume(f32 new_vol);
// End Music


// Mouse
void SetMouseToHidden() noexcept;
void SetMouseToDisabled() noexcept;
void SetMouseToNormal() noexcept;
void SetMouseReadToFPP() noexcept;
void SetMouseFPPSensitivity(f32 sensitivity) noexcept;
f32 GetMouseFPPSensitivity() noexcept;
void SetMouseReadToNormal() noexcept;
// End Mouse


// GUI
void AddButton(vec2 pos, vec2 scale, vec3 color, float alpha);
void AddButton(vec2 pos, vec2 scale, float alpha, const char* texture_path = "");
void SetGUIVisibility(const bool value);
// End GUI


// imGUI
void UseIMGUI(const bool value);
// End imGUI


// Window
void SetWindowClearColor(vec3 color = vec3(.35f, .15f, .35f));
i32 GetWindowWidth() noexcept;
i32 GetWindowHeight() noexcept;
void SetWindowTitle(const char* name) noexcept;
// End Window


// Loop Controls
void SetTimedOutKeyHandlingLength(const f32& newtime);
void SetSlowUpdateTimeoutLength(const f32& newtime);
u32 AddToOnBegin(void(*function)());
u32 AddToDeltaUpdate(void(*function)(f32));
u32 AddToUpdate(void(*function)());
u32 AddToSlowUpdate(void(*function)());
u32 AddToTimedOutKeyHandling(bool(*function)(KeyboardInput&));
u32 AddToScrollHandling(void(*function)(ScrollInput&));
u32 AddToKeyHandling(void(*function)(KeyboardInput&));
u32 AddToMouseHandling(void(*function)(MouseInput&));
u32 AddToOnTeardown(void(*function)());
bool RemoveFromOnBegin(u32 r_id);
bool RemoveFromDeltaUpdate(u32 r_id);
bool RemoveFromUpdate(u32 r_id);
bool RemoveFromSlowUpdate(u32 r_id);
bool RemoveFromTimedOutKeyHandling(u32 r_id);
bool RemoveFromScrollHandling(u32 r_id);
bool RemoveFromKeyHandling(u32 r_id);
bool RemoveFromMouseHandling(u32 r_id);
bool RemoveFromTeardown(u32 r_id);
// End Loop Controls


}  // end namespace AA
