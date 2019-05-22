#include "Spawner.h"
#include <stdexcept>

void Spawner::checkAndSpawn(float deltaTime)
{
  static float timeAccumulator = 0;
  timeAccumulator += deltaTime;

  if (timeAccumulator > _timeBetweenSpawns)
  {
    timeAccumulator = 0.f;

    // spawn enemy thing

    _numberAlive++;

  }
}

void Spawner::setTimeBetweenSpawns(float time)
{
  if (time > 0.f) 
  {
    _timeBetweenSpawns = time;
  }
  else
  {
    throw std::runtime_error("wut. You can't do negative time get outta here!");
  }
}

void Spawner::setPopulationCap(unsigned int max)
{
  _popCap = max;
}

void Spawner::render()
{

}
