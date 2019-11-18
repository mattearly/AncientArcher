#pragma once
#include <string>
#include "../engine/AAOGLGraphics.h"
#include <iostream>
#include <assimp\BaseImporter.h>
#include <assimp\Importer.hpp>

//void showModelHeaderInfo(std::string path)
//{
//  AAOGLGraphics test;
//
//  test.showModelHeaderInfo(path);
//
//  std::cout << "model: " << test.path << ":\n";
//  std::cout << " numNodes " << model.numNodes << '\n';
//  std::cout << "   numMeshes " << model.numMeshes << '\n';
//  std::cout << "     numVertices " << model.numVertices << '\n';
//  std::cout << "     numFaces " << model.numFaces << '\n';
//  std::cout << "     numBones " << model.numBones << '\n';
//  std::cout << " numTextures " << model.numTextures << '\n';
//  std::cout << " numMaterials " << model.numMaterials << '\n';
//  std::cout << " numAnimations " << model.numAnimations << '\n';
//}
//
//void showModelHeaderInfo(std::vector<std::string> paths)
//{
//  AAOGLGraphics test;
//
//  for (auto i = 0; i < paths.size(); ++i)
//  {
//    std::cout << "model " << i << '\n';
//    test.showModelHeaderInfo(paths[i]);
//  }
//}
//
//int loadDemoBlock(std::string path)
//{
//  AAOGLGraphics test;
//  test.loadModel(path);
//
//}

void testAssimpBaseLoading(std::string path)
{
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

  ModelHeader demoModel;
  demoModel.modelPath = path;

  static Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, 0);

  if (!scene)
  {
    std::cout << "could not openfile\n";
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
