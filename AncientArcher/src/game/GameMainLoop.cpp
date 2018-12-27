#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Sound.h"
#include <iostream>
#include "../Player.h"
#include <cmath>
#include "../util/mearly.h"
#include "../camera/Camera.h"
#include "../displayManager/Display.h"
#include "../Controls.h"

extern Camera camera;
extern Display display;
extern Controls controls;

void Game::mainLoop() {

  glm::mat4 model = glm::mat4(1.0f);

  while (!glfwWindowShouldClose(display.window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    gameTime += deltaTime;

    controls.keyboardInput(player, &pickups, deltaTime, gameTime);
    
    camera.update();

    renderer.update(&primativeManager, deltaTime);
    
    glfwPollEvents();

  }

}