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

//Kinematics::Kinematics(glm::vec3 loc, glm::vec3 sz)
//{
//	loc[0] = loc.x;
//	loc[1] = loc.y;
//	loc[2] = loc.z;
//	sz[0] = sz.x;
//	sz[1] = sz.y;
//	sz[2] = sz.z;
//}
