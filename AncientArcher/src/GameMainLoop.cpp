#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

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

    /* update view matrix */
    glm::mat4 view = camera->getViewMatrix();
    shader->setMat4("view", view);

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture001);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture002);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture003);  
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture004);    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture005);  
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, texture006);  
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, texture007);

    shader->setBool("tex1", true);

    glm::mat4 model = glm::mat4(1.0f);
    for (unsigned int i = 0; i < 30; i++) {   //4 sides of area blocked with boxes
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(1.0f + 2.0f * i, -9.0f, -1.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(1.0f + 2.0f * i, -9.0f, 61.0f));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-1.0f, -9.0f, 1.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(61.0f, -9.0f, 1.0f + 2.0f * i));
      model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
      shader->setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);


    }

    /* grassy floor */
    shader->setBool("tex1", false);
    shader->setBool("tex4", true);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(30.0f, -10.0, 30.0f));
    model = glm::scale(model, glm::vec3(60.0f, 0.01f, 60.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    /* math wall */
    shader->setBool("tex4", false);
    shader->setBool("tex6", true);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(62.0f, 21.0f, 30.0f));
    model = glm::scale(model, glm::vec3(0.01f, 60.0f, 60.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);  
    
    /* meme wall */
    shader->setBool("tex6", false);
    shader->setBool("tex7", true);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 21.0f, 30.0f));
    model = glm::scale(model, glm::vec3(0.01f, 60.0f, 60.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //reset all texture
    shader->setBool("tex1", false);
    shader->setBool("tex2", false);
    shader->setBool("tex3", false);
    shader->setBool("tex4", false);
    shader->setBool("tex5", false);
    shader->setBool("tex6", false);
    shader->setBool("tex7", false);

    //glBindVertexArray(0);  // we don't really need to unbind it every time
    glfwSwapBuffers(window);
    glfwPollEvents();

  }

}