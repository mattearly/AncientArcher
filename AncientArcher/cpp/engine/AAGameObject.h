#pragma once
#include <vector>
class AAGameObject
{
  struct Mesh
  {
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int ebo = 0;
    Mesh(unsigned int a, unsigned int b, unsigned int e);

  };

  struct Model
  {
    int id = 0;
  };

public:

  
private:
  std::vector<Mesh> mMeshes;

};

