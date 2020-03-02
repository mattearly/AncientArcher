/*
  Test building a small world with different shaders.
  Test Collision.
  Test basic sound effects functionality.
  Test demo asssets.
  Test Assimp base model header loading.
*/
#pragma once
#include "dry/addGodMovement.h"
#include "dry/addSkybox.h"
#include <string>
#include "dry/addRandomWorld.h"
#include "dry/ModelHeader.h"
#include <assimp\Importer.hpp>

extern AAWorld mainWorld;

int testEngineLoopBase()
{
  mainWorld.setWindowTitle("demo loop base");
  // Add to loop functions.
  mainWorld.addToOnBegin([]()noexcept {printf("World Started. timestamp: %s \n", __TIMESTAMP__); });
  mainWorld.addToDeltaUpdate([](float step)noexcept {static float timePassed = 0; timePassed += step; const float newcolor = abs(sin(timePassed)); AADisplay::getInstance()->setWindowClearColor(glm::vec3(newcolor, newcolor, newcolor)); });
  mainWorld.addToDeltaUpdate([](float step) {std::cout << ".      process delta update: " << step << '\n'; });
  //mainWorld.addToOnRender([]() {std::cout << "..     process render\n"; });
  mainWorld.addToTimedOutKeyHandling([](AAKeyBoardInput& keys)noexcept {printf("...    %fms timedout keyboard/mouse\n", 90.f); return false;  });
  mainWorld.addToKeyHandling([](AAKeyBoardInput& keys) {std::cout << "....   process keyboard/mouse \n";  });
  mainWorld.addToScrollHandling([](AAScrollInput& scroll) {std::cout << ".....  process mouse scroll: X:" << scroll.xOffset << " Y:" << scroll.yOffset << '\n'; });
  mainWorld.addToMouseHandling([](AAMouseInput& pointer) {std::cout << "...... process mouse pointer: X:" << pointer.xOffset << " Y:" << pointer.yOffset << '\n';  });
  mainWorld.addToUpdate([]() {std::cout << ".......process update\n"; });
  return mainWorld.runMainLoop();
}

int testEngineDemoWorld()
{
  mainWorld.setWindowTitle("Test Engine Demo World");
  int cameraId = setGodCamWithMovement(); 

  addRandomWorld(cameraId);

  addCubeSkybox();

  return mainWorld.runMainLoop();
}

//void testAssimpBaseLoading(std::string path)
//{
//  ModelHeader demoModel;
//  demoModel.modelPath = path;
//
//  static Assimp::Importer importer;
//  const aiScene* scene = importer.ReadFile(path, 0);
//
//  if (!scene)
//  {
//    std::cout << "could not openfile at " << path << '\n';
//  }
//  else
//  {
//    demoModel.numNodes = scene->mRootNode->mNumChildren;
//    demoModel.numMeshes = scene->mNumMeshes;
//    demoModel.numTextures = scene->mNumTextures;
//    demoModel.numMaterials = scene->mNumMaterials;
//
//    for (int i = 0; i < demoModel.numMeshes; ++i)
//    {
//      demoModel.numVertices += scene->mMeshes[i]->mNumVertices;
//      demoModel.numFaces += scene->mMeshes[i]->mNumFaces;
//      demoModel.numBones += scene->mMeshes[i]->mNumBones;
//    }
//
//    std::cout << "Assimp Base Header Only loading Test On: \n"
//      << demoModel.modelPath << '\n'
//      << "nodes: " << demoModel.numNodes << '\n'
//      << "meshes: " << demoModel.numMeshes << '\n'
//      << "textures: " << demoModel.numTextures << '\n'
//      << "materials: " << demoModel.numMaterials << '\n'
//      << "vertices: " << demoModel.numVertices << '\n'
//      << "faces: " << demoModel.numFaces << '\n'
//      << "bones: " << demoModel.numBones << '\n';
//  }
//}

void runAllCoreTests(int& result)
{
  result = testEngineLoopBase();
  result = testEngineDemoWorld();
}