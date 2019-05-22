#pragma once
#include <Shader.h>
#include <glm/glm.hpp>
#include <vector>

class Lighting {
public:

  // --- Directional Constant Lights --- //
  void setConstantLight(Shader* shader);
  void updateConstantLightDirection(glm::vec3 changedVar);
  void updateConstantLightAmbient(glm::vec3 changedVar);
  void updateConstantLightDiffuse(glm::vec3 changedVar);
  void updateConstantLightSpecular(glm::vec3 changedVar);

  // --- Point Lights --- //
  void addPointLight(glm::vec3 pos, Shader* shader);
  void movePointLight(int lightnum, glm::vec3 newpos, Shader* shader);

private:

  // --- Directional Lights defaults --- //
  glm::vec3 direction = { -0.2f, -1.0f, -0.3f };
  glm::vec3 ambient = { 0.15f, 0.15f, 0.15f };
  glm::vec3 diffuse = { 0.15f, 0.15f, 0.15f };
  glm::vec3 specular = { 0.5f, 0.5f, 0.5f };

  // --- Point Lights --- //
  std::vector<glm::vec3> pointLights;
  int _currentPointLights = 0;
  const int MAXPOINTLIGHTS = 4;

  // --- Helper Functions --- //
  void clampVec3Between0and1(glm::vec3& changedVar);
  void clampVec3BetweenNeg1and1(glm::vec3& changedVar);

};

// lighting reference: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp

