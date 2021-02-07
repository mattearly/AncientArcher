#pragma once
#include <glm/glm.hpp>

namespace AA
{
// todo: something better with these limitation holders

const int MAXPOINTLIGHTS = 50;
const int MAXSPOTLIGHTS = 25;
static int NUM_POINT_LIGHTS = 0;
static int NUM_SPOT_LIGHTS = 0;

struct DirectionalLight {
	glm::vec3 Direction, Ambient, Diffuse, Specular;
};

struct PointLight {
	glm::vec3 Position;
	float Constant, Linear, Quadratic;
	glm::vec3 Ambient, Diffuse, Specular;
};

struct SpotLight {
	glm::vec3 Position, Direction;
	float CutOff, OuterCutOff;
	float Constant, Linear, Quadratic;
	glm::vec3 Ambient, Diffuse, Specular;
};

}  // end namespace AA