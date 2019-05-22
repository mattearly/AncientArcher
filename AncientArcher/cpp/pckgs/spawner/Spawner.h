#include "../primatives/PrimativeRenderer.h"
#pragma once
class Spawner
{
public:

  void checkAndSpawn(float deltaTime);

  void setTimeBetweenSpawns(float time);

  void setPopulationCap(unsigned int max);

  void render();

private:

  PrimativeRenderer enemyModel;

  unsigned int _popCap = 0;

  unsigned int _numberAlive = 0;

  float _timeBetweenSpawns = 0.f;

};