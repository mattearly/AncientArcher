#include "ColliderBox.h"
namespace mearly
{
ColliderBox::ColliderBox()
{
  loc = prevLoc = glm::vec3(0.f);
  size = glm::vec3(1.f);
}

ColliderBox::ColliderBox(glm::vec3 lc, glm::vec3 sz)
{
  loc = prevLoc = lc;
  size = sz;
}
} // end namespace mearly