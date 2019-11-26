#include "AAOGLGraphics.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <iostream>

AAOGLGraphics* AAOGLGraphics::getInstance()
{
  static AAOGLGraphics* graphics = new AAOGLGraphics();
  return graphics;
}

AAGameObject AAOGLGraphics::loadModelWithAssimpToOpenGL(std::string path)
{
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> texCoords;
  std::vector<glm::vec3> normals;
  std::vector<glm::ivec3> indices;
  std::string directory;

  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, 0);

  std::vector<Mesh> meshes;


  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
    return AAGameObject(meshes);
  }

  directory = path.substr(0, path.find_last_of('/'));


  processNode(scene->mRootNode, scene, meshes);

  
  return AAGameObject(meshes);

}

void AAOGLGraphics::processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes)
{
  for (unsigned int i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; ++i)
  {
    //processNode(node->mChildren[i], scene,);
  }
}

Mesh AAOGLGraphics::processMesh(aiMesh* mesh, const aiScene* scene)
{
  return Mesh(0,0,0);
}

std::vector<Texture> AAOGLGraphics::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
  return std::vector<Texture>();
}
