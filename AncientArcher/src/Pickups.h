#pragma once
#include<glm/glm.hpp>

class PrimativeManager;

class Pickups {

public:
  Pickups();
  ~Pickups();

  bool speedBoostAvail;
  glm::vec2 speedBoostLoc;

  bool attackBoostAvail;
  glm::vec2 attackBoostLoc;

  void respawn();

  void draw(PrimativeManager *primativeManager);

};
