#include <World.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>
#include <vector>

/**
 * Loads up a primative renderer with the default settings and shader.
 */
World::World()
{

  // world position 0,0,0
  // yaw -90.f : forward facing down the -z axis  (should be 0.f is down the -z axis, why the f is it -90? #confusedprogramming)
  // pitch 0.0f : forward
  // field of view 80.f : pretty wide, slight fisheye
  _defaultWorldCamera = std::make_shared<Camera>(glm::vec3(0.f, 5.0f, 0.f), -90.f, -45.0f, 80.f);
  _defaultWorldShader = std::make_shared<Shader>("../AncientArcher/resource/world/primative.vert", "../AncientArcher/resource/world/primative.frag");

  _defaultWorldShader->use();
  glm::mat4 proj = _defaultWorldCamera->getProjectionMatrix();
  _defaultWorldShader->setMat4("projection", proj);


  _defaultWorldLighting = std::make_shared<Lighting>();

  //_defaultWorldLighting->updateConstantLightAmbient(glm::vec3(.09, 0.07, 0.07));
  //_defaultWorldLighting->updateConstantLightDirection(glm::vec3(0, -1, 0));
  //_defaultWorldLighting->updateConstantLightDiffuse(glm::vec3(.80, .70, .74));
  //_defaultWorldLighting->updateConstantLightSpecular(glm::vec3(.5, .5, .5));

  _defaultWorldLighting->setConstantLight(getShader());

  // debug
  std::cout << "number of shared Camera in world init " << _defaultWorldCamera.use_count() << std::endl;
  std::cout << "number of shared Shader in world init " << _defaultWorldShader.use_count() << std::endl;
  std::cout << "number of shared Lighting in world init " << _defaultWorldLighting.use_count() << std::endl;
  // -- ok

}

void World::update(float deltaTime)
{
  static float elapsedTime = 0.0f;
  static bool timeTrigger = true;

  elapsedTime += deltaTime;

  if (elapsedTime > 2.0f)
  {
    timeTrigger = !timeTrigger;
    elapsedTime = 0.0f;
  }

  for (auto& e : *(getMovingEntites())) 
  {
    e.moveBy(glm::vec3(0.f,
      0.f,
      (sin(elapsedTime * 3.14159 / 180) * (timeTrigger ? 1.0 : -1.0)))
    );
  }
}

/**
 * Renders all the objects on the std::vector<Entity> entities array.
 */
void World::render()
{
  _defaultWorldShader->use();

  _defaultWorldCamera->update(_defaultWorldShader.get());
  glEnable(GL_DEPTH_TEST);

  for (auto e : _stationaryEntities)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, e.gameItem.textureID);

    glm::mat4 model = glm::mat4(1.0f);
    // step1: translate
    model = glm::translate(model, glm::vec3(e.gameItem.loc));
    // step2: rotations  -- not supported by colliders yet
    //  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // step3: scale
    model = glm::scale(model, glm::vec3(e.gameItem.scale));

    _defaultWorldShader->setMat4("model", model);

    switch (e.gameItem.type)
    {
    case ENTITYTYPE::CUBE:
      _prims.drawCube();
      break;
    case ENTITYTYPE::PLANE:
      _prims.drawPlane();
      break;
    case ENTITYTYPE::SPHERE:
      _prims.drawSphere();
      break;
    default: break;
    }
  }
  for (auto e : _movingEntities)
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, e.gameItem.textureID);

    glm::mat4 model = glm::mat4(1.0f);
    // step1: translate
    model = glm::translate(model, glm::vec3(e.gameItem.loc));
    // step2: rotations  -- not supported by colliders yet
    //  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    //  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    // step3: scale
    model = glm::scale(model, glm::vec3(e.gameItem.scale));

    _defaultWorldShader.get()->setMat4("model", model);

    switch (e.gameItem.type)
    {
    case ENTITYTYPE::CUBE:
      _prims.drawCube();
      break;
    case ENTITYTYPE::PLANE:
      _prims.drawPlane();
      break;
    case ENTITYTYPE::SPHERE:
      _prims.drawSphere();
      break;
    default: break;
    }
  }
}

/**
 * Adds a built entity to the std::vector<Entity> entities array.
 */
void World::addToStationaryEntities(Entity entity)
{
  _stationaryEntities.push_back(entity);
}

void World::addToMovingEntities(Entity entity)
{
  _movingEntities.push_back(entity);
}

std::vector<Entity>* World::getEntities()
{
  return &_stationaryEntities;
}

Entity* World::getFirstEntity()
{
  return &_stationaryEntities[0];
  //return entities.data();
}

std::vector<Entity>* World::getPlayers()
{
  return nullptr;
}

Entity* World::getFirstPlayerEntity()
{
  return &_players[0];
  //return players.data();
}

std::vector<Entity>* World::getMovingEntites()
{
  return &_movingEntities;
}

Entity* World::getFirstMovingEntity()
{
  return &_movingEntities[0];
}

Shader* World::getShader()
{
  return _defaultWorldShader.get();
}

std::shared_ptr<Shader>& World::getSharedShader()
{
  return _defaultWorldShader;
}

Lighting* World::getLighting()
{
  return _defaultWorldLighting.get();
}

std::shared_ptr<Lighting>& World::getSharedLighting()
{
  return _defaultWorldLighting;
}

Camera* World::getCamera()
{
  return _defaultWorldCamera.get();
}

std::shared_ptr<Camera>& World::getSharedCamera()
{
  return _defaultWorldCamera;

}
std::size_t World::numberOfStationaryEntities()
{
  return _stationaryEntities.size();
}

void World::stationaryEntityPopBack()
{
  if (_stationaryEntities.size() > 0)
  {
    _stationaryEntities.pop_back();
  }
}
