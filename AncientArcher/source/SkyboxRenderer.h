#pragma once
#include "Skybox.h"
#include "Shader.h"
#include "TextureLoader.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

class SkyboxRenderer
{
public:
  SkyboxRenderer();
  ~SkyboxRenderer();
  void render();

private:

  unsigned int skyboxVAO;
  void loadSkybox();
  Shader* skyboxShader;
  TextureLoader loader;
  int texture;

  std::vector<std::string> texture_files = { 
    "skybox_heresy_right", 
    "skybox_heresy_left", 
    "skybox_heresy_up",
    "skybox_heresy_down", 
    "skybox_heresy_back",
    "skybox_heresy_front" 
  };
  Skybox skybox;
};

