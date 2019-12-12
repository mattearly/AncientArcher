#pragma once
#include <glm/glm.hpp>

struct DirectionalLight
{
  glm::vec3 Direction, Ambient, Diffuse, Specular;
};

struct PointLight
{
  glm::vec3 Position;
  float Constant, Linear, Quadratic;
  glm::vec3 Ambient, Diffuse, Specular;
};

struct SpotLight
{
  glm::vec3 Position, Direction;
  float CutOff, OuterCutOff;
  float Constant, Linear, Quadratic;
  glm::vec3 Ambient, Diffuse, Specular;
};
