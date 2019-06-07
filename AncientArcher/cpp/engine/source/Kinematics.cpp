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
}

glm::vec3 Kinematics::getCalculatedPosition(float speed_stat, float weight, float delta_time)
{
	return glm::vec3(
		(velocity[0] + acceleration[0] + jerk[0]),
		(velocity[1] + acceleration[1] + jerk[1]),
		(velocity[2] + acceleration[2] + jerk[2])
	);
}
