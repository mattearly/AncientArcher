#pragma once
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\matrix_float4x4.hpp>
#include "ColliderSphere.h"
namespace AA
{
struct InstanceDetails
{
	void calcModelMatrix();

	InstanceDetails();
	InstanceDetails(glm::vec3 scale, glm::vec3 rot, glm::vec3 transl);

	// UpdateModelMatrix uses these
	glm::vec3 Scale = glm::vec3(1);
	glm::vec3 Rotation = glm::vec3(0); // radians on each of xyz
	glm::vec3 Translate = glm::vec3(0);

	// final result
	glm::mat4 ModelMatrix = glm::mat4(1);

	// for if the unit has collision
	ColliderSphere* mColliderSphere = nullptr;
};
}  // end namespace AA
