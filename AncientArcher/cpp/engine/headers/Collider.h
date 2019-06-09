#pragma once
#include <glm/glm.hpp>
struct Impasse {
  glm::vec3 prevLoc;
  glm::vec3 loc;
  glm::vec3 size;
};

class Collider
{
public:

  Impasse impasse;
  Collider();
  Collider(glm::vec3 loc, glm::vec3 sz);
private:
};

