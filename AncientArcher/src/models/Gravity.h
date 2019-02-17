#pragma once
#include <glm\glm.hpp>
class Entity;
class Gravity
{
public:
  void apply(Entity &e, float deltaTime);
private:
  const float POWER = -9.810f;
  const glm::vec3 DIRECTION = { 0.0f, -1.0f, 0.0f };
};

