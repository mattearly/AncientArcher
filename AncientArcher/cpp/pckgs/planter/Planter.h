#pragma once
#include <TextureLoader.h>

#include "../primatives/PrimativeRenderer.h"

class Planter
{
public:
  void plantDemoTree(const glm::vec3 loc, PrimativeRenderer *prims);
private:
  TextureLoader texLoader;
  unsigned int texIDPlant = 0;
};