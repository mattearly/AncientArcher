#pragma once

#include <Collider.h>
#include <vector>

class Collider;

enum ENTITYTYPE { BOX, PLANE, SPHERE };
class Entity {
private:
  struct GameItem {
    float location[3];
    float scale[3];
    int textureID;
    ENTITYTYPE type;
  };
public:
  GameItem gameItem;
  Collider *collider;
  Entity(ENTITYTYPE t, glm::vec3 loc, glm::vec3 sc, int texID, bool isImpassable);
  
  // MODIFIERS
  void moveBy(glm::vec3 amount);
  void moveTo(glm::vec3 newLocation);
};

