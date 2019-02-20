#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../globals.h"
#include "../sound/Sound.h"
#include "../player/Player.h"
#include "../util/mearly.h"
#include "../controls/Controls.h"

#include <iostream>
#include <cmath>

extern Controls controls;

void Game::mainLoop() {

  glm::mat4 model = glm::mat4(1.0f);

  while (!glfwWindowShouldClose(display.window)) {

    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    controls.keyboardInput(player, &pickups, deltaTime);

    camera.update();

    renderer.update(&pickups, &primativeManager, deltaTime);

    glfwPollEvents();

  }

}