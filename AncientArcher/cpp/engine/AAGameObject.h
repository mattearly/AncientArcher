#pragma once
#include <vector>
#include <mearly\Shader.h>
#include <memory>

struct TextureInfo
{
  unsigned int id;
  std::string type;
  std::string path;
};

struct MeshDrawInfo
{
  MeshDrawInfo(unsigned int a, unsigned int b, unsigned int e, std::vector<TextureInfo> t, std::vector<unsigned int> el);
  unsigned int vao = 0;
  unsigned int vbo = 0;
  unsigned int ebo = 0;
  std::vector<TextureInfo> textures;
  std::vector<unsigned int> elements;
};

class AAGameObject
{
public:

  AAGameObject(std::vector<MeshDrawInfo> meshes);
  void draw();
  void translate(glm::vec3 amt);
  void rotate(float angle, glm::vec3 axis);
  
private:

  std::vector<MeshDrawInfo> mMeshes;
  glm::mat4 mModelMatrix = glm::mat4(1);
};
