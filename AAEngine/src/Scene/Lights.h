#pragma once
#include <glm/glm.hpp>

namespace AA
{
// todo: something better with these limitation holders

struct DirectionalLight {
  DirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
  glm::vec3 Direction, Ambient, Diffuse, Specular;
private:
  DirectionalLight() = delete;
};

struct PointLight {
  PointLight(glm::vec3 pos, float constant, float linear, float quad, glm::vec3 amb,
    glm::vec3 diff, glm::vec3 spec);

  glm::vec3 Position;
  float Constant, Linear, Quadratic;
  glm::vec3 Ambient, Diffuse, Specular;

  int id;
private:
  PointLight() = delete;
};

struct SpotLight {
  SpotLight(glm::vec3 pos, glm::vec3 dir, float inner, float outer, float constant,
    float linear, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
  glm::vec3 Position, Direction;
  float CutOff, OuterCutOff;
  float Constant, Linear, Quadratic;
  glm::vec3 Ambient, Diffuse, Specular;

  int id;

private:
  SpotLight() = delete;
};

}  // end namespace AA