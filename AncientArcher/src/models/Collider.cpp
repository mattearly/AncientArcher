#include "Collider.h"


Collider::Collider(glm::vec3 loc, glm::vec3 sz)
{
  impasse.loc[0] = loc.x;
  impasse.loc[1] = loc.y;
  impasse.loc[2] = loc.z;
  impasse.sz[0] = sz.x + COLLISION_EXPANDER;
  impasse.sz[1] = sz.y + COLLISION_EXPANDER;
  impasse.sz[2] = sz.z + COLLISION_EXPANDER;
}


