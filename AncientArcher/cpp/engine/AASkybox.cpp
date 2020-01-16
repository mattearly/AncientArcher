#include "AASkybox.h"
#include <mearly/TexLoader.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

/**
 * Custom skymap constructor. Loads up the files in the path and skymap shader.
 *
 * @param incomingSkymapFiles  A six image cube map texture.
 */
AASkybox::AASkybox(std::vector<std::string> incomingSkymapFiles)
{
  skyboxShader = std::make_unique< Shader >(
    "../AncientArcher/shader/vert_skybox.glsl",
    "../AncientArcher/shader/frag_skybox.glsl"
    );

  loadSkybox();

  cubemapTexture = TexLoader::getInstance()->loadCubeTexture(incomingSkymapFiles);

  skyboxShader->use();
  skyboxShader->setInt("skybox", 0);

  loadProjectionMatrix(); 

}

/**
 * Renders the skybox behind all other visable objects.
 */
void AASkybox::render()
{
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

  if (AAViewport::getInstance()->hasViewportChanged())
  {
    loadProjectionMatrix();
  }

  
  loadViewMatrix();        // todo: only update when camera moved

  glBindVertexArray(mSkyboxVAO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  //skyboxShader->use();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 * Loads up a cube that is pushed to the graphics card. skyboxVAO and skyboxVBO are populated with IDs.
 */
void AASkybox::loadSkybox()
{
  const float SIZE = 1.f;

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

  //todo: use element buffers
  glGenVertexArrays(1, &mSkyboxVAO);

  unsigned int skyboxVBO = 0;

  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(mSkyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

/**
 * Sets the projection matrix value on the skyboxShader from the getProjectionMatrix() function in AAViewport.
 * skyboxShader is in use after this call completes.
 */
void AASkybox::loadProjectionMatrix()
{
  glm::mat4 projectionMatrix = AAViewport::getInstance()->getProjectionMatrix();
  skyboxShader->use();
  skyboxShader->setMat4("projection", projectionMatrix);
}

/**
 * Sets the view matrix value on the skyboxShader from the getViewMatrix() function in AAViewport.
 * skyboxShader is in use after this call completes.
 */
void AASkybox::loadViewMatrix()
{
  glm::mat4 viewMatrix = glm::mat4(glm::mat3(AAViewport::getInstance()->getViewMatrix())); // skybox never appears to move https://learnopengl.com/Advanced-OpenGL/Cubemaps
  skyboxShader->use();
  skyboxShader->setMat4("view", viewMatrix);
}