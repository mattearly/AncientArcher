// derived from https://learnopengl.com/Advanced-OpenGL/Cubemaps
#include "AASkybox.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include "AAOGLGraphics.h"

/**
 * Custom skymap constructor. Loads up the files in the path and skymap shader.
 *
 * @param incomingSkymapFiles  A six png image cube map texture. The order must be: "right", "left", "up", "down", "front", "back"
 */
AASkybox::AASkybox(std::vector<std::string> incomingSkymapFiles)
{
  skyboxShader = std::make_unique<AAOGLShader>(
    "../shaders/skycube.vert",
    "../shaders/skycube.frag"
  );
  loadSkybox();
  cubemapTexture = TexLoader::getInstance()->loadCubeTexture(incomingSkymapFiles);
  skyboxShader->use();
  skyboxShader->setInt("skybox", 0);
  //loadProjectionMatrix();  //does on first render
}

/**
 * Renders the skybox behind all other visable objects.
 */
void AASkybox::render(const AACamera& cam)
{
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

  //if (AAViewport::getInstance()->hasViewportChanged())
  //{
  loadProjectionMatrix(cam);
  //}

  loadViewMatrix(cam);        // todo: only update when camera moved

  glBindVertexArray(mSkyboxVAO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  skyboxShader->use();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

void AASkybox::setProjectionMatrix(const AACamera& cam)
{
  loadProjectionMatrix(cam);
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
 * Sets the view matrix value on the skyboxShader from the getViewMatrix() function in AAViewport.
 * skyboxShader is in use after this call completes.
 */
void AASkybox::loadViewMatrix(const AACamera& cam)
{
  const glm::mat4 viewMatrix = glm::mat4(glm::mat3(cam.getViewMatrix()));
  skyboxShader->use();
  skyboxShader->setMat4("view", viewMatrix);
}

/**
 * Sets the projection matrix value on the skyboxShader from the getProjectionMatrix() function in AAViewport.
 * skyboxShader is in use after this call completes.
 */
void AASkybox::loadProjectionMatrix(const AACamera& cam)
{
  glm::mat4 projectionMatrix = cam.getProjectionMatrix();
  skyboxShader->use();
  skyboxShader->setMat4("projection", projectionMatrix);
}
