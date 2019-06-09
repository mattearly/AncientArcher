#include <Entity.h>

Entity::Entity(ENTITYTYPE t, glm::vec3 loc, glm::vec3 sc, int texID, bool isImpassable, bool hasKinematics)
{
  gameItem.type = t;
  gameItem.loc = loc;
  gameItem.scale[0] = sc.x;
  gameItem.scale[1] = sc.y;
  gameItem.scale[2] = sc.z;
  gameItem.textureID = texID;
  if (isImpassable) {
    collider = new Collider(loc, sc);
  }
  else {
    collider = nullptr;
  }
  if (hasKinematics) {
	  kinematics = new Kinematics(7.0f, 175.0f);
  }
  else {
	  kinematics = nullptr;
  }
}

void Entity::moveBy(glm::vec3 amount)
{
  
  this->gameItem.loc += amount;

  if (collider != nullptr) {
    this->gameItem.loc += amount;
  }

}

void Entity::moveTo(glm::vec3 newLocation)
{
  this->gameItem.loc = newLocation;
  if (collider != nullptr) {
    this->collider->impasse.loc = newLocation;
  }
}


