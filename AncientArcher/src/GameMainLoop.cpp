#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void Game::mainLoop() {
  glm::mat4 model = glm::mat4(1.0f);

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //std::cout << deltaTime << std::endl;

    control->keyboardInput(window, camera, deltaTime);

    glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* switch to light shader and adjust values */
    lightShader->use();
    lightShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f); 
    lightShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f); // white
    lightShader->setVec3("lightPos", light.lightPos);

    // view/projection transformations - might only need to do this once if it doesn't change
    glm::mat4 projection = glm::perspective(glm::radians(camera->FoV),
      (float)window_width / (float)window_height, 0.1f, 200.0f);
    lightShader->setMat4("projection", projection);

    glm::mat4 view = camera->getViewMatrix();
    lightShader->setMat4("view", view);

    /* render light projection object*/

          // world transformation
    model = glm::mat4(1.0f);
    lightShader->setMat4("model", model);

    // render the cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    /* switch to object shader and adjust values */
    shader->use();
    /* update view matrix */
    //already got the view matrix just need to update this shader too
    //glm::mat4 view = camera->getViewMatrix();  
    shader->setMat4("view", view);

    //was drawing light object here

    /* draw lamp light source cube */
           // also draw the lamp object
    model = glm::mat4(1.0f);
    model = glm::translate(model, light.lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    shader->setMat4("model", model);

    //glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //glBindVertexArray(cubeVAO); 

    shader->setUint("texnum", 0);

    //glm::mat4 model = glm::mat4(1.0f);  //moved to top of function
    for (unsigned int i = 0; i < 30; i++) {   //4 sides of area blocked with boxes
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(1.0f + 2.0f * i, 1.0f, -1.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(1.0f + 2.0f * i, 1.0f, 61.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 1.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(61.0f, 1.0f, 1.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

    }

    ///* grassy floor */
    shader->setUint("texnum", 3);


    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(30.0f, 0.0f, 30.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ///* math wall */
    shader->setUint("texnum", 5);


    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(62.0f, 32.0f, 30.0f));
    model = glm::scale(model, glm::vec3(0.01f, 60.0f, 60.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);  
    
    ///* meme wall */
    shader->setUint("texnum", 6);


    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, 32.0f, 30.0f));
    model = glm::scale(model, glm::vec3(0.01f, 60.0f, 60.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    ////reset all texture
    //shader->setBool("tex1", false);
    //shader->setBool("tex2", false);
    //shader->setBool("tex3", false);
    //shader->setBool("tex4", false);
    //shader->setBool("tex5", false);
    //shader->setBool("tex6", false);
    //shader->setBool("tex7", false);

    glBindVertexArray(0);  // unbind vertex array

    glfwSwapBuffers(window);
    glfwPollEvents();

  }

}