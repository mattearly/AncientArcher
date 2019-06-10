#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
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
	elapsedTime += deltaTime;
    lastFrame = currentFrame;
    // PROCESS PLAYER CONTROLS & MOVEMENT ---- //
    player->update(deltaTime);

	if (elapsedTime > 2.0f) {
		timeTrigger = !timeTrigger;
		elapsedTime = 0.0f;
	}

	for (auto &e : *(prims->getMovingEntites())) {
		e.moveBy(glm::vec3((sin(elapsedTime * 3.14159 / 180) * (timeTrigger ? 1.0 : -1.0)),
			0.f,
			0.f)
		);
	}

	//if (prims->getFirstMovingEntity.gameItem.loc.x == 9.f) {
	//}

    //  FINAL COLLISION CHECK
    if (player->moves.positionChanged)
    {
      player->finalCollisionCheck(prims->getEntites());
	  player->finalCollisionCheck(prims->getMovingEntites());
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