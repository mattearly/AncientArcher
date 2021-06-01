#include "AnimVertex.h"
namespace AA {
AnimVertex::AnimVertex(vec3 pos, vec3 norms, vec2 texcoords, ivec4 boneids, vec4 weights) noexcept
  : Position(pos), Normal(norms), TexCoords(texcoords), BoneIds(boneids), Weights(weights) {}
}