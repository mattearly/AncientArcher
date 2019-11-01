#pragma once
#include <Scene.h>

class LevelLoader
{
public:
  static LevelLoader* getLevelLoader();

  void loadDemoLevel(Scene& scene);

  void loadDemoLevel2(Scene& scene);

  void loadDemoLevel3(Scene& scene);

  void loadDemoLevel4(Scene& scene);

private:
  /* demo level*/
  void populateLayeredBlockGround(Scene& scene);
  void populateBoundries(Scene& scene);
  void populateMidSpheres(Scene& scene);
  void populateWalkwayPlanes(Scene& scene);
  void populateRockBall_1_Model(Scene& scene);

  void populateFullWalkwayPlanes(Scene& scene);

  void populateRandomGuy(Scene& scene);

  void populateBouncyBall(Scene& scene);

  void populateAliveBlock(Scene& scene);

  enum CURRENT_LEVEL { NA = 0, ONE, TWO, THREE, FOUR } _currentLevelLoaded = NA;

};