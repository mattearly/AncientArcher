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
  ~PrimativeRenderer() = default;

  void render();

  void addToPrimativeEntities(Entity entity);

private:

  void loadCube();
  void loadPlane();

  bool cubeLoaded;
  bool planeLoaded;

  unsigned int cubeVAO, cubeVBO;
  unsigned int planeVAO, planeVBO;

  std::vector<Entity> entities;

  std::unique_ptr< Shader > primShader;

  void drawCube();
  void drawPlane();

  Lighting lighting;

};
