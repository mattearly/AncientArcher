#pragma once
#include <vector>
#include <mearly\Shader.h>
#include <memory>

struct MeshDrawInfo
{
  MeshDrawInfo(unsigned int a, unsigned int b, unsigned int e);
  unsigned int vao = 0;
  unsigned int vbo = 0;
  unsigned int ebo = 0;
};

struct TextureInfo
{
  unsigned int id;
  std::string type;
  std::string path;
};

class AAGameObject
{

public:

  AAGameObject(std::vector<MeshDrawInfo> meshes, std::vector<TextureInfo> textures, std::vector<unsigned int> elements);

  void draw();

private:

  std::vector<MeshDrawInfo> mMeshes;
  std::vector<unsigned int> mElements;
  std::vector<TextureInfo> mTextures;

  std::shared_ptr<Shader> mShader;
};
