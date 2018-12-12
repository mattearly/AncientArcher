#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Sound.h>
#include <iostream>
#include "Player.h"
#include <cmath>
#include <mearly.h>


void Game::mainLoop() {
  playsuccesssound();

  glm::mat4 model = glm::mat4(1.0f);

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    control->keyboardInput(window, camera, player, &pickups, deltaTime);

    glClearColor(0.2f, 0.3f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //shader->use();  //our only one, stays in use

    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);

    //glBindVertexArray(cubeVAO);  // bind vertex array, our only one, stays in use

    //PLAYER MODEL STUFF
    switch (player->getSelectedWeapon()) {
    case 1:
      texBank.activate(shader, 14);  // metal / sword
      break;
    case 2:
      texBank.activate(shader, 2);  // wood / bow
      break;
    }

    if (player->getSelectedWeapon() != 0) {
      // draw selected weapon
      model = glm::mat4(1.0f);

      // player loc            // move out front              // move to right                //move down some               //move model                 // move based on pitch needed here - Quaternion?
      model = glm::translate(model, *camera->getPosition() + (*camera->getFront() * 0.4f) + (*camera->getRight() * 0.15f) - glm::vec3(0.0f, 0.05f, 0.0f) /*+ glm::vec3(0.0f, cos(glm::radians(camera->getPitch())), 0.0f)*/);

      model = glm::rotate(model, glm::radians(-camera->getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));

      model = glm::scale(model, glm::vec3(0.02f, 0.35f, 0.01f));

      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

    }


    // WORLD MODEL STUFF

    texBank.activate(shader, 0);  // grass : 60 x 60 floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(30.0f, 0.0f, 30.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    texBank.activate(shader, 3);  //shrubbery     grass area walls
    for (unsigned int i = 0; i < 30; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(1.0f + 2.0f * i, 1.0f, -1.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 1.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //

    texBank.activate(shader, 7);  // gravel : 60 x 60 floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(90.0f, 0.0f, 30.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    texBank.activate(shader, 5);  //darkstone   gravel area wall
    for (unsigned int i = 0; i < 30; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(61.0f + 2.0f * i, 1.0f, -1.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(121.0f, 1.0f, 1.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //

    texBank.activate(shader, 10);  //mud : 60 x 60 floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(90.0f, 0.0f, 90.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36); 

    texBank.activate(shader, 11);  // darkwood   mud area wall
    for (unsigned int i = 0; i < 30; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(61.0f + 2.0f * i, 1.0f, 121.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(121.0f, 1.0f, 61.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //
    
    texBank.activate(shader, 13);  // snow : 60 x 60 area
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(30.0f, 0.0f, 90.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    texBank.activate(shader, 7);  // gravel wall for snow area
    for (unsigned int i = 0; i < 30; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(1.0f + 2.0f * i, 1.0f, 121.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 61.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

    }

    //

    if (pickups.speedBoostAvail) {
      // speedboost pickup
      texBank.activate(shader, 12);
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(pickups.boostLoc.x, 1.0f, pickups.boostLoc.y));
      model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    //

    //glBindVertexArray(0);  // unbind vertex array, our only one, stays in use

    glfwSwapBuffers(window);
    glfwPollEvents();

  }

}