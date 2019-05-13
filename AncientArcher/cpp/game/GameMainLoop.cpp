#include <AAEngine.h>

#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../pckgs/firstPersonPlayer/Movement.h"
#include <glm/gtc/type_ptr.hpp>
extern Movement movedir;
void Game::mainLoop() {

  toggleAmbientWindyNight();

  static float currentFrame;

  while (!glfwWindowShouldClose(display.window)) {
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    player->update(deltaTime);

    // process player movement and colliion
    if (movedir.isMoving()) {
      player->processCommands( deltaTime, primativeRenderer->getEntites() );
    }

    masterRenderer.update(primativeRenderer, skyboxRenderer, deltaTime);

    glfwPollEvents();
  }
}