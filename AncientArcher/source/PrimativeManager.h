#pragma once
#include <glm/glm.hpp>
class PrimativeManager {
public:
  PrimativeManager() = default;
  ~PrimativeManager() = default;

  void drawCube(glm::vec3 location);
  void drawCube(glm::vec3 location, glm::vec3 scale);
  void drawCube(glm::vec3 location, glm::vec3 scale, glm::vec3 rotation);

  void drawPlane(glm::vec3 location);
  void drawPlane(glm::vec3 location, glm::vec3 scale);
  void drawPlane(glm::vec3 location, glm::vec3 scale, glm::vec3 rotation);

private:

  void loadCube();
  void loadPlane();

  bool cubeLoaded;
  bool planeLoaded;

  unsigned int cubeVAO, cubeVBO;
  unsigned int planeVAO, planeVBO;

};
