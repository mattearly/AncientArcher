#include "Game.h"
#include <Display.h>

extern Display g_display;

void Game::mainLoop() {
  static float currentFrame;
  while (!glfwWindowShouldClose(g_display.window))
  {
    currentFrame = (float)glfwGetTime();    // --- TIMING - UPDATE DELTA TIME
    deltaTime = currentFrame - lastFrame;   // --- don't touch this or the world
    lastFrame = currentFrame;               // --- may come crashing down
   

    // --- RENDER
    g_display.clear();
    player->update();
    world->update(deltaTime);  // updates moving entites if there are any
    world->render();
    sky->render();

    g_display.update();
    
    // --- RECORD KEYPRESSES FOR NEXT FRAME
    glfwPollEvents();
  }
}