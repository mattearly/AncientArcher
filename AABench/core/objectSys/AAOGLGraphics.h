#pragma once
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "AAGameObject.h"
#include "shaderSys/AAOGLShader.h"
#include "MeshDrawInfo.h"
#include "InstanceDetails.h"

class TexLoader
{
public:
  static TexLoader* getInstance();
  unsigned int loadCubeTexture(const std::vector<std::string>& files);
  unsigned int textureFromFile(const char* filepath, bool gamma = false);
};

class AAOGLGraphics
{
public:

  static AAOGLGraphics* getInstance();
  int loadGameObjectWithAssimp(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path, bool pp_triangulate);
  void processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_);

  friend class AAGameObject;
private:

  MeshDrawInfo processMesh(aiMesh* mesh, const aiScene* scene);
  int loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureInfo>& out_texInfo);

  void Render(const std::vector<MeshDrawInfo>& meshes, const std::vector<InstanceDetails>& details, const AAOGLShader& modelShader);

  // holder vars
  std::string mLastDir = "";
  std::vector<TextureInfo> mTexturesLoaded;

};
