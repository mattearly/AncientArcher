#pragma once
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "AAGameObject.h"
#include "shaderSys/AAOGLShader.h"

class TexLoader
{
public:
  static TexLoader* getInstance();
  unsigned int loadCubeTexture(const std::vector<std::string>& files);
  unsigned int textureFromFile(const char* filepath, bool gamma = false);
};

struct Vertex
{
  Vertex(glm::vec3 pos, glm::vec3 norms, glm::vec2 texcoords) noexcept;
  Vertex(glm::vec3 pos, glm::vec3 norms, glm::vec4 colors, glm::vec2 texcoords) noexcept;
  glm::vec3 Position;
  glm::vec2 TexCoords;
  glm::vec3 Normal;
  glm::vec4 Color;
};

class AAOGLGraphics
{
public:

  static AAOGLGraphics* getInstance();
  bool loadGameObjectWithAssimp(std::string path, bool pp_triangulate, std::vector<MeshDrawInfo>& out_);

  void processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_);

private:

  MeshDrawInfo processMesh(aiMesh* mesh, const aiScene* scene);
  int loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureInfo>& out_texInfo);

  // holder vars
  std::string mLastDir = "";
  std::vector<TextureInfo> mTexturesLoaded;
};
