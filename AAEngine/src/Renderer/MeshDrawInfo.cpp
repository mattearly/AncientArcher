#include "MeshDrawInfo.h"
namespace AA
{
MeshDrawInfo::MeshDrawInfo(uint32_t a, uint32_t elcount, std::unordered_map<uint32_t, std::string> t_id, float shine, glm::mat4 trans) :
  vao(a), numElements(elcount), textureDrawIds(t_id), shininess(shine), transformation(trans)
{
}
}  // end namespace AA