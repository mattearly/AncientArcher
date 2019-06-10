#pragma once
#include <Collider.h>
#include <glm/glm.hpp>

class CollisionHandler
{
public:
  bool AABB_vs_AABB_3D(const Impasse& a, const Impasse& b) const;
  bool point_vs_AABB_3D(const float& px, const float& py, const float& pz, const Impasse& b) const;
  bool point_vs_AABB_3D(glm::vec3 pt, const Impasse& b) const;
private:

};
