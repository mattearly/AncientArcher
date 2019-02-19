#pragma once
#include <glm/glm.hpp>
#include <vector>

class Lighting {
public:
  Lighting();
  ~Lighting();

  void setConstantLight();

  void addPointLight(glm::vec3 pos);

  void movePointLight(int lightnum, glm::vec3 newpos);

private:

  std::vector<glm::vec3> pointLights;

  const int MAXPOINTLIGHTS = 4;

};

// lighting reference: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp

