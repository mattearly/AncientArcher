#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

void Game::mainLoop() {

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //std::cout << deltaTime << std::endl;

    control->keyboardInput(window, camera, deltaTime);

    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    shader->use();

    ///* update projection matrix  - this may rarely chance and could be more efficient
    //   by not being in the main loop */
    //glm::mat4 projection = glm::perspective(glm::radians(camera->FoV), 
    //  (float)window_width / (float)window_height, 0.1f, 100.0f);
    //shader->setMat4("projection", projection);

    /* update view matrix */
    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);


    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture001);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture002);

    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, float(glfwGetTime()) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader->setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //glBindVertexArray(0);  // we don't really need to unbind it every time
    glfwSwapBuffers(window);
    glfwPollEvents();

  }
}