#include "Renderer.h"
#include "../models/PrimativeManager.h"
#include "../player/Player.h"
#include "../globals.h"

#include <iostream>

extern Entity *entity;

Renderer::Renderer() {
  width = round(logic_checking_distance + 1);
  enableGLDepthTest();
}

Renderer::~Renderer() {}

void Renderer::enableGLBlend() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::disableGLBlend() {
  glDisable(GL_BLEND);
}

void Renderer::enableGLDepthTest() {
  glEnable(GL_DEPTH_TEST);
}

void Renderer::disableGLDepthTest() {
  glDisable(GL_DEPTH_TEST);

}

void Renderer::update(Player *player, PrimativeManager *primativeManager, float deltaTime) {

  display.clear();

  drawEntity(entity, primativeManager);

  for (auto e : entities) {

    texBank.activate(e.gameItem.textureID);

    drawEntity(&e, primativeManager);

  }

  display.update();

}

void Renderer::drawEntity(Entity *e, PrimativeManager *pm)
{
  texBank.activate(e->gameItem.textureID);
  switch (e->gameItem.type) {
  case ENTITYTYPE::SQUARE:
    pm->drawCube(
      glm::vec3(e->gameItem.location[0], e->gameItem.location[1], e->gameItem.location[2]),
      glm::vec3(e->gameItem.scale[0], e->gameItem.scale[1], e->gameItem.scale[2])
    );
    break;
  case ENTITYTYPE::PLANE:
    pm->drawPlane(
      glm::vec3(e->gameItem.location[0], e->gameItem.location[1], e->gameItem.location[2]),
      glm::vec3(e->gameItem.scale[0], e->gameItem.scale[1], e->gameItem.scale[2])
    );
    break;
  default: break;
  }
}
