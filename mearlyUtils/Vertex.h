#pragma once
#include <glm\ext\vector_float4.hpp>
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
namespace mearly {
struct Vertex
{
  Vertex(glm::vec4 pos, glm::vec4 norms, glm::vec2 texcoords) noexcept;
  Vertex(glm::vec4 pos, glm::vec4 norms, glm::vec4 colors, glm::vec2 texcoords) noexcept;
  glm::vec4 Position;
  glm::vec4 Normal;
  glm::vec4 Color;
  glm::vec2 TexCoords;
};
} // end namespace mearly
