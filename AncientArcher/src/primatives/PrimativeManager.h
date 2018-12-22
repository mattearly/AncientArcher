#pragma once
class PrimativeManager {
public:
  PrimativeManager();
  ~PrimativeManager();
  
  void drawCube(float deltaTime);

private:

  void loadCube();

  //std::map<std::string, unsigned int> objects;

  bool cubeLoaded;

  unsigned int cubeVAO, cubeVBO;


};

