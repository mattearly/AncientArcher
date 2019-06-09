#pragma once
#include <glm/glm.hpp>

class Kinematics
{
private:
public:

  glm::vec3 vel; // Velocity of the moving body (position increased per second)
  glm::vec3 acc; // Acceleration of the moving body (velocity increased per second)
  glm::vec3 jrk; // Jerk of the moving body (acceleration increased per second)

  //For rotational motion (rotational inertia, etc.)

  glm::vec3 angVel; // Angular velocity of the moving body (angular position increased per second)
  glm::vec3 angAcc; // Angular acceleration of the moving body (angular velocity increased per second)
  glm::vec3 angJrk; // Angular jerk of the moving body (angular acceleration increased per second)

  float speed; // Kinematic body max speed
  float weight; // Kinematic body weight

  Kinematics(); // Constructor
  Kinematics(float speedStat, float weightStat); // Constructor for setting custom body 
  glm::vec3 getCalculatedPosition(float deltaTime, bool f, bool b, bool u, bool d, bool l, bool r);

};