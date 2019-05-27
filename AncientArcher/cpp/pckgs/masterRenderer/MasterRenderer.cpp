#include "MasterRenderer.h"
#include <Display.h>
#include <glad/glad.h>
extern Display display;
MasterRenderer::MasterRenderer() {
  enableGLDepthTest();
}

MasterRenderer::~MasterRenderer() {}

void MasterRenderer::enableGLBlend() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MasterRenderer::disableGLBlend() {
  glDisable(GL_BLEND);
}

void MasterRenderer::enableGLDepthTest() {
  glEnable(GL_DEPTH_TEST);
}

void MasterRenderer::disableGLDepthTest() {
  glDisable(GL_DEPTH_TEST);
}

void MasterRenderer::update(PrimativeRenderer* primRen, SideScrollPlayer* playerRen, Spawner* enemyRen, HealthBar* healthRen, SkyboxRenderer* skyRen, float deltaTime)
{
  display.clear();

  primRen->render();

  playerRen->render();

  enemyRen->render();

  healthRen->render();

  skyRen->render();

  display.update();

}
