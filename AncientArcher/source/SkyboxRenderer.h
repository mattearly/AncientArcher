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

  // vao to switch to during render
  unsigned int skyboxVAO = 0;
  // vbo to store the index
  unsigned int skyVBO = 0;

  void loadSkybox();

  Shader* skyboxShader;

  TextureLoader loader;

  // textureID to switch to during render
  int cubeMapTextureID = 0;

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

