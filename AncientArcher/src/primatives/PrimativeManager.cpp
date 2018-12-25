#include "PrimativeManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../shaders/Shader.h"
#include "../util/TextureBank.h"
#include <cmath>

extern Shader shader;
extern TextureBank texBank;

PrimativeManager::PrimativeManager() {
  cubeLoaded = false;
}

PrimativeManager::~PrimativeManager() {}

void PrimativeManager::loadCube() {
  // cube with texture coords and normals
  float vertices[] = {
    // positions           // texture coords  // normals
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,        0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,        0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,        0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,        0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,        0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,        0.0f, 0.0f, -1.0f,
                         
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,        0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,        0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,        0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,        0.0f,  0.0f,  1.0f,
                         
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,       -1.0f,  0.0f,  0.0f,
                         
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f,        1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f,        1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,        1.0f,  0.0f,  0.0f,
                         
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,        0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,        0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,        0.0f, -1.0f,  0.0f,
                         
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,        0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,        0.0f,  1.0f,  0.0f
  };                     

  /* set up an area to store vertex data */
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);

  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // normal attribute
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
 
  cubeLoaded = true;

  std::cout << "cube Loaded to Graphics Card \n";

}

void PrimativeManager::drawCube(float deltaTime) {
  if (!cubeLoaded) {
    loadCube();
  }
  
  glBindVertexArray(cubeVAO);
  //glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  //glEnableVertexAttribArray(0);
  
  glm::mat4 model = glm::mat4(1.0f);

  //model = glm::rotate(model, glm::radians(50.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
  //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  //model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

  shader.setMat4("model", model);

  glDrawArrays(GL_TRIANGLES, 0, 36);

}

void PrimativeManager::drawCube(glm::vec3 location, glm::vec3 scale) {
  if (!cubeLoaded) {
    loadCube();
  }

  glBindVertexArray(cubeVAO);

  glm::mat4 model = glm::mat4(1.0f);

  //model = glm::rotate(model, glm::radians(50.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));

  model = glm::translate(model, location);

  model = glm::scale(model, scale);

  shader.setMat4("model", model);

  glDrawArrays(GL_TRIANGLES, 0, 36);
}
