#pragma once
#include <glm\ext\vector_float3.hpp>
struct ColliderSphere
{
	ColliderSphere(glm::vec3 loc, float r);
	ColliderSphere();
	glm::vec3 center;
	float radius;
};

