#pragma once
#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include "AAGameObject.h"

struct Model
{
  int id = 0;
};

struct Vertex
{
  Vertex(glm::vec3 pos, glm::vec2 texcoords, glm::vec3 norms);
  glm::vec3 Position;
  glm::vec2 TexCoords;
  glm::vec3 Normal;
};

class AAOGLGraphics
{
public:

  static AAOGLGraphics* getInstance();

  AAGameObject loadGameObjectWithAssimp(std::string path);

private:

  void processNode(aiNode* node, const aiScene* scene);
  MeshDrawInfo processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

  std::vector<std::string> mPathsLoaded;
  std::vector<TextureInfo> mTexturesLoaded;

  std::shared_ptr<Shader> mShader;

};
