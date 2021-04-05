#pragma once
#include "../../src/Renderer/OpenGL/Skybox.h"
#include "../../src/Sound/ShortSound.h"
#include "../../src/Sound/LongSound.h"
#include "../../src/Controls/KeyboardInput.h"
#include "../../src/Controls/MouseInput.h"
#include "../../src/Controls/ScrollInput.h"
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

struct GLFWwindow;
class OGLShader;
class Settings;
enum class SHADERTYPE;
struct InstanceDetails;

namespace AA
{
void InitEngine();
int Run();
void Shutdown() noexcept;
void SoftReset();

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
void ShiftCamPitchAndYaw(int camId, float pitch_offset, float yaw_offset);
glm::vec3 GetCamFront(int camId);
glm::vec3 GetCamRight(int camId);
glm::vec3 GetCamPosition(int camId);
glm::mat4 GetProjectionMatrix(int camId);
// End Camera

// 3d Game Objects
int AddProp(const char* path, int camId, bool is_lit);
void SetPropTranslation(int propId, glm::vec3 new_pos);
void SetPropScale(int propId, glm::vec3 new_scale);
void SetPropRotationY(int propId, float new_y_rot);


void SetDirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
void RemoveDirectionalLight();
int AddPointLight(glm::vec3 pos, float constant, float linear, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
void MovePointLight(int which, glm::vec3 new_pos);
void ChangePointLight(int which, glm::vec3 new_pos, float new_constant, float new_linear, float new_quad,
	glm::vec3 new_amb, glm::vec3 new_diff, glm::vec3 new_spec);
bool RemovePointLight(int which_by_id);
int AddSpotLight(glm::vec3 pos, glm::vec3 dir, float inner, float outer, float constant,
	float linear, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
void MoveSpotLight(int which, glm::vec3 new_pos, glm::vec3 new_dir);
void ChangeSpotLight(int which, glm::vec3 new_pos, glm::vec3 new_dir, float new_inner,
	float new_outer, float new_constant, float new_linear, float new_quad, glm::vec3 new_amb,
	glm::vec3 new_diff, glm::vec3 new_spec);
bool RemoveSpotLight(int which_by_id);
int AddSpeaker();
int AddSoundEffect(const char* path);
void RemoveSoundEffect(int effect_id);
void ChangeMusic(const char* path);
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
void SetCursorToEnabled(bool isHardwareRendered = false);
ShortSound& GetSpeaker(int speaker_id);
LongSound& GetMusic();
void PlaySoundEffect(int effect_id, int speaker_id);
int GetWindowWidth() noexcept;
int GetWindowHeight() noexcept;
GLFWwindow* GetWindow() noexcept;
enum class MouseReporting { STANDARD, PERSPECTIVE };
void SetClearColor(glm::vec3 color = glm::vec3(.35f, .15f, .35f));
void SetCursorToVisible() noexcept;   // to use the os pointer
void SetCursorToHidden() noexcept;    // for hidden but still there (render your own pointer)
void SetCursorToDisabled() noexcept;  // for first person hidden mouse type
void SetReadMouseCurorAsFPP() noexcept;
void SetReadMouseCurorAsStandard() noexcept;
void SetWindowTitle(const char* name) noexcept;
void ReshapeWindowHandler(int width, int height);
void SetupReshapeCallback() noexcept;
float GetMouseFPPSensitivity() noexcept;
void SetMouseFPPSensitivity(float sensitivity) noexcept;
void PerspectiveMouseHandler(float xpos, float ypos);
void StandardMouseHandler(float xpos, float ypos);
void ScrollHandler(float xpos, float ypos);

}  // end namespace AA
