#pragma once
#include "ColliderBox.h"
namespace mearly 
{
class CollisionHandler
{
public:
  static bool AABB_vs_AABB_3D(const ColliderBox& a, const ColliderBox& b);
  static bool point_vs_AABB_3D(const float& px, const float& py, const float& pz, const ColliderBox& b);
  static bool point_vs_AABB_3D(glm::vec3 pt, const ColliderBox& b);
  static bool vector_vs_AABB_3D(glm::vec3 startpt, glm::vec3 endpt, const ColliderBox& b);

private:

};
} // end namespace meraly

