#include "Collision.h"

Collision & Collision::getInstance() {
  static Collision instance;
  return instance;
}

void Collision::addCollider(glm::vec3 loc, glm::vec3 sz) {
  impasse imp;
  imp.location[0] = loc.x;
  imp.location[1] = loc.y;
  imp.location[2] = loc.z;
  imp.size[0] = sz.x;
  imp.size[1] = sz.y;
  imp.size[2] = sz.z;
  blockings.push_back(imp);
}

void Collision::removeCollider(int vec_pos) {
  blockings.erase(blockings.begin() + vec_pos);
}

