#pragma once
#include <glm/glm.hpp>
class PrimativeManager {
public:
  PrimativeManager();
  ~PrimativeManager();
  
  //void drawCube();
  void drawCube(float deltaTime);
  void drawCube(glm::vec3 xyzlocation, glm::vec3 xyzsize);


private:

  void loadCube();

  //std::map<std::string, unsigned int> objects;

  bool cubeLoaded;

  unsigned int cubeVAO, cubeVBO;


};

