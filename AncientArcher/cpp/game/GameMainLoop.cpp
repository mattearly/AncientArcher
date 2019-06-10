#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Display.h>
#include <CollisionHandler.h>
#include <vector>
#include <Entity.h>

extern Display g_display;

void Game::mainLoop() {
  static float currentFrame;
  while (!glfwWindowShouldClose(g_display.window))
  {
    // TIMING - UDPATE DELTA TIME ---- // 
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // PROCESS PLAYER CONTROLS & MOVEMENT ---- //
    player->update(deltaTime);

	//if (prims->getFirstMovingEntity.gameItem.loc.x == 9.f) {
	//}

    //  FINAL COLLISION CHECK
    if (player->moves.positionChanged)
    {
      player->finalCollisionCheck(prims->getEntites());
    }
    // MOVE CAMERA TO PROPER LOCATION
    player->syncCam();
    player->syncPlayerLight(prims->getShader());
    // RENDER EVERYTHING ---- //
    masterRenderer.update(deltaTime, player, prims, sky);
    // RECORD KEYPRESSES N STUFF FOR NEXT FRAME ---- //
    glfwPollEvents();
  }
}