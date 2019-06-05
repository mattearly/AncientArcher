#pragma once

#include <Collider.h>
#include <Kinematics.h>
#include <vector>

class Collider;

enum ENTITYTYPE { CUBE, PLANE, SPHERE };
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
  Collider* collider;
  Kinematics* kinematics;
  Entity(ENTITYTYPE t, glm::vec3 loc, glm::vec3 sc, int texID, bool isImpassable, bool hasKinematics);

  // MODIFIERS
  void moveBy(glm::vec3 amount);
  void moveTo(glm::vec3 newLocation);
};

