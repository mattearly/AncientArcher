#pragma once
#include <TextureLoader.h>
#include <World.h>
class Planter
{
public:
  void plantDemoTree(const glm::vec3 loc, World *world);
private:
  TextureLoader texLoader;
  unsigned int texIDPlant = 0;
};