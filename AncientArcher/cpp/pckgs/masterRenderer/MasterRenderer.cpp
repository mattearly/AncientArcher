#include "MasterRenderer.h"
#include <Display.h>
#include <glad/glad.h>
#include "../topDownPlayer/TopDownPlayer.h"

extern Display g_display;

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
//
//void MasterRenderer::update(float deltaTime, FirstPersonPlayer fpplayer, PrimativeRenderer prims, SkyboxRenderer sky)
//{
//}

void MasterRenderer::update(float deltaTime, FirstPersonPlayer* player, PrimativeRenderer* prims, SkyboxRenderer* sky)
{
  g_display.clear();

  prims->render();

  player->render();

  sky->render();

  g_display.update();
}

void MasterRenderer::update(float deltaTime, TopDownPlayer* tdp, PrimativeRenderer* prims, SkyboxRenderer* sky)
{
  g_display.clear();

  prims->render();

  sky->render();

  g_display.update();
}

void MasterRenderer::update(PrimativeRenderer* primRen, SideScrollPlayer* playerRen, Spawner* enemyRen, HealthBar* healthRen, SkyboxRenderer* skyRen, float deltaTime)
{
  g_display.clear();

  primRen->render();

  playerRen->render();

  enemyRen->render();

  healthRen->render();

  skyRen->render();

  g_display.update();

}
