#pragma once
#include <mearly/Shader.h>
#include "AAViewport.h"
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class AASkybox
{
public:

  //AASkybox(std::shared_ptr<AAViewport>& viewport);
  AASkybox(std::vector<std::string> incomingSkymapFiles);

  void render();

private:

  unsigned int mSkyboxVAO = 0;
  void loadSkybox();

  unsigned int cubemapTexture = 0;
  std::unique_ptr< Shader > skyboxShader;
  void loadProjectionMatrix();
  void loadViewMatrix();

};