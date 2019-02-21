#include "Renderer.h"
#include "../models/PrimativeManager.h"
#include "../pickups/Pickups.h"
#include "../globals.h"

Renderer::Renderer() {
  width = round(world_width + 1);
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

void Renderer::update(Pickups *pickups, PrimativeManager *primativeManager, float deltaTime) {

  display.clear();

  for (auto const & e : entities) {   // looping through as const reference for efficiency and this shouldn't be modified

    texBank.activate(e.gameItem.textureID);

    switch (e.gameItem.type) {
    case ENTITYTYPE::SQUARE:
      primativeManager->drawCube(
        glm::vec3(e.gameItem.location[0], e.gameItem.location[1], e.gameItem.location[2]),
        glm::vec3(e.gameItem.scale[0], e.gameItem.scale[1], e.gameItem.scale[2])
      );
      break;
      
    case ENTITYTYPE::PLANE:
      primativeManager->drawPlane(
        glm::vec3(e.gameItem.location[0], e.gameItem.location[1], e.gameItem.location[2]),
        glm::vec3(e.gameItem.scale[0], e.gameItem.scale[1], e.gameItem.scale[2])
      );
      break;

    default: break;
    }
  }

  pickups->draw(primativeManager);

  display.update();

}
