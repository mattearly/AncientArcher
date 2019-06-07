#include <Kinematics.h>

Kinematics::Kinematics()
{
	velocity[0] = 0.0f;
	velocity[1] = 0.0f;
	velocity[2] = 0.0f;
	acceleration[0] = 0.0f;
	acceleration[1] = 0.0f;
	acceleration[2] = 0.0f;
	jerk[0] = 0.0f;
	jerk[1] = 0.0f;
	jerk[2] = 0.0f;

	//For rotational motion (rotational inertia, etc.)
	angular_velocity[0] = 0.0f;
	angular_velocity[1] = 0.0f;
	angular_velocity[2] = 0.0f;
	angular_acceleration[0] = 0.0f;
	angular_acceleration[1] = 0.0f;
	angular_acceleration[2] = 0.0f;
	angular_jerk[0] = 0.0f;
	angular_jerk[1] = 0.0f;
	angular_jerk[2] = 0.0f;

	speed = 7.0f;
	weight = 175.0f;
}

Kinematics::Kinematics(float speedStat, float weightStat)
{
	velocity[0] = 0.0f;
	velocity[1] = 0.0f;
	velocity[2] = 0.0f;
	acceleration[0] = 1.0f / weightStat;
	acceleration[1] = 0.0f;
	acceleration[2] = 0.0f;
	jerk[0] = 0.0f;
	jerk[1] = 0.0f;
	jerk[2] = 0.0f;

	//For rotational motion (rotational inertia, etc.)
	angular_velocity[0] = 0.0f;
	angular_velocity[1] = 0.0f;
	angular_velocity[2] = 0.0f;
	angular_acceleration[0] = 0.0f;
	angular_acceleration[1] = 0.0f;
	angular_acceleration[2] = 0.0f;
	angular_jerk[0] = 0.0f;
	angular_jerk[1] = 0.0f;
	angular_jerk[2] = 0.0f;

	speed = speedStat;
	weight = weightStat;
}

glm::vec3 Kinematics::getCalculatedPosition(float deltaTime, bool forward, bool backward, bool direction)
{

	if (forward || backward)
	{
		if (velocity[0] < speed) {
			velocity[0] += acceleration[0];
		}
	}
	else {
		if (velocity[0] > 0.0f) {
			velocity[0] -= acceleration[0];
		}
	}

	float temp_vel = velocity[0];

	if (direction) {
		temp_vel = -temp_vel;
	}

	return glm::vec3(
		(temp_vel * deltaTime),
		(velocity[1] + acceleration[1] + jerk[1]),
		(velocity[2] + acceleration[2] + jerk[2])
	);
}
