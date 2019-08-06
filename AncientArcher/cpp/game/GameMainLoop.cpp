#include "Game.h"
#include <Display.h>

extern Display g_display;

void Game::mainLoop() {
  while (!glfwWindowShouldClose(g_display.window))
  {
    static float currentFrame(0.f), deltaTime(0.f), lastFrame(0.f);
    currentFrame = (float)glfwGetTime();    // --- TIMING - UPDATE DELTA TIME
    deltaTime = currentFrame - lastFrame;   // --- don't touch this or the world
    lastFrame = currentFrame;               // --- may come crashing down


    update(deltaTime);                      // --- process events reliant on deltaTime


    render();                               // --- Draw the next frame


    update();                               // process events not reliant on deltaTime
  }
}

void Game::update(float dt)
{
}

void Game::render()
{
  g_display.clear();   // don't touch this

  world->render();
  sky->render();

  g_display.update();  // don't touch this
}


void Game::update()
{
  glfwPollEvents();    // --- Record Keypresses and Mouse Movement
  
  world->getCamera()->increaseYawAndPitch(mousepos->xPos, mousepos->yPos);

}
