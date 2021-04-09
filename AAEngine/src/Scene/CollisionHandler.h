#pragma once
#include "ColliderBox.h"
#include "ColliderSphere.h"
namespace AA
{
class CollisionHandler
{
public:
  static CollisionHandler* Get();

  bool AABB_vs_AABB_3D(const ColliderBox& a, const ColliderBox& b);
  bool point_vs_AABB_3D(const glm::vec3& pt, const ColliderBox& b);
  bool line_vs_AABB_3D(const glm::vec3& startpt, const glm::vec3& endpt, const ColliderBox& b);

  bool sphere_vs_AABB_3D(const ColliderSphere& sphere, const ColliderBox& box);

  bool sphere_vs_Sphere_3D(const ColliderSphere* sphere, const ColliderSphere* other);
  bool point_vs_Sphere_3D(const glm::vec3& point, const ColliderSphere& sphere);
  bool line_vs_Sphere_3D(const glm::vec3& startpoint, const glm::vec3& endpoint, const ColliderSphere& sphere);

private:
  CollisionHandler();
};
} // end namespace AA
