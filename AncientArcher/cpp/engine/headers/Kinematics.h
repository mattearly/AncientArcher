#pragma once
#include <glm/glm.hpp>

class Kinematics
{
private:
public:
	float velocity[3];
	float accelaration[3];
	float jerk[3];

	Kinematics();
	//Kinematics(glm::vec3 loc, glm::vec3 sz);
};