#include "PrimativeRenderer.h"

#include <AAEngine.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>

extern Camera camera;

void PrimativeRenderer::drawCube()
{
  if (!cubeLoaded) {
    loadCube();
  }
  glBindVertexArray(cubeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void PrimativeRenderer::drawPlane()
{
  if (!planeLoaded) {
    loadPlane();
  }
  glBindVertexArray(planeVAO);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

PrimativeRenderer::PrimativeRenderer()
{
  primShader = std::make_unique< Shader >(
    "../AncientArcher/cpp/packages/primatives/primative.vert",
    "../AncientArcher/cpp/packages/primatives/primative.frag"
    );
  lighting.setConstantLight(primShader.get());
  lighting.addPointLight(*camera.getPosition(), primShader.get());
  primShader->use();
  glm::mat4 proj = camera.getProjectionMatrix();
  primShader->setMat4("projection", proj);
}

void PrimativeRenderer::render()
{
  primShader->use();
  camera.update(primShader.get());
  for (auto e : entities)
  {
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texBank.textureIDs[e->gameItem.textureID]);
    glBindTexture(GL_TEXTURE_2D, e.gameItem.textureID);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(e.gameItem.location[0], e.gameItem.location[1], e.gameItem.location[2]));

    // rotations
  //  model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  //  model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  //  model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, glm::vec3(e.gameItem.scale[0], e.gameItem.scale[1], e.gameItem.scale[2]));
    primShader.get()->setMat4("model", model);

    switch (e.gameItem.type) {
    case ENTITYTYPE::SQUARE:
      drawCube();
      break;
    case ENTITYTYPE::PLANE:
      std::cout << "PLANE RENDERING not built\n";
      break;
    default: break;
    }
  }
}

void PrimativeRenderer::addToPrimativeEntities(Entity entity)
{
  entities.push_back(entity);
}

void PrimativeRenderer::loadCube() {
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

void PrimativeRenderer::loadPlane() {
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
