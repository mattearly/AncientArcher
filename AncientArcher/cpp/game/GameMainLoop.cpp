#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Display.h>

extern Display g_display;

void Game::mainLoop() {
  static float currentFrame;
  while (!glfwWindowShouldClose(g_display.window)) 
  {
    // TIMING - UDPATE DELTA TIME ---- // 
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // PROCESS PLAYER CONTROLS ---- //
    player->update(deltaTime);



    // RENDER EVERYTHING ---- //
    masterRenderer.update(deltaTime, player, prims, sky);



    // RECORD KEYPRESSES N STUFF FOR NEXT FRAME ---- //
    glfwPollEvents();
  }

}