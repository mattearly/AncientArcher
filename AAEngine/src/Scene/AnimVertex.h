#pragma once
#include "../../include/AncientArcher/Types.h"
namespace AA {
struct AnimVertex {
  AnimVertex(vec3 pos, vec3 norms, vec2 texcoords, ivec4 boneids, vec4 weights) noexcept;
  vec3 Position;
  vec3 Normal;
  vec2 TexCoords;
  ivec4 BoneIds;  // 4 boneids per vertex
  vec4 Weights;   // weights for each boneid
};
}