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
  void populateWalkwayPlanes(Scene& scene);
  void populateDemoNanosuitModel(Scene& scene);
  void populateDemoConeModel(Scene& scene);
  void populateDemoMonkeyModel(Scene& scene);
};