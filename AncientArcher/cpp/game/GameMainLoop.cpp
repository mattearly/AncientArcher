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
    // --- TIMING - UPDATE DELTA TIME
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
  
    // --- RENDER

    
    

    // --- RECORD KEYPRESSES FOR NEXT FRAME
    glfwPollEvents();
  }
}