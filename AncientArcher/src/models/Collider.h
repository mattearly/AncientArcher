#pragma once
#include <glm/glm.hpp>
#include "../util/mearly.h"

class Collider
{
private:
public:
  mearly::Impasse impasse;
  Collider(glm::vec3 loc, glm::vec3 sz);
};

