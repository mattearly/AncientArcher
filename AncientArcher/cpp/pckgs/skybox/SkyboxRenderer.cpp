#include "SkyboxRenderer.h"
#include <Camera.h>
#include <TextureLoader.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

extern Camera g_camera;  // camera is instantiated as a global in main.cpp

/**
 * Default Constructor. Load a cube map texture the default skybox and skymap shader.
 */
SkyboxRenderer::SkyboxRenderer()
{
  skyboxShader = std::make_unique< Shader >(
    "../AncientArcher/cpp/pckgs/skybox/skybox.vert",
    "../AncientArcher/cpp/pckgs/skybox/skybox.frag"
    );
  loadSkybox();

  TextureLoader loader;

  std::vector<std::string> skymapFiles =
  {
    "../AncientArcher/cpp/pckgs/skybox/default/skybox_right.jpg",
    "../AncientArcher/cpp/pckgs/skybox/default/skybox_left.jpg",
    "../AncientArcher/cpp/pckgs/skybox/default/skybox_top.jpg",
    "../AncientArcher/cpp/pckgs/skybox/default/skybox_bottom.jpg",
    "../AncientArcher/cpp/pckgs/skybox/default/skybox_front.jpg",
    "../AncientArcher/cpp/pckgs/skybox/default/skybox_back.jpg"
  };

  cubemapTexture = loader.loadCubeTexture(skymapFiles);

  skyboxShader->use();
  skyboxShader->setInt("skybox", 0);
}

/**
 * Custom skymap constructor. Loads up the files in the path and skymap shader.
 *
 * @param incomingSkymapFiles  A six image cube map texture.
 */
SkyboxRenderer::SkyboxRenderer(std::vector<std::string> incomingSkymapFiles)
{
  skyboxShader = std::make_unique< Shader >(
    "../AncientArcher/cpp/pckgs/skybox/skybox.vert",
    "../AncientArcher/cpp/pckgs/skybox/skybox.frag"
    );
  loadSkybox();

  TextureLoader loader;

  cubemapTexture = loader.loadCubeTexture(incomingSkymapFiles);

  // not sure if this is neccessary when only one texture
  skyboxShader->use();
  skyboxShader->setInt("skybox", 0);
}

/**
 * Renders the skybox behind all other visable objects.
 */
void SkyboxRenderer::render()
{
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

  loadProjectionMatrix();
  loadViewMatrix();

  glBindVertexArray(skyboxVAO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 * Loads up a cube that is pushed to the graphics card. skyboxVAO and skyboxVBO are populated with IDs.
 */
void SkyboxRenderer::loadSkybox() 
{
  const auto SIZE = 1.f;

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

/**
 * Sets the projection matrix value on the skyboxShader from the getProjectionMatrix() function in Camera.
 * skyboxShader is in use after this call completes.
 */
void SkyboxRenderer::loadProjectionMatrix()
{
  glm::mat4 projectionMatrix = g_camera.getProjectionMatrix();
  skyboxShader->use();
  skyboxShader->setMat4("projection", projectionMatrix);
}

/**
 * Sets the view matrix value on the skyboxShader from the getViewMatrix() function in Camera.
 * skyboxShader is in use after this call completes.
 */
void SkyboxRenderer::loadViewMatrix()
{
  glm::mat4 viewMatrix = glm::mat4(glm::mat3(g_camera.getViewMatrix())); // skybox never appears to move https://learnopengl.com/Advanced-OpenGL/Cubemaps
  skyboxShader->use();
  skyboxShader->setMat4("view", viewMatrix);
}
