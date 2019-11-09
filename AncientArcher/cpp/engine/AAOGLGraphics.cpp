#include "AAOGLGraphics.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <iostream>

int AAOGLGraphics::loadModel(std::string path)
{
  if (!isLoaded(path))
  {
    static int id = -1;
    static Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 0);
    if (!scene)
    {
      return -2;  // failed to load path
    }
    mModels.emplace_back(loadElements(scene));
    mPathsLoaded.emplace_back(path);
    return ++id;
  }
  else
  {
    return getAlreadyLoadedModelID(path);  //already loaded, return the index
  }
}

void AAOGLGraphics::outputModelData()
{
  std::cout << "models loaded: " << mModels.size() << '\n';
  int count = 0;
  for (auto model : mModels)
  {
    std::cout << "model " << count << ":\n";
    std::cout << " numMeshes " << model.numMeshes << '\n';
    std::cout << " numVertices " << model.numVertices << '\n';
    std::cout << " numFaces " << model.numFaces << '\n';
  }
}

bool AAOGLGraphics::isLoaded(std::string path)
{
  for (auto loadedPath : mPathsLoaded)
  {
    if (loadedPath == path)
      return true;  //already loaded
  }
  return false;  // not loaded
}

AAOGLModel AAOGLGraphics::loadElements(const aiScene* scene)
{
  // load model up here
  AAOGLModel buildingModel;
  buildingModel.numMeshes = scene->mNumMeshes;
  buildingModel.numTextures = scene->mNumTextures;
  buildingModel.numMaterials = scene->mNumMaterials;
  buildingModel.numAnimations = scene->mNumAnimations;

  for (int i = 0; i < buildingModel.numMeshes; ++i)
  {
    buildingModel.numVertices += scene->mMeshes[i]->mNumVertices;
    buildingModel.numFaces += scene->mMeshes[i]->mNumFaces;
    buildingModel.numBones += scene->mMeshes[i]->mNumBones;
    //buildingModel.numTexels += scene->mMeshes[i]->mNumUVComponents[x];
    //buildingModel.numNormals += scene->mMeshes[i]->mNum;
    //buildingModel.numPositions += scene->mMeshes[i]->mNumPos
    //buildingModel.numTexels += scene->mN
  }

  return buildingModel;
}

int AAOGLGraphics::getAlreadyLoadedModelID(std::string path)
{
  int count = 0;
  for (auto loadedPath : mPathsLoaded)
  {
    if (loadedPath == path)
      return count;  //already loaded
    count++;
  }
  return -1;  // failed to get loaded model
}
