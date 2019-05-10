#include "SkyboxRenderer.h"
#include "Shader.h"
#include "Camera.h"
#include "Globals.h"

#include <glad/glad.h>

extern Shader skyboxShader;
extern Camera camera;  // camera is instantiated as a global in Player.cpp

void SkyboxRenderer::loadProjectionMatrix()
{
  glm::mat4 projectionMatrix = camera.getProjectionMatrix();
  skyboxShader.use();
  skyboxShader.setMat4("projection", projectionMatrix);
  skyboxShader.stop();
}

void SkyboxRenderer::loadViewMatrix()
{
  glm::mat4 viewMatrix = camera.getViewMatrix();
  skyboxShader.use();
  skyboxShader.setMat4("view", viewMatrix);
}
SkyboxRenderer::SkyboxRenderer()
{
  loadSkybox();
  skyboxShader.use();
  cubemapTexture = loader.loadCubeTexture(skymapFiles);
  skyboxShader.setInt("skybox", 0);
}

SkyboxRenderer::~SkyboxRenderer()
{
}

void SkyboxRenderer::render()
{
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content  skyboxShader.use();
  skyboxShader.use();
  glActiveTexture(0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 32);
  glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
  skyboxShader.setMat4("view", view);
  glm::mat4 projection = camera.getProjectionMatrix();
  skyboxShader.setMat4("projection", projection);
  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

void SkyboxRenderer::loadSkybox() {
  float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
  };

  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


}