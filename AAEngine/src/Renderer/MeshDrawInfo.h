#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <unordered_map>
#include "../../include/AncientArcher/Types.h"
namespace AA {
struct MeshDrawInfo   // NodeDrawInfo a better name?
{
  u32 vao = 0;
  std::unordered_map<u32, std::string> textureDrawIds; // list of texture ids(u32) and type(string) in mem
  float shininess;
  u32 numElements;
  mat4 transformation;
  MeshDrawInfo(u32 a, u32 elcount, std::unordered_map<u32, string> t_id, f32 shine, mat4 trans);
};
}  // end namespace AA
