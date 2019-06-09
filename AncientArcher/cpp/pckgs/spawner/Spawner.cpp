#include "Spawner.h"
#include <stdexcept>
#include <TextureLoader.h>

void Spawner::init()
{

  TextureLoader texLoader;
  enemyTexID = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/spawner/scaryModel.png");

  enemyModel = std::make_unique<PrimativeRenderer>();

  Lighting lighting;
  lighting.updateConstantLightAmbient(glm::vec3(0.6f, 0.6f, 0.6f));
  lighting.setConstantLight(enemyModel->getShader());
}

/**
 * Call this every frame for it to work as intended.
 */
void Spawner::checkAndSpawn(float deltaTime)
{
  static float timeAccumulator = 0;

  static float totalNumberSpawned = 0;

  if (_numberAlive < _popCap)
  {
    timeAccumulator += deltaTime;

    if (timeAccumulator > _timeBetweenSpawns)
    {

      totalNumberSpawned += 1.f;

      // spawn enemy thing
      Entity* e = new Entity(
        ENTITYTYPE::CUBE,
        glm::vec3(21.f * totalNumberSpawned, 2.79f, 0.f),
        glm::vec3(2.5f, 4.1f, 0.03f),
        enemyTexID,
        true,
		true
      );

      // add to render list
      enemyModel->addToPrimativeEntities(*e);

      // yeah we're done lets not leak 
      delete e;

      _numberAlive++;
      timeAccumulator = 0.f;
      _currentHP = _defaultHP;

    }
  }
  else
  {
    timeAccumulator = 0.f;
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
  enemyModel->render();
}

void Spawner::takeHit(float damage)
{
  _currentHP -= damage;
  if (_currentHP < 0.f)
  {
    despawn();
  }
}

void Spawner::despawn()
{
  enemyModel->entityPopBack();
  _numberAlive--;
}

float Spawner::getDamage()
{
  return _minionAttackDamage;
}

/**
 * Returns the first Entity Collider
 */
Collider* Spawner::getCollider()
{
  return enemyModel->getEntityPtr()->collider;
}

/**
 * Returns the first Entity
 */
Entity* Spawner::getEntity()
{
  return  enemyModel->getEntityPtr();
}

unsigned int Spawner::getAliveCount()
{
  return _numberAlive;
}

