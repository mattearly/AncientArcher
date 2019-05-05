#include "SkyboxRenderer.h"
#include "Skybox.h"
#include <glad/glad.h>

SkyboxRenderer::SkyboxRenderer()
{
  skyboxShader = new Shader(
    "../AncientArcher/resource/shaderSkyboxVert.glsl",
    "../AncientArcher/resource/shaderSkyboxFrag.glsl"
  );

  cubeMapTextureID = loader.loadCubeTexture(texture_files, skyboxShader);

  loadSkybox();

  //skybox.loadProjectionMatrix(skyboxShader);

}

SkyboxRenderer::~SkyboxRenderer()
{
  delete skyboxShader;
}

void SkyboxRenderer::render()
{
  //glDepthMask(GL_FALSE);
  //glDepthRange(1.f, 1.f);
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

  skyboxShader->use();
  skybox.loadProjectionMatrix(skyboxShader);  // only needs called on screen size and fov changes
  skybox.loadViewMatrix(skyboxShader);

  glBindVertexArray(skyboxVAO);
  //glEnableVertexAttribArray(0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  //skyboxShader->stop();
  //glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  //glDepthRange(0.f, 1.f);
  //glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS); // set depth function back to default

}

void SkyboxRenderer::loadSkybox() {
  const float SIZE = 500.0f;
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

  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // unbind buffers
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glBindVertexArray(0);

}