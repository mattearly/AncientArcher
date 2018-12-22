#pragma once
#include "../Shader.h"
class PrimativeManager {
public:
  PrimativeManager();
  ~PrimativeManager();
  
  void drawCube(const Shader *shader);

private:

  void loadCube();

  //std::map<std::string, unsigned int> objects;

  bool cubeLoaded;

  unsigned int cubeVAO, cubeVBO;


};

