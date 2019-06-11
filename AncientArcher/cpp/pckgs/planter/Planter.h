#pragma once
#include <Entity.h>
#include <TextureLoader.h>

#include <memory>
#include "../primatives/PrimativeRenderer.h"

class Planter
{
public:
  void update(float deltaTime);
  Entity* plantDemoTree(glm::vec3 loc);
private:
  std::vector<Entity*> allPlants;
  TextureLoader texLoader;
  unsigned int texIDPlant = 0;
};