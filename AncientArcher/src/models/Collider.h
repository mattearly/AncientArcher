#pragma once
#include <glm/glm.hpp>
#include "../util/mearly.h"

class Collider
{
private:
  const float COLLISION_EXPANDER = .3f;
public:
  mearly::Impasse impasse;
  Collider(glm::vec3 loc, glm::vec3 sz);
};

