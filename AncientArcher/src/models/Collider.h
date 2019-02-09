#pragma once
#include <glm/glm.hpp>
class Collider
{
private:
  struct Impasse {
    float location[3];
    float size[3];
  };
public:
  Impasse impasse;
  Collider();
  Collider(glm::vec3 loc, glm::vec3 sz);
  ~Collider();
};

