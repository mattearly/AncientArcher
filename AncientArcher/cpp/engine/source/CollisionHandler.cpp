#include <CollisionHandler.h>

/**
 * Bounding Box Collision for 2 3D rectagular Boxes
 * @param[in] a  An impassible collider blockage, A.
 * @param[in] b  An impassible collider blockage, B.
 * @return       True if they collide. False if they do not collide.
 */
bool CollisionHandler::AABB_vs_AABB_3D(const Impasse& a, const Impasse& b) const
{
  float aMinX = a.loc.x - a.size.x / 2.f;
  float aMaxX = a.loc.x + a.size.x / 2.f;
  float aMinY = a.loc.y - a.size.y / 2.f;
  float aMaxY = a.loc.y + a.size.y / 2.f;
  float aMinZ = a.loc.z - a.size.z / 2.f;
  float aMaxZ = a.loc.z + a.size.z / 2.f;

  float bMinX = b.loc.x - b.size.x / 2.f;
  float bMaxX = b.loc.x + b.size.x / 2.f;
  float bMinY = b.loc.y - b.size.y / 2.f;
  float bMaxY = b.loc.y + b.size.y / 2.f;
  float bMinZ = b.loc.z - b.size.z / 2.f;
  float bMaxZ = b.loc.z + b.size.z / 2.f;

  return (aMinX <= bMaxX && aMaxX >= bMinX) && (aMinY <= bMaxY && aMaxY >= bMinY) && (aMinZ <= bMaxZ && aMaxZ >= bMinZ);
}

/**
 * Bouding Box Collision for a point v.s. a 3D rectangular Box.
 * @return        True if they collide. False if they do not collide
 */
bool CollisionHandler::point_vs_AABB_3D(const float& px, const float& py, const float& pz, const Impasse& b) const
{
  float bMinX = b.loc.x - b.size.x / 2.f;
  float bMaxX = b.loc.x + b.size.x / 2.f;
  float bMinY = b.loc.y - b.size.y / 2.f;
  float bMaxY = b.loc.y + b.size.y / 2.f;
  float bMinZ = b.loc.z - b.size.z / 2.f;
  float bMaxZ = b.loc.z + b.size.z / 2.f;

  return (px >= bMinX && px <= bMaxX) && (py >= bMinY && py <= bMaxY) && (pz >= bMinZ && pz <= bMaxZ);
}

