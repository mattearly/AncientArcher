#include <Collider.h>
//
//struct Impasse {
//  Impasse(glm::vec3 lc, glm::vec3 sz) { loc = lc; size = sz; }
//  glm::vec3 loc;
//  glm::vec3 size;
//};

class CollisionHandler
{
public:
  bool AABB_vs_AABB_3D(const Impasse& a, const Impasse& b) const;
  bool point_vs_AABB_3D(const float& px, const float& py, const float& pz, const Impasse& b);
private:

};
