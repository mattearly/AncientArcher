#include "Renderer.h"
#include "../models/PrimativeManager.h"
#include "../Constraints.h"
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

  // FLOOR AND BOUNDRY CRATES
  //texBank.activate(1);

  //for (int i = 0; i < width; i += 1) {
  //  for (int j = 0; j < 4; j += 1) {

      //primativeManager->drawPlane(glm::vec3(-2.0f, 2.0f + j, 0.0f + i), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
      //primativeManager->drawPlane(glm::vec3(float(width + 1), 2.0f + j, 0.0f + i), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
      //primativeManager->drawPlane(glm::vec3(0.0f + i, 2.0f + j, -2.0f), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));
      //primativeManager->drawPlane(glm::vec3(0.0f + i, 2.0f + j, float(width + 1)), glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));

      //primativeManager->drawPlane(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f));
      //primativeManager->drawPlane(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f));
      //primativeManager->drawPlane(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f));
      //primativeManager->drawPlane(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    //}
  //}

  //for (int i = 0; i < width; i += 1) {
  //  for (int j = 0; j < width; j += 1) {
  //    primativeManager->drawPlane(glm::vec3(0.0f + i, 0.0f, 0.0f + j));
  //  }
  //}


  //primativeManager->drawPlane(glm::vec3(width/2, 0.0f, width/2), glm::vec3(width, 1, width));

  //texBank.activate(3);

  //primativeManager->drawCube(glm::vec3(0.f, 1.5f, 0.f), glm::vec3(1.0f, 2.0f, 1.0f));  //should still draw at 0, 1.5, 0
  //primativeManager->drawCube(glm::vec3(1.f, 1.5f, 1.f), glm::vec3(1.0f, 1.0f, 1.0f));  
  //primativeManager->drawCube(glm::vec3(2.f, 2.0f, 2.0f), glm::vec3(1.2f, 1.2f, 1.2f), glm::vec3(30.0f, 30.0f, 30.0f));
  //primativeManager->drawCube(glm::vec3(1.f, 1.5f, 1.f), glm::vec3(1.0f, 1.0f, 1.0f));  //should still draw at 1, 1.5, 1

  for (auto e : entities) {

    texBank.activate(e.squareItem.textureID);
    
    primativeManager->drawCube(
      glm::vec3(e.squareItem.location[0], e.squareItem.location[1], e.squareItem.location[2]),
      glm::vec3(e.squareItem.scale[0], e.squareItem.scale[1], e.squareItem.scale[2])
    );

  }

  pickups->draw(primativeManager);



  display.update();

}
