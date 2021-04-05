#pragma once
#include "../Base/UniqueInstance.h"
#include <glm/glm.hpp>
#include <memory>

namespace AA
{
enum class RenderProjection { ORTHO, PERSPECTIVE };
class Camera : public UniqueInstance
{
public:
	Camera(int width, int height);
	int              Width;
	int              Height;
	glm::vec3        Front;
	glm::vec3        Right;
	glm::vec3        Up;
	glm::vec3        Position;
	float            FOV;
	float            Yaw;
	float            Pitch;
	float            MaxRenderDistance;
	RenderProjection RenderProjection;
	glm::mat4        Projection;
	glm::mat4        View;
	glm::vec2        TopLeftPositionOnScreen;
	glm::vec2        RatioToScreen;
	void updateCameraVectors();
	void updateProjectionMatrix();
	void updateViewMatrix();
private:
	void resetViewportVars();
};
} // end namespace AA
