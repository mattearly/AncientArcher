#include "AAOGLGraphics.h"
//#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <iostream>
//#include <iostream>
//#include <algorithm>
//#include <cstddef>
//
//
//void AAOGLGraphics::drawModel(int id)
//{
//
//}
//
//int AAOGLGraphics::loadModel(std::string path)
//{
//  AAOGLModel tmpModel;
//
//  if (!isLoaded(path))
//  {
//    static int id = -1;
//    static Assimp::Importer importer;
//    const aiScene* scene = importer.ReadFile(path, 0);
//    if (!scene)
//    {
//      return -2;  // failed to load path
//    }
//    tmpModel = loadElements(scene);
//    mPathsLoaded.emplace_back(path);
//    pushToGraphicsCard(tmpModel);
//    return ++id;
//  }
//  else
//  {
//    return getAlreadyLoadedModelID(path);  //already loaded, return the index
//  }
//}
//
//void AAOGLGraphics::showModelHeaderInfo(std::string path)
//{
//  AAModel model;
//
//  static Assimp::Importer importer;
//  const aiScene* scene = importer.ReadFile(path, 0);
//  model = loadModelHeaderInfo(scene);
//
//}

//bool AAOGLGraphics::isLoaded(std::string path)
//{
//  for (auto loadedPath : mPathsLoaded)
//  {
//    if (loadedPath == path)
//      return true;  //already loaded
//  }
//  return false;  // not loaded
//}
//
//AAOGLModel AAOGLGraphics::loadElements(const aiScene* scene)
//{
//  // header info
//  AAOGLModel buildingModel = loadModelHeaderInfo(scene);
//
//  // mesh data
//  for (int i = 0; i < buildingModel.numMeshes; ++i)
//  {
//    buildingModel.meshes.emplace_back(loadMesh(scene->mMeshes[i]));
//  }
//
//  return buildingModel;
//}
//
//AAOGLModel AAOGLGraphics::loadModelHeaderInfo(const aiScene* scene)
//{
//  AAOGLModel buildingModel;
//  buildingModel.numNodes = scene->mRootNode->mNumChildren;
//  buildingModel.numMeshes = scene->mNumMeshes;
//  buildingModel.numTextures = scene->mNumTextures;
//  buildingModel.numMaterials = scene->mNumMaterials;
//
//  for (int i = 0; i < buildingModel.numMeshes; ++i)
//  {
//    buildingModel.numVertices += scene->mMeshes[i]->mNumVertices;
//    buildingModel.numFaces += scene->mMeshes[i]->mNumFaces;
//    buildingModel.numBones += scene->mMeshes[i]->mNumBones;
//  }
//
//  return buildingModel;
//
//}
//
//int AAOGLGraphics::getAlreadyLoadedModelID(std::string path)
//{
//  int count = 0;
//  for (auto loadedPath : mPathsLoaded)
//  {
//    if (loadedPath == path)
//      return count;  //already loaded
//    count++;
//  }
//  return -1;  // failed to get loaded model
//}
//
//AAOGLModel::Mesh AAOGLGraphics::loadMesh(const aiMesh* mesh)
//{
//  AAOGLModel::Mesh tmpMesh;
//
//  for (int i = 0; i < mesh->mNumVertices; ++i)
//  {
//    std::cout << "loading vert\n";
//    tmpMesh.verticies.emplace_back(mesh->mVertices[i]);
//  }
//
//  return tmpMesh;
//}
//
//void AAOGLGraphics::pushToGraphicsCard(AAOGLModel& aamodel)
//{
//  //for (int i = 0; i < aamodel.meshes.size(); ++i)
//  //{
//  //  // setup Mesh
//  //  glGenVertexArrays(1, &aamodel.meshes[i].vao);
//  //  glGenBuffers(1, &aamodel.meshes[i].vbo);
//  //  glGenBuffers(1, &aamodel.meshes[i].ebo);
//
//  //  glBindVertexArray(aamodel.meshes[i].vao);
//  //  glBindBuffer(GL_ARRAY_BUFFER, aamodel.meshes[i].vbo);
//
//  //  glBufferData(
//  //    GL_ARRAY_BUFFER,
//  //    aamodel.meshes[i].verticies.size() * sizeof(aamodel.meshes[i].verticies[0]),
//  //    &aamodel.meshes[i].verticies[0],
//  //    GL_STATIC_DRAW
//  //  );
//
//  //  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aamodel.meshes[i].ebo);
//  //  glBufferData(
//  //    GL_ELEMENT_ARRAY_BUFFER,
//  //    aamodel.meshes[i].indices.size() * sizeof(unsigned int),
//  //    &aamodel.meshes[i].indices[0],  
//  //    GL_STATIC_DRAW
//  //  );
//
//  //  glEnableVertexAttribArray(0);
//  //  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(aamodel.meshes[i].verticies[0]), (void *)0);
//
//  //  glBindVertexArray(0);
//  //}
//}
//
//void AAOGLGraphics::unloadElements(std::string path)
//{
//  int id = getAlreadyLoadedModelID(path);
//  if (id == -1)
//  {
//    std::cout << "model " << path << "is not loaded\n";
//  }
//  else
//  {
//    mPathsLoaded.erase(
//      std::remove_if(
//        mPathsLoaded.begin(),
//        mPathsLoaded.end(),
//        [path](std::string a) {return a == path; }
//      ),
//      mPathsLoaded.end()
//    );
//
//    std::cout << "removed " << path << '\n';
//
//    unloadElements(id);
//  }
//}
//
//void AAOGLGraphics::unloadElements(int id)
//{
//  //todo
//  std::cout << "todo-build unloadElements(id)\n";
//}

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
    //processMesh(mesh, scene));
  }
}
