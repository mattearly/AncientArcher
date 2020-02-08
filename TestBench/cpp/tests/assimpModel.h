#pragma once
#include <string>
#include <iostream>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>

struct ModelHeader
{
  std::string modelPath;
  int numNodes = 0;
  int numVertices = 0;
  int numFaces = 0;
  int numMeshes = 0;
  int numTextures = 0;
  int numMaterials = 0;
  int numAnimations = 0;
  int numBones = 0;
};

void testAssimpBaseLoading(std::string path)
{

  ModelHeader demoModel;
  demoModel.modelPath = path;

  static Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, 0);

  if (!scene)
  {
    std::cout << "could not openfile at " << path << '\n';
  }
  else {
    demoModel.numNodes = scene->mRootNode->mNumChildren;
    demoModel.numMeshes = scene->mNumMeshes;
    demoModel.numTextures = scene->mNumTextures;
    demoModel.numMaterials = scene->mNumMaterials;

    for (int i = 0; i < demoModel.numMeshes; ++i)
    {
      demoModel.numVertices += scene->mMeshes[i]->mNumVertices;
      demoModel.numFaces += scene->mMeshes[i]->mNumFaces;
      demoModel.numBones += scene->mMeshes[i]->mNumBones;
    }

    std::cout << "Assimp Base Header Only loading Test On: \n"
      << demoModel.modelPath << '\n'
      << "nodes: " << demoModel.numNodes << '\n'
      << "meshes: " << demoModel.numMeshes << '\n'
      << "textures: " << demoModel.numTextures << '\n'
      << "materials: " << demoModel.numMaterials << '\n'
      << "vertices: " << demoModel.numVertices << '\n'
      << "faces: " << demoModel.numFaces << '\n'
      << "bones: " << demoModel.numBones << '\n';
  }

}
