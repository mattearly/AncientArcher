#pragma once
#include <vector>
#include <mearly\Shader.h>
#include <memory>

enum class Shading { NONE, CELL, IMGTEX };

struct TextureInfo
{
  unsigned int id = 0;
  std::string type;
  std::string path;
  glm::vec3 color = glm::vec3(0);
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

  glm::mat4* getModelMatrix();

  AAGameObject(std::vector<MeshDrawInfo> meshes, Shading shading);
  void draw();
  void translate(glm::vec3 amt);
  void rotate(float angle, glm::vec3 axis);
  
private:

  std::vector<MeshDrawInfo> mMeshes;
  glm::mat4 mModelMatrix = glm::mat4(1);

  Shading mModelShaderType = Shading::NONE;
};
