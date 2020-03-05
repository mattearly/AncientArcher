#pragma once
#include <string>
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
