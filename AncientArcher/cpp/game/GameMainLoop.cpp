#include <AAEngine.h>

#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Game::mainLoop() {

  toggleAmbientWindyNight();

  static float currentFrame;

  while (!glfwWindowShouldClose(display.window)) {
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    player->update(deltaTime);

    masterRenderer.update(primativeRenderer, skyboxRenderer, deltaTime);

    glfwPollEvents();
  }
}