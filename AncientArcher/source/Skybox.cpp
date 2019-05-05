#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"

extern Camera camera;  // camera is instantiated as a global in Player.cpp

void Skybox::loadProjectionMatrix(Shader *skyboxShader)
{
  glm::mat4 projectionMatrix = camera.getProjectionMatrix();
  skyboxShader->setMat4("projectionMatrix", projectionMatrix);
}

void Skybox::loadViewMatrix(Shader *skyboxShader)
{
  glm::mat4 viewMatrix = camera.getViewMatrix();
  skyboxShader->setMat4("viewMatrix", viewMatrix);
}
