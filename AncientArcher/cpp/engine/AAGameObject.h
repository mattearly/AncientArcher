#pragma once
#include <vector>
#include <mearly\Shader.h>

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

class AAGameObject
{


public:

  AAGameObject(std::vector<Mesh> meshes);
  
  void draw(Shader* shader);

private:

  std::vector<Mesh> mMeshes;

};
