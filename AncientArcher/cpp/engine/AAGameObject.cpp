#include "AAGameObject.h"

Mesh::Mesh(unsigned int a, unsigned int b, unsigned int e) : vao(a), vbo(b), ebo(e) 
{
}

AAGameObject::AAGameObject(std::vector<Mesh> meshes)
{
  mMeshes = meshes;
}

void AAGameObject::draw(Shader* shader)
{
  for (auto m : mMeshes)
  {

  }
}
