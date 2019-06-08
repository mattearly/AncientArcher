#include <Kinematics.h>

Kinematics::Kinematics()
{
	vel = glm::vec3(0.0f, 0.0f, 0.0f);
	acc = glm::vec3(0.0f, 0.0f, 0.0f);
	jrk = glm::vec3(0.0f, 0.0f, 0.0f);

	//For rotational motion (rotational inertia, etc.)
	angVel = glm::vec3(0.0f, 0.0f, 0.0f);
	angAcc = glm::vec3(0.0f, 0.0f, 0.0f);
	angJrk = glm::vec3(0.0f, 0.0f, 0.0f);

	speed = 7.0f;
	weight = 175.0f;
}

Kinematics::Kinematics(float speedStat, float weightStat)
{
	vel = glm::vec3(0.0f, 0.0f, 0.0f);
	acc = glm::vec3(1.0f / weightStat, 0.0f, 0.0f);
	jrk = glm::vec3(0.0f, 0.0f, 0.0f);

	//For rotational motion (rotational inertia, etc.)
	angVel = glm::vec3(0.0f, 0.0f, 0.0f);
	angAcc = glm::vec3(0.0f, 0.0f, 0.0f);
	angJrk = glm::vec3(0.0f, 0.0f, 0.0f);

	speed = speedStat;
	weight = weightStat;
}

glm::vec3 Kinematics::getCalculatedPosition(float deltaTime, bool forward, bool backward, bool direction)
{
	if (forward || backward)
	{
		if (vel.x < speed) {
			acc.x += jrk.x;
			vel.x += acc.x;
		}
	}
	else {
		if (vel.x > 0.0f) {
			acc.x -= jrk.x;
			vel.x -= acc.x;
		}
	}

	float temp_vel = vel.x;

	if (direction) {
		temp_vel = -temp_vel;
	}

	return glm::vec3(
		(temp_vel * deltaTime),
		(vel.y + acc.y + jrk.y),
		(vel.z + acc.z + jrk.z)
	);
}
