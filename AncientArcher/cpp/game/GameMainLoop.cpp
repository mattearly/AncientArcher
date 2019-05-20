#include <AAEngine.h>
#include "Game.h"
#include "../pckgs/firstPersonPlayer/Movement.h"
#include "../pckgs/masterRenderer/MasterRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
extern Movement movedir;
extern Camera camera;
void Game::mainLoop() {

  toggleAmbientWindyNight();

  static float currentFrame;

  while (!glfwWindowShouldClose(display.window)) {
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //player->update(deltaTime);

    // process player movement and collision
    //if (movedir.isMoving()) {
      //player->processCommands(deltaTime, primativeRenderer->getEntites());
      //player->movePlayerLight(*camera.getPosition(), primativeRenderer->getShader());
    //}

    masterRenderer.update(primativeRenderer, textRenderer, skyboxRenderer, deltaTime);

    glfwPollEvents();
  }
}