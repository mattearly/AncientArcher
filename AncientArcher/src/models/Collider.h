#pragma once
#include <glm/glm.hpp>
struct Impasse {
  float location[3];
  float size[3];
};
class Collider
{
private:
  const float COLLISION_EXPANDER = .3f;
public:
  Impasse impasse;
  Collider(glm::vec3 loc, glm::vec3 sz);
};

