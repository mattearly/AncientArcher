#pragma once
#include <glm/glm.hpp>

class Kinematics
{
private:
public:
	float velocity[3];
	float acceleration[3];
	float jerk[3];

	//For rotational motion (rotational inertia, etc.)
	float angular_velocity[3];
	float angular_acceleration[3];
	float angular_jerk[3];

	float speed;
	float weight;

	Kinematics();
	Kinematics(float speedStat, float weightStat);
	glm::vec3 getCalculatedPosition(float deltaTime);
};