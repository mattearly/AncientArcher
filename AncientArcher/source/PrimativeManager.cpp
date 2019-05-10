#include "PrimativeManager.h"
#include "Globals.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>

void PrimativeManager::drawCube()
{
  if (!cubeLoaded) {
    loadCube();
  }
  glBindVertexArray(cubeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void PrimativeManager::drawPlane()
{
  if (!planeLoaded) {
    loadPlane();
  }
  glBindVertexArray(planeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void PrimativeManager::loadCube() {
  // cube with texture coords and normals
  const float vertices[] = {
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

  glBindVertexArray(cubeVAO);

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

  // texture coord attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

  // normal attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

  // unbind buffers
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  cubeLoaded = true;

  std::cout << "cube Loaded to Graphics Card \n";

}

void PrimativeManager::loadPlane() {
  // plane with texture coords and normals
  float vertices[] = {
    // positions         // texture coords    // normals
    -0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,  1.0f, 1.0f, 0.0f,    1.0f, 0.0f,
     0.5f, 0.0f, 0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
     0.5f, 0.0f, 0.5f,   1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
    -0.5f, 0.0f, 0.5f,   0.0f, 0.0f, 0.0f,    1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,    1.0f, 0.0f
  };

  /* set up an area to store vertex data */
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);

  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(planeVAO);

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

  // texture coord attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

  // normal attribute
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  planeLoaded = true;
}

void PrimativeManager::drawCube(glm::vec3 location) {
  if (!cubeLoaded) {
    loadCube();
  }
  // textures should be set before calling this


  glBindVertexArray(cubeVAO);
  glEnableVertexAttribArray(0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, location);
  texBankShader.use();
  texBankShader.setMat4("model", model);

  glBindVertexArray(cubeVAO);
  //glEnableVertexAttribArray(0);

  glDrawArrays(GL_TRIANGLES, 0, 36);
  //glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}


void PrimativeManager::drawCube(glm::vec3 location, glm::vec3 scale) {
  if (!cubeLoaded) {
    loadCube();
  }
  // textures should be set before calling this

  glBindVertexArray(cubeVAO);
  glEnableVertexAttribArray(0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, location);
  model = glm::scale(model, scale);
  texBankShader.use();

  texBankShader.setMat4("model", model);

  glDrawArrays(GL_TRIANGLES, 0, 36);
  //glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void PrimativeManager::drawCube(glm::vec3 location, glm::vec3 scale, glm::vec3 rotation)
{
  if (!cubeLoaded) {
    loadCube();
  }
  // textures should be set before calling this

  glBindVertexArray(cubeVAO);
  glEnableVertexAttribArray(0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, location);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);
  texBankShader.use();

  texBankShader.setMat4("model", model);

  glDrawArrays(GL_TRIANGLES, 0, 36);
  //glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void PrimativeManager::drawPlane(glm::vec3 location) {
  if (!planeLoaded) {
    loadPlane();
  }
  // textures should be set before calling this

  glBindVertexArray(planeVAO);
  glEnableVertexAttribArray(0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, location);
  texBankShader.use();

  texBankShader.setMat4("model", model);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  //glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void PrimativeManager::drawPlane(glm::vec3 location, glm::vec3 scale) {
  if (!planeLoaded) {
    loadPlane();
  }
  // textures should be set before calling this

  glBindVertexArray(planeVAO);
  glEnableVertexAttribArray(0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, location);
  model = glm::scale(model, scale);
  texBankShader.use();

  texBankShader.setMat4("model", model);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  //glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void PrimativeManager::drawPlane(glm::vec3 location, glm::vec3 scale, glm::vec3 rotation) {
  if (!planeLoaded) {
    loadPlane();
  }
  // textures should be set before calling this

  glBindVertexArray(planeVAO);
  glEnableVertexAttribArray(0);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, location);
  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  texBankShader.use();

  model = glm::scale(model, scale);
  texBankShader.setMat4("model", model);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  //glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}
