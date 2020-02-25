#pragma once
#include "AAGameObject.h"
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>

class TexLoader
{
public:
  static TexLoader* getInstance();
  unsigned int loadCubeTexture(const std::vector<std::string>& files);
  unsigned int textureFromFile(const char* filepath, bool gamma = false);
};

struct Vertex
{
  Vertex(glm::vec3 pos, glm::vec2 texcoords, glm::vec3 norms) noexcept;
  glm::vec3 Position;
  glm::vec2 TexCoords;
  glm::vec3 Normal;
};

class AAOGLGraphics
{
public:

  static AAOGLGraphics* getInstance();
  AAGameObject loadGameObjectWithAssimp(std::string path, bool pp_triangulate);

private:

  void processNode(aiNode* node, const aiScene* scene);
  MeshDrawInfo processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

  // holder vars
  std::string mLastDir = "";
  std::vector<TextureInfo> mTexturesLoaded;
  std::vector<MeshDrawInfo> mMeshDrawInfo;
};
