#pragma once
#include <Collider.h>
#include <Kinematics.h>
#include <vector>

enum ENTITYTYPE { CUBE, PLANE, SPHERE, MODEL };

class Entity {
private:
  struct GameItem {
    glm::vec3 prevLoc;
    glm::vec3 loc;
    glm::vec3 scale;
    int textureID;
    ENTITYTYPE type;
  };
public:
  GameItem gameItem;
  Collider* collider;
  Kinematics* kinematics;
  Entity(ENTITYTYPE t, glm::vec3 loc, glm::vec3 sc,
    int texID, bool isImpassable, bool hasKinematics);

  // MODIFIERS
  void moveBy(glm::vec3 amount);
  void moveTo(glm::vec3 newLocation);
  void syncLocation();

  void growBy(glm::vec3 scaleAmount);

};

