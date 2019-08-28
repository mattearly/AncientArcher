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
  acc = glm::vec3(0.01f / weightStat, 1.f - 1.f / weightStat, 0.0f);
  jrk = glm::vec3(0.0f, 0.0f, 0.0f);

  //For rotational motion (rotational inertia, etc.)
  angVel = glm::vec3(0.0f, 0.0f, 0.0f);
  angAcc = glm::vec3(0.0f, 0.0f, 0.0f);
  angJrk = glm::vec3(0.0f, 0.0f, 0.0f);

  speed = speedStat;
  weight = weightStat;
}

glm::vec3 Kinematics::getCalculatedPosition(float deltaTime, bool f, bool b, bool u, bool d, bool l, bool r)
{
  if (f || b)
  {
    if (vel.x < speed) 
    {
      acc.x += jrk.x;
      vel.x += acc.x;
    }
  }
  else 
  {
    if (vel.x > 0.0f) 
    {
      acc.x -= jrk.x;
      vel.x -= acc.x;
    }
  }

  if (u || d)
  {
    if (vel.y < speed) 
    {
      acc.y += jrk.y;
      vel.y += acc.y;
    }
  }
  else 
  {
    if (vel.y > 0.0f)
    {
      acc.y -= jrk.y;
      vel.y -= acc.y;
    }
  }

  if (l || r)
  {
    if (vel.z < speed) 
    {
      acc.z += jrk.z;
      vel.z += acc.z;
    }
  }
  else
  {
    if (vel.z > 0.0f)
    {
      acc.z -= jrk.z;
      vel.z -= acc.z;
    }
  }

  glm::vec3 temp_vel = vel;

  if (b) 
  {
    temp_vel.x = -temp_vel.x;
  }
  if (d) 
  {
    temp_vel.y = -temp_vel.y;
  }
  if (l) 
  {
    temp_vel.z = -temp_vel.z;
  }

  return glm::vec3(
    (temp_vel.x * deltaTime),
    (temp_vel.y * deltaTime),
    (temp_vel.z * deltaTime)
  );
}
