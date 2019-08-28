#pragma once
#include <Scene.h>

class LevelLoader
{
public:
  void loadDemoLevel(Scene& scene);
private:
  /* demo level*/
  void populateLayeredBlockGround(Scene& scene);
  void populateBoundries(Scene& scene);
  void populateMidSpheres(Scene& scene);
  void popluateWalkwayPlanes(Scene& scene);
};