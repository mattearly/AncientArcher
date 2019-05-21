#include "MasterRenderer.h"
#include "../primatives/PrimativeRenderer.h"
#include "../skybox/SkyboxRenderer.h"
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

void MasterRenderer::update(PrimativeRenderer* primRen, SideScrollPlayer* playerRen, TextRenderer* textRen, SkyboxRenderer* skyRen, float deltaTime)
{
  display.clear();

  primRen->render();

  playerRen->render();

  skyRen->render();

  //textRen->renderText(0, 0, "this is a test");

  display.update();
}

