#pragma once
#include <Shader.h>
#include <Lighting.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
class Entity;
class PrimativeRenderer {
public:
  PrimativeRenderer();

  void update(float deltaTime);
  void render();
  void addToPrimativeEntities(Entity entity);
  void addToMovingEntities(Entity entity);

  std::vector<Entity>* getEntites();
  Entity* getFirstEntity();  // returns ptr to first entity

  std::vector<Entity>* getMovingEntites();
  Entity* getFirstMovingEntity();  // returns ptr to first entity

  Shader* getShader();

  Lighting* getLight();

  std::size_t size();

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

  std::vector<Entity> entities;

  std::vector<Entity> movingEntities;

  std::unique_ptr< Shader > primShader;

  std::shared_ptr<Lighting> primWorldLighting;

};
