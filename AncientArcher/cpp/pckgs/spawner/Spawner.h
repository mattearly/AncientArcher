#pragma once
#include "../primatives/PrimativeRenderer.h"
#include <Collider.h>
#include <Entity.h>
#include <memory>
class Spawner
{
public:

  void checkAndSpawn(float deltaTime);

  void setTimeBetweenSpawns(float time);

  void setPopulationCap(unsigned int max);

  void render();

  Collider* getCollider();
  Entity* getEntity();
  unsigned int getAliveCount();
  void init();

private:

  std::unique_ptr<PrimativeRenderer> enemyModel;

  unsigned int _popCap = 0;

  unsigned int _numberAlive = 0;

  float _timeBetweenSpawns = 0.f;

  unsigned int enemyTexID = 0;

};