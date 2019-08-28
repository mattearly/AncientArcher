#pragma once
#include <Shader.h>
#include <Camera.h>
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class Skybox
{
public:

  //Skybox(std::shared_ptr<Camera>& camera);
  Skybox(std::shared_ptr<Camera>& camera, std::vector<std::string> incomingSkymapFiles);

  void render();

private:

  unsigned int skyboxVAO = 0;
  unsigned int skyboxVBO = 0;
  void loadSkybox();

  unsigned int cubemapTexture = 0;
  std::unique_ptr< Shader > skyboxShader;
  void loadProjectionMatrix();
  void loadViewMatrix();

  std::shared_ptr<Camera> _camera;

};
