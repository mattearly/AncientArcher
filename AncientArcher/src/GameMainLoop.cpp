#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Sound.h>
#include <iostream>

void Game::mainLoop() {
  playsuccesssound();

  glm::mat4 model = glm::mat4(1.0f);

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    control->keyboardInput(window, camera, deltaTime);

    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //shader->use();  //our only one, stays in use

    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);

    //glBindVertexArray(cubeVAO);  // bind vertex array, our only one, stays in use

    texBank.activate(shader, 3);

    for (unsigned int i = 0; i < 30; i++) {  

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(1.0f + 2.0f * i, 1.0f, -1.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      //model = glm::mat4(1.0f);
      //model = glm::translate(model, glm::vec3(1.0f + 2.0f * i, 1.0f, 61.0f));
      //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      //shader->setMat4("model", model);
      //glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 1.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      //model = glm::mat4(1.0f);
      //model = glm::translate(model, glm::vec3(61.0f, 1.0f, 1.0f + 2.0f * i));
      //model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      //shader->setMat4("model", model);
      //glDrawArrays(GL_TRIANGLES, 0, 36);

    }

    texBank.activate(shader, 0);  // grass
    // 60 x 60 floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(30.0f, 0.0f, 30.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    texBank.activate(shader, 7);  // gravel

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(90.0f, 0.0f, 30.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    texBank.activate(shader, 5);  //darkstone

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(90.0f, 0.0f, 90.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36); 
    
    texBank.activate(shader, 8);  // water

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(30.0f, 0.0f, 90.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);



    //// big wall 1
    //model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(62.0f, 32.0f, 30.0f));
    //model = glm::scale(model, glm::vec3(0.01f, 60.0f, 60.0f));
    //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //shader->setMat4("model", model);
    //glDrawArrays(GL_TRIANGLES, 0, 36);  
    //
    //// big wall 2
    //model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(-2.0f, 32.0f, 30.0f));
    //model = glm::scale(model, glm::vec3(0.01f, 60.0f, 60.0f));
    //model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //shader->setMat4("model", model);
    //glDrawArrays(GL_TRIANGLES, 0, 36);

    // middle box thing
    texBank.activate(shader, 6);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(25.0f, 4.0f, 25.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //glBindVertexArray(0);  // unbind vertex array, our only one, stays in use

    glfwSwapBuffers(window);
    glfwPollEvents();

  }

}