#pragma once
#include "../../include/AncientArcher/Types.h"
namespace AA {

struct Vertex {
  Vertex(vec3 pos, vec3 norm);
  Vertex(vec3 pos, vec3 norm, vec2 uv);
  vec3 Position{};
  vec3 Normal{};
  vec2 TexCoords{};
  vec3 Tangent{};
  vec3 BiTangent{};
  ivec4 BoneIds{};
  vec4 Weights{};
};


} // end namespace AA
