#pragma once
#include <glm/glm.hpp>
#include "mearly.h"

class Collider
{
private:
public:
  mearly::Impasse impasse;
  Collider();
  Collider(glm::vec3 loc, glm::vec3 sz);
};

