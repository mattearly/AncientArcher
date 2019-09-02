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
  void populateDemoSphereModel(Scene& scene);
  void populateDemoCylinderModel(Scene& scene);
  void populateDemoBrickCylinderModel(Scene& scene);
  void populateRobot_1_Model(Scene& scene);
  void populateRockBall_1_Model(Scene& scene);
  void populateRustRobotDemo_1_Model(Scene& scene);
};