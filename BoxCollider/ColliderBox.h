#pragma once
#include <glm/glm.hpp>
namespace mearly
{
struct ColliderBox
{
  ColliderBox(glm::vec3 lc, glm::vec3 sz);
  ColliderBox();
  glm::vec3 prevLoc;
  glm::vec3 loc;
  glm::vec3 size;
};
} // end namespace mearly
