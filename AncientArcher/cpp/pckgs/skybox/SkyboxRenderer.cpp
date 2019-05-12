#include "SkyboxRenderer.h"
#include "Shader.h"
#include "Camera.h"

#include <glad/glad.h>

extern Camera camera;  // camera is instantiated as a global in Player.cpp

SkyboxRenderer::SkyboxRenderer()
{
  skyboxShader = std::make_unique< Shader >(
      "../AncientArcher/cpp/pckgs/skybox/skybox.vert",
      "../AncientArcher/cpp/pckgs/skybox/skybox.frag"
    );
  loadSkybox();
  cubemapTexture = loader.loadCubeTexture(skymapFiles);
  skyboxShader->use();
  skyboxShader->setInt("skybox", 0);
}

void SkyboxRenderer::loadProjectionMatrix()
{
  glm::mat4 projectionMatrix = camera.getProjectionMatrix();
  skyboxShader->use();
  skyboxShader->setMat4("projection", projectionMatrix);
}

void SkyboxRenderer::loadViewMatrix()
{
  glm::mat4 viewMatrix = camera.getViewMatrix();
  skyboxShader->use();
  skyboxShader->setMat4("view", viewMatrix);
}

void SkyboxRenderer::render()
{
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content  skyboxShader.use();
  
  loadProjectionMatrix();
  loadViewMatrix();
/*
  skyboxShader->use();
  camera.update(skyboxShader.get());
  glm::mat4 projection = camera.getProjectionMatrix();
  skyboxShader->setMat4("projection", projection);
*/
  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

void SkyboxRenderer::loadSkybox() {

  // size should be larger than the explorable world, as the skybox doesn't move with the player currently
  const float SIZE = 500.0f;

  float skyboxVertices[] = {
    // positions          
    -SIZE,  SIZE, -SIZE,
    -SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,

    -SIZE, -SIZE,  SIZE,
    -SIZE, -SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE,  SIZE,
    -SIZE, -SIZE,  SIZE,

     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,

    -SIZE, -SIZE,  SIZE,
    -SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE, -SIZE,  SIZE,
    -SIZE, -SIZE,  SIZE,

    -SIZE,  SIZE, -SIZE,
     SIZE,  SIZE, -SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
    -SIZE,  SIZE,  SIZE,
    -SIZE,  SIZE, -SIZE,

    -SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE,  SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE,  SIZE,
     SIZE, -SIZE,  SIZE
  };

  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}
