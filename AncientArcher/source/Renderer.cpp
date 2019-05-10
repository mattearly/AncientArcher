#include "Renderer.h"
#include "PrimativeManager.h"
#include "Player.h"
#include "Globals.h"
#include "SkyboxRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Renderer::Renderer() {
  width = round(ENGINE_LOGIC_CHECKING_DISTANCE + 1);
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

void Renderer::update(Player *player, PrimativeManager *primativeManager, SkyboxRenderer *skyboxRenderer, float deltaTime) 
{
  display.clear();
  
  //drawEntity(player->getEntity(), primativeManager);
  
  for (auto e : entities) {
    drawEntity(&e, primativeManager);
  }

  //skyboxRenderer->render();  // TODO make this work

  display.update();
}

void Renderer::drawEntity(Entity *e, PrimativeManager *pm)
{
  texBankShader.use();
  glActiveTexture(GL_TEXTURE0);
  //glBindTexture(GL_TEXTURE_2D, texBank.textureIDs[e->gameItem.textureID]);
  glBindTexture(GL_TEXTURE_2D, 32);
  
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(e->gameItem.location[0], e->gameItem.location[1], e->gameItem.location[2]));
  // trotations not supported yet
//  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(e->gameItem.scale[0], e->gameItem.scale[1], e->gameItem.scale[2]));
  texBankShader.setMat4("model", model);

  switch (e->gameItem.type) {
  case ENTITYTYPE::SQUARE:
    pm->drawCube();
    break;
  case ENTITYTYPE::PLANE:
    std::cout << "PLANE RENDERING not built\n";
    break;
  default: break;
  }
}
