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

  void render();
  void addToPrimativeEntities(Entity entity);

  std::vector<Entity>* getEntites();
  Shader* getShader();

private:

  void drawCube();
  void drawPlane();
  void drawSphere();

  void loadCube();
  void loadPlane();
  void loadSphere();

  bool cubeLoaded;
  bool planeLoaded;
  bool sphereLoaded;

  unsigned int cubeVAO, cubeVBO;
  unsigned int planeVAO, planeVBO;
  unsigned int sphereVAO, sphereVBO, sphereIBO, sphereIndexSize;

  std::vector<Entity> entities;

  std::unique_ptr< Shader > primShader;

  //Lighting lighting;

};
