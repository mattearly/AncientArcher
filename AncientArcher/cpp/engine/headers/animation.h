#pragma once
#include <vector>
#include <glm/glm.hpp>
struct animation
{
  std::vector<float> timings;
  std::vector<glm::mat4> transformations;
};