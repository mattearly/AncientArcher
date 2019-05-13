#pragma once
#include <Shader.h>
#include <vector>
#include <string>
#include <memory>

class SkyboxRenderer
{
public:
  
  SkyboxRenderer();
  SkyboxRenderer(std::vector<std::string> incomingSkymapFiles);

  void render();

private:

  unsigned int skyboxVAO = 0;
  unsigned int skyboxVBO = 0;
  void loadSkybox();

  unsigned int cubemapTexture = 0;
  std::unique_ptr< Shader > skyboxShader;
  void loadProjectionMatrix();
  void loadViewMatrix();

};
