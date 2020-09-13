#pragma once
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "../MeshDrawInfo.h"

namespace AA
{
class SceneLoader
{
public:
  int loadGameObjectWithAssimp(std::vector<MeshDrawInfo>& out_MeshInfo, std::string path, bool pp_triangulate);

private:

  void processNode(aiNode* node, const aiScene* scene, std::vector<MeshDrawInfo>& out_);
  MeshDrawInfo processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4* trans);
  int loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureInfo>& out_texInfo);

  // holder vars
  std::string mLastDir = "";
  std::vector<TextureInfo> mTexturesLoaded;
};

} // end namespace AA