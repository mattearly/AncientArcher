#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include "AAGameObject.h"

class AAOGLGraphics
{
public:
  static AAOGLGraphics* getInstance();

  AAGameObject loadModelWithAssimpToOpenGL(std::string path);

private:

  void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes);
  Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

