#pragma once
#include <glm/glm.hpp>
struct Impasse {
  //Impasse(glm::vec3 lc, glm::vec3 sz) { loc = lc; size = sz; }
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

