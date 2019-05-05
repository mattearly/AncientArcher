#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"

extern Camera camera;  // camera is instantiated as a global in Player.cpp

void Skybox::loadProjectionMatrix(Shader *skyboxShader)
{
  skyboxShader->use();
  glm::mat4 projectionMatrix = camera.getProjectionMatrix();

  skyboxShader->setMat4("projectionMatrix", projectionMatrix);
}

void Skybox::loadViewMatrix(Shader *skyboxShader)
{
  skyboxShader->use();
  //glm::mat4 viewMatrix = camera.getViewMatrix();

  // removes translation column
  glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.getViewMatrix()));

  skyboxShader->setMat4("viewMatrix", viewMatrix);

}
