#include "MeshDrawInfo.h"
namespace AA {

MeshDrawInfo::MeshDrawInfo(u32 a, u32 elcount, std::unordered_map<u32, string> t_id, f32 shine, mat4 trans) :
  vao(a), numElements(elcount), textureDrawIds(t_id), shininess(shine), transformation(trans) {}

MeshDrawInfo::MeshDrawInfo(u32 a, u32 elcount, std::unordered_map<u32, string> t_id, f32 shine, mat4 trans,
  VertexBoneData vbd) :
  vao(a), numElements(elcount), textureDrawIds(t_id), shininess(shine), transformation(trans), boneData(vbd) {}
}  // end namespace AA