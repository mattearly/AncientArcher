#pragma once
#include <glm/glm.hpp>
#include <vector>
class Renderer;
class Lighting {
public:
  Lighting();
  ~Lighting();

  std::vector<glm::vec3> pointLightPositions = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
  };
  
  void drawLighting();

private:



  bool lightLoaded;



  unsigned int lightVAO, lightVBO;


};

// lighting reference: https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/1.advanced_lighting/advanced_lighting.cpp

