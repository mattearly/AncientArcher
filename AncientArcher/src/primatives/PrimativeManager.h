#pragma once
//#include<map>
//#include<string>
class PrimativeManager {
public:
  PrimativeManager();
  ~PrimativeManager();
  
  void drawCube();

private:

  void loadCube();

  //std::map<std::string, unsigned int> objects;

  bool cubeLoaded;

};

