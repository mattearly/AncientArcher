#pragma once
#include <glm\ext\vector_float3.hpp>
struct InstanceDetails
{
  void updateModelMatrix();

  InstanceDetails();
  InstanceDetails(glm::vec3 scale, glm::vec3 rotAx, glm::vec3 transl, float rotAng);
  // updateModelMatrix uses these
  glm::vec3 Scale = glm::vec3(1);
  glm::vec3 RotationAxis = glm::vec3(1);
  glm::vec3 Translate = glm::vec3(0);
  float RotationAngle = 0.f;

  glm::mat4 ModelMatrix = glm::mat4(1);
};
