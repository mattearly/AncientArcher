#pragma once
#include "../../src/Renderer/OpenGL/Skybox.h"
#include "../../src/Controls/KeyboardInput.h"
#include "../../src/Controls/MouseInput.h"
#include "../../src/Controls/ScrollInput.h"
#include "ShaderType.h"
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

namespace AA
{

// Init, Run, Shutdown, Reset
void InitEngine();
int Run();
void Shutdown() noexcept;
void SoftReset() noexcept;
// End Init, Run, Shutdown, Reset


// Camera
int AddCamera(int w, int h);
bool RemoveCamera(int camId);
void SetCamMaxRenderDistance(int camId, float amt);
void SetCamToPerspective(int camId);
void SetCamFOV(int camId, float new_fov);
void SetCamDimensions(int camId, int w, int h);
void SetCamPosition(int camId, glm::vec3 new_loc);
void SetCamPitch(int camId, float new_pitch_degrees);
void SetCamYaw(int camId, float new_yaw_degrees);
void ShiftCamPosition(int camId, glm::vec3 offset);
void ShiftCamPitchAndYaw(int camId, double pitch_offset, double yaw_offset);
glm::vec3 GetCamFront(int camId);
glm::vec3 GetCamRight(int camId);
glm::vec3 GetCamPosition(int camId);
float GetCamPitch(int camId);
float GetCamYaw(int camId);
glm::mat4 GetProjectionMatrix(int camId);
glm::mat4 GetOrthoMatrix(int camId);
// End Camera


// 3d Game Objects
int AddProp(const char* path, int camId, SHADERTYPE shadertype);
void SetPropTranslation(int propId, glm::vec3 new_pos);
void SetPropScale(int propId, glm::vec3 new_scale);
void SetPropRotationX(int propId, float new_x_rot);
void SetPropRotationY(int propId, float new_y_rot);
void SetPropRotationZ(int propId, float new_z_rot);
// End 3D Game Objects


// Skybox
void SetSkybox(const std::shared_ptr<Skybox>& skybox) noexcept;
// End Skybox


// Directional Light
void SetDirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
void RemoveDirectionalLight();
// End Directional Light


// Point Light
int AddPointLight(glm::vec3 pos, float constant, float linear, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
bool RemovePointLight(int which_by_id);
void MovePointLight(int which, glm::vec3 new_pos);
void ChangePointLight(int which, glm::vec3 new_pos, float new_constant, float new_linear, float new_quad,
  glm::vec3 new_amb, glm::vec3 new_diff, glm::vec3 new_spec);
// End Point Light


// Spot Light
int AddSpotLight(glm::vec3 pos, glm::vec3 dir, float inner, float outer, float constant,
  float linear, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
bool RemoveSpotLight(int which_by_id);
void MoveSpotLight(int which, glm::vec3 new_pos, glm::vec3 new_dir);
void ChangeSpotLight(int which, glm::vec3 new_pos, glm::vec3 new_dir, float new_inner,
  float new_outer, float new_constant, float new_linear, float new_quad, glm::vec3 new_amb,
  glm::vec3 new_diff, glm::vec3 new_spec);
// End Spot Light


// Sound Effects
int AddSoundEffect(const char* path);
void RemoveSoundEffect(int id);
void PlaySoundEffect(int id, bool interrupt = true);
// End Sound Effects


// Music
void AddMusic(const char* path);
void RemoveMusic();
void PlayMusic();
void StopMusic();
// End Music


// Mouse
void SetMouseToHidden() noexcept;
void SetMouseToDisabled() noexcept;
void SetMouseToNormal() noexcept;
void SetMouseReadToFPP() noexcept;
void SetMouseFPPSensitivity(float sensitivity) noexcept;
float GetMouseFPPSensitivity() noexcept;
void SetMouseReadToNormal() noexcept;
void SetScrollWheelCallback() noexcept;
// End Mouse


// Window
void SetWindowClearColor(glm::vec3 color = glm::vec3(.35f, .15f, .35f));
int GetWindowWidth() noexcept;
int GetWindowHeight() noexcept;
void SetReshapeCallback() noexcept;
void SetWindowTitle(const char* name) noexcept;
// End Window

// Interface
void AddInterfaceSlider(vec2 bot_left_loc, float low_val, float high_val, float bar_thickness, float bar_length, bool horizontal, void(*returnvaluehandler)(float result));
// End Interface


// Loop Controls
void SetSlowUpdateTimeoutLength(const float& newtime);
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
// End Loop Controls


}  // end namespace AA
