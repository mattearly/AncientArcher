#include "SkyboxRenderer.h"
#include "Skybox.h"
#include <glad/glad.h>

SkyboxRenderer::SkyboxRenderer() {
  loadSkybox();
  skyboxShader = new Shader("../AncientArcher/resource/shaderSkyboxVert.glsl", "../AncientArcher/resource/shaderSkyboxFrag.glsl");
  texture = loader.loadCubeTexture(texture_files, skyboxShader);  //skyboxshader->use() set in this function
  skybox.loadProjectionMatrix(skyboxShader);
}

SkyboxRenderer::~SkyboxRenderer()
{
  delete skyboxShader;
}

void SkyboxRenderer::render()
{
  glDepthMask(GL_FALSE);
  glDepthRange(1.f, 1.f);
  skyboxShader->use();
  skybox.loadViewMatrix(skyboxShader);
  glBindVertexArray(skyboxVAO);
  glEnableVertexAttribArray(0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
  glDrawArrays(GL_TRIANGLES, 0, 18 * 6);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDepthRange(0.f, 1.f);
  glDepthMask(GL_TRUE);

}


void SkyboxRenderer::loadSkybox() {
  const float SIZE = 500.f;
  const float VERTICES[] = {
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

  /* set up an area to store vertex data */
  unsigned int skyVBO;

  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyVBO);

  glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

  glBindVertexArray(skyboxVAO);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

}