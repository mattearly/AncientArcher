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

  std::unique_ptr< Shader > primShader;

};
