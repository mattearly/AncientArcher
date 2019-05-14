#include <Collider.h>

Collider::Collider()
{
  impasse.loc[0] = 0.0f;
  impasse.loc[1] = 0.0f;
  impasse.loc[2] = 0.0f;
  impasse.sz[0] = 1.0f;
  impasse.sz[1] = 1.0f;
  impasse.sz[2] = 1.0f;
}

Collider::Collider(glm::vec3 loc, glm::vec3 sz)
{
  impasse.loc[0] = loc.x;
  impasse.loc[1] = loc.y;
  impasse.loc[2] = loc.z;
  impasse.sz[0] = sz.x;
  impasse.sz[1] = sz.y;
  impasse.sz[2] = sz.z;
}
