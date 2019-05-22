#include "Spawner.h"
#include <stdexcept>
#include <AAEngine.h>

void Spawner::init()
{

  TextureLoader texLoader;
  enemyTexID = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/spawner/scaryModel.png");

  enemyModel = std::make_unique<PrimativeRenderer>();

  Lighting lighting;
  lighting.updateConstantLightAmbient(glm::vec3(0.6f, 0.6f, 0.6f));
  lighting.setConstantLight(enemyModel->getShader());
}


void Spawner::checkAndSpawn(float deltaTime)
{
  static float timeAccumulator = 0;
  timeAccumulator += deltaTime;

  if (timeAccumulator > _timeBetweenSpawns)
  {

    // spawn enemy thing
    Entity* e = new Entity(
      ENTITYTYPE::CUBE,
      glm::vec3(15.f, 2.2f, 0.f),
      glm::vec3(1.5f, 2.5f, 0.03f),
      enemyTexID,
      true
    );

    // add to render list
    enemyModel->addToPrimativeEntities(*e);

    // yeah we're done lets not leak 
    delete e;

    _numberAlive++;
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
