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
    lastFrame = currentFrame;
    // PROCESS PLAYER CONTROLS & MOVEMENT ---- //
    player->update(deltaTime);

    // PROCESS PRIMATIVES MOVEMENT ---- //
    prims->update(deltaTime);

    //  FINAL COLLISION CHECK
    if (player->moves.isMoving())
    {
      player->finalCollisionCheck(prims->getEntites());
      player->finalCollisionCheck(prims->getMovingEntites());
    }



    // PLAYER SYNC
    player->syncCam();

    // RENDER EVERYTHING ---- //
    masterRenderer.update(deltaTime, player, prims, sky);
    // RECORD KEYPRESSES FOR NEXT FRAME ---- //
    glfwPollEvents();
  }
}