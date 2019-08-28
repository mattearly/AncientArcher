#pragma once
#include <Camera.h>
#include <Shader.h>
#include <Lighting.h>
#include <Entity.h>
#include <Cube.h>
#include <Plane.h>
#include <Sphere.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Camera;

class World 
{
public:
  World();

  void update(float deltaTime);
  void render();

  void addToStationaryEntities(Entity entity);
  void stationaryEntityPopBack();
  std::size_t numberOfStationaryEntities();

  void addToMovingEntities(Entity entity);

  Entity* getFirstEntity();
  std::vector<Entity>* getEntities();

  Entity* getFirstPlayerEntity();
  std::vector<Entity>* getPlayers();

  Entity* getFirstMovingEntity();
  std::vector<Entity>* getMovingEntites();

  Shader* getShader();
  std::shared_ptr<Shader>& getSharedShader();

  Lighting* getLighting();
  std::shared_ptr<Lighting>& getSharedLighting();

  Camera* getCamera();
  std::shared_ptr<Camera>& getSharedCamera();

private:

  std::vector<Entity> _players;

  std::vector<Entity> _stationaryPrimitives;
  std::vector<Entity> _movingPrimitives;

  // for drawing 
  //Primitive _prims;

  std::shared_ptr<Camera>   _defaultWorldCamera;
  
  std::shared_ptr<Shader>   _defaultPrimitiveShader;
  std::shared_ptr<Lighting> _defaultPrimitiveLighting;

};
