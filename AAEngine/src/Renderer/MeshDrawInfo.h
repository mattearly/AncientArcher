#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <unordered_map>
#include "../../include/AncientArcher/Types.h"
namespace AA {

const u32 NUM_BONES_PER_VEREX = 4;

struct VertexBoneData {
  u32 IDs[NUM_BONES_PER_VEREX];
  float Weights[NUM_BONES_PER_VEREX];
};

struct MeshDrawInfo   // NodeDrawInfo a better name?
{
  u32 vao = 0;
  std::unordered_map<u32, std::string> textureDrawIds; // list of texture ids(u32) and type(string) in mem
  float shininess;
  u32 numElements;
  mat4 transformation;
  VertexBoneData boneData;
  MeshDrawInfo(u32 a, u32 elcount, std::unordered_map<u32, string> t_id, f32 shine, mat4 trans);
  MeshDrawInfo(u32 a, u32 elcount, std::unordered_map<u32, string> t_id, f32 shine, mat4 trans, VertexBoneData vbd);
};
}  // end namespace AA
