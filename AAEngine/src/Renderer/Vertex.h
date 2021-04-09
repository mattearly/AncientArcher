#pragma once
#include <glm\ext\vector_float4.hpp>
#include <glm\ext\vector_float3.hpp>
#include <glm\ext\vector_float2.hpp>
namespace AA
{
struct Vertex
{
  Vertex(glm::vec3 pos, glm::vec2 texcoords, glm::vec3 norms) noexcept;
  //Vertex(glm::vec4 pos, glm::vec4 norms, glm::vec4 colors, glm::vec2 texcoords) noexcept;
  glm::vec3 Position;
  glm::vec2 TexCoords;
  glm::vec3 Normal;
  //glm::vec4 Color;
};
} // end namespace AA
