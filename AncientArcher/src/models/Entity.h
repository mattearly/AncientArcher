#pragma once
#include <vector>
#include "Collider.h"
class Collider;
class Entity {
private:
  struct SquareItem {
    float location[3];
    float scale[3];
    int textureID;
  };
public:
  SquareItem squareItem;
  Collider *collider;
  Entity();
  Entity(glm::vec3 loc, glm::vec3 sc, int texID, bool isImpassable);
  ~Entity();
};

