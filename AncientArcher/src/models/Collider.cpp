#include "Collider.h"

Collider::Collider()
{
}

Collider::Collider(glm::vec3 loc, glm::vec3 sz)
{
  impasse.location[0] = loc.x;
  impasse.location[1] = loc.y;
  impasse.location[2] = loc.z; 
  impasse.size[0] = sz.x + COLLISION_EXPANDER;
  impasse.size[1] = sz.y + COLLISION_EXPANDER;
  impasse.size[2] = sz.z + COLLISION_EXPANDER;
}


Collider::~Collider()
{
}
