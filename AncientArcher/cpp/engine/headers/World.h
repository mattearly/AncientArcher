#pragma once
#include <Camera.h>
#include <Shader.h>
#include <Lighting.h>
#include <Entity.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Camera;

class World {
public:
  World();

  void update(float deltaTime);
  void render();
  void addToStationaryEntities(Entity entity);
  void addToMovingEntities(Entity entity);

  std::vector<Entity>* getEntites();

  Entity* getFirstEntity();                   // returns ptr to first entity

  Entity* getFirstPlayerEntity();             // returns ptr to the first player entity

  std::vector<Entity>* getMovingEntites();

  Entity* getFirstMovingEntity();             // returns ptr to first entity

  Shader* getShader();
  std::shared_ptr<Shader>& getSharedShader();

  Lighting* getLight();

  Camera* getCamera();
  std::shared_ptr<Camera>& getSharedCamera();

  std::size_t numberOfEntities();

  void entityPopBack();

private:
  float elapsedTime = 0.0f;
  bool timeTrigger = true;

  void drawCube();
  void drawPlane();
  void drawSphere();
  void loadCube();
  void loadPlane();
  void loadSphere();
  bool cubeLoaded = false;
  bool planeLoaded = false;
  bool sphereLoaded = false;

  unsigned int cubeVAO = 0;
  unsigned int cubeVBO = 0;
  unsigned int planeVAO = 0;
  unsigned int planeVBO = 0;
  unsigned int sphereVAO = 0;
  unsigned int sphereVBO = 0;
  unsigned int sphereIBO = 0;
  unsigned int sphereIndexSize = 0;

  std::vector<Entity> _players;
  std::vector<Entity> _stationaryEntities;
  std::vector<Entity> _movingEntities;

  std::shared_ptr<Camera>   _defaultWorldCamera;
  std::shared_ptr<Shader>   _defaultWorldShader;
  std::shared_ptr<Lighting> _defaultWorldLighting;

};
