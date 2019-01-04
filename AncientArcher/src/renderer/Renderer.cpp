#include "Renderer.h"
#include "../models/PrimativeManager.h"
#include "../displayManager/Display.h"
#include "../shaders/Shader.h"
#include "../util/TextureBank.h"
#include "../Constraints.h"
#include "../lighting/Lighting.h"
#include "../Pickups.h"


extern Display display;
extern Shader shader;
extern TextureBank texBank;

Renderer::Renderer() {}

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
  //float worldcenter = world_width / 2.0f;

  display.clear();

  texBank.activate(0);
  //primativeManager->drawPlane(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
  int width = round(world_width + 1);


  texBank.activate(1);
  for (int i = 0; i < width; i += 2) {
    for (int j = 0; j < 4; j += 2) {
      primativeManager->drawCube(glm::vec3(-2.0f, 2.0f + j, 0.0f + i), glm::vec3(2.0f, 2.0f, 2.0f));
      primativeManager->drawCube(glm::vec3(float(width + 1), 2.0f + j, 0.0f + i), glm::vec3(2.0f, 2.0f, 2.0f));
      primativeManager->drawCube(glm::vec3(0.0f + i, 2.0f + j, -2.0f), glm::vec3(2.0f, 2.0f, 2.0f));
      primativeManager->drawCube(glm::vec3(0.0f + i, 2.0f + j, float(width + 1)), glm::vec3(2.0f, 2.0f, 2.0f));
    }
  }
  for (int i = 0; i < width; i+=2) {
    for (int j = 0; j < width; j+=2) {
      primativeManager->drawCube(glm::vec3(0.0f + i, 0.0f, 0.0f + j), glm::vec3(2.0f, 2.0f, 2.0f));
    }
  }

  pickups->draw(primativeManager);


  //texBank.activate(15);   // thing near middle
  //for (int i = 0; i < width; i += 2) {
    //for (int j = 0; j < width; j += 2) {
      //primativeManager->drawCube(glm::vec3(10.0f, 2.0f, 10.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    //}
  //}

  //texBank.activate(8);    //sky
  //for (int i = 0; i < width; i+=2) {
  //  for (int j = 0; j < width; j += 2) {
  //		primativeManager->drawCube(glm::vec3(0.0f + i, world_height, 0.0f + j), glm::vec3(2.0f, 2.0f, 2.0f));
  //	}
  //}


  display.update();

}
