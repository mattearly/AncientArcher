#include "Skybox.h"
#include "Shader.h"
#include "Camera.h"

extern Camera camera;

Skybox::Skybox()
{
  skyboxShader = new Shader("../AncientArcher/resource/shadeSkyboxVert.glsl", "../AncientArcher/resource/shaderSkyboxFrag.glsl");
}

Skybox::~Skybox()
{
  delete skyboxShader;
}

void Skybox::loadProjectionMatrix(glm::mat4 projectionMatrix)
{
  skyboxShader->setMat4("projectionMatrix", projectionMatrix);
}

void Skybox::loadViewMatrix()
{
  glm::mat4 viewMatrix = camera.getViewMatrix();
  skyboxShader->setMat4("viewMatrix", viewMatrix);
}
