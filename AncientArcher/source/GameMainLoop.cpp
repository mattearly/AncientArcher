#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "globals.h"
#include "Sound.h"
#include "Player.h"
#include "Controls.h"
#include "mearly.h"

#include <iostream>
#include <cmath>

extern Controls controls;
extern Movement movedir;

void Game::mainLoop() {

  glm::mat4 model = glm::mat4(1.0f);

  toggleAmbientWindyNight();

  static float currentFrame;

  while (!glfwWindowShouldClose(display.window)) {

    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    player->update(deltaTime);

    renderer.update(player, &primativeManager, deltaTime);

    glfwPollEvents();

  }

}