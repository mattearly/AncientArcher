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

    update(deltaTime);                 // --- process world logic

    render();                          // --- Draw the next frame

    glfwPollEvents();                       // --- Record Keypresses and Mouse Movement
  }
}

void Game::update(float dt)
{
}

void Game::render()
{
  g_display.clear();


  world->render();
  sky->render();



  g_display.update();
}
