#include <Collider.h>
Collider::Collider()
{
  impasse.loc = impasse.prevLoc = glm::vec3(0.f);
  impasse.size = glm::vec3(1.f);
}

Collider::Collider(glm::vec3 lc, glm::vec3 sz)
{
  impasse.loc = impasse.prevLoc = lc;
  impasse.size = sz;
}
