#include "Entity.h"



Entity::Entity() {}

Entity::Entity(glm::vec3 loc, glm::vec3 sc, int texID, bool isImpassable)
{
  squareItem.location[0] = loc.x;
  squareItem.location[1] = loc.y;
  squareItem.location[2] = loc.z;
  squareItem.scale[0] = sc.x;
  squareItem.scale[1] = sc.y;
  squareItem.scale[2] = sc.z;
  squareItem.textureID = texID;
  if (isImpassable) {
    collider = new Collider(loc, sc);
  }
  else {
    collider = nullptr;
  }
}


Entity::~Entity() {}
