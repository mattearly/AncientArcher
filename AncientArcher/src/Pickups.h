#pragma once
#include<glm/glm.hpp>
#include<mearly.h>
class Pickups {
public:
  Pickups();
  ~Pickups();

  bool speedBoostAvail;
  glm::vec2 boostLoc;
};

