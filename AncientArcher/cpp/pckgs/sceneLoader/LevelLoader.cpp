#include "LevelLoader.h"
#include <TextureLoader.h>
#include <Model.h>
#include <Prims.h>
#include <Entity.h>
#include <glm/glm.hpp>

LevelLoader* LevelLoader::_levelLoader = nullptr;

LevelLoader* LevelLoader::getLevelLoader()
{
  if (!_levelLoader)
  {
    _levelLoader = new LevelLoader();
  }
  return _levelLoader;
}

/////////////////////////PUBLIC FUNCTIONS/////////////////////////
void LevelLoader::loadDemoLevel(Scene& scene)
{
  if (_currentLevelLoaded != ONE)
  {
    scene.clearAll();

    populateLayeredBlockGround(scene);
    populateBoundries(scene);
    populateMidSpheres(scene);
    populateWalkwayPlanes(scene);
    populateRockBall_1_Model(scene);
    _currentLevelLoaded = ONE;

  }
  else
  {
    std::cout << "already on level one\n";
  }
}

void LevelLoader::loadDemoLevel2(Scene& scene)
{
  if (_currentLevelLoaded != TWO)
  {
    scene.clearAll();
    populateLayeredBlockGround(scene);
    _currentLevelLoaded = TWO;

  }
  else
  {
    std::cout << "already on level two\n";
  }
}

void LevelLoader::loadDemoLevel3(Scene& scene)
{
  if (_currentLevelLoaded != THREE)
  {
    scene.clearAll();

    populateFullWalkwayPlanes(scene);
    _currentLevelLoaded = THREE;

  }
  else
  {
    std::cout << "already on level three\n";
  }
}


////////////////////////PRIVATE FUNCTIONS/////////////////////////
void LevelLoader::populateLayeredBlockGround(Scene& scene)
{
  Prims tmp;
  for (int i = -20; i < 20; i++)  // Base Ground Layers
  {
    for (int j = -20; j < 20; j++)
    {
      for (int k = 0; k < 21; k++)
      {
        tmp.newObject(
          PRIMTYPE::CUBE,
          glm::vec3(i, -3.f - 1.0f * k, j),
          glm::vec3(1, 1, 1),
          (k < 1) ? "../AncientArcher/resource/world/grass.png" :
          (k < 4) ? "../AncientArcher/resource/world/dirt.png" :
          (k < 8) ? "../AncientArcher/resource/world/crumbling_rocks.png" :
          (k < 13) ? "../AncientArcher/resource/world/packed_rocks.png" :
          (k < 19) ? "../AncientArcher/resource/world/darkstone.png" :
          "../AncientArcher/resource/world/lava.png",
          true
        );
      }
    }
  }
  scene.addPrimGroup(tmp);
}


void LevelLoader::populateBoundries(Scene& scene)
{
  Prims tmp;

  for (int i = -21; i < 21; i++)  // width
  {
    for (int j = -21; j < 21; j++)  // width
    {
      for (int k = -5; k < 22; k++)  // height
      {
        if (((i == -21 || i == 20) || (j == -21 || j == 20)) || k == 21)
        {
          tmp.newObject(
            PRIMTYPE::CUBE,
            glm::vec3(i, -3.f - 1.0f * k, j),
            glm::vec3(1, 1, 1),
            "../AncientArcher/resource/14-maze_metal.png",
            true
          );
        }
      }
    }
  }
  scene.addPrimGroup(tmp);
}

void LevelLoader::populateMidSpheres(Scene& scene)
{
  Prims tmp;
  for (int i = -3; i < 3; i++)  // Base Ground Layers
  {
    for (int j = -3; j < 3; j++)
    {
      tmp.newObject(
        PRIMTYPE::SPHERE,
        glm::vec3(i, -2.f, j),
        glm::vec3(1, 1, 1),
        "../AncientArcher/resource/08-pixelized_water.png",
        true
      );
    }
  }
  scene.addPrimGroup(tmp);
}

void LevelLoader::populateWalkwayPlanes(Scene& scene)
{
  Prims tmp;
  for (int i = -21; i < 21; i++)  // width
  {
    for (int j = -21; j < 21; j++)  // width
    {
      if ((i == -20 || i == 19) || (j == -20 || j == 19))
      {
        tmp.newObject(
          PRIMTYPE::PLANE,
          glm::vec3(i, 0.f, j),
          glm::vec3(1, 1, 1),
          "../AncientArcher/resource/11-pixelized_darkwood.png",
          true
        );
      }
    }
  }
  scene.addPrimGroup(tmp);
}

void LevelLoader::populateRockBall_1_Model(Scene& scene)
{
  Model* tmpModel;
  tmpModel = new Model("../AncientArcher/resource/models/sphere/rock_ball_1.dae");
  scene.addModel(*tmpModel);
}

void LevelLoader::populateFullWalkwayPlanes(Scene& scene)
{
  Prims tmp;
  for (int i = -21; i < 21; i++)  // width
  {
    for (int j = -21; j < 21; j++)  // width
    {
      tmp.newObject(
        PRIMTYPE::CUBE,
        glm::vec3(i, -4.f, j),
        glm::vec3(1, 1, 1),
        "../AncientArcher/resource/11-pixelized_darkwood.png",
        true
      );
    }
  }
  scene.addPrimGroup(tmp);
}