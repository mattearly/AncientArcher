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

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  unsigned int id;
  std::string type;
};

class AAGameObject
{


public:

  AAGameObject(std::vector<Mesh> meshes);
  
  void draw();

private:

  std::vector<Mesh> mMeshes;

};
