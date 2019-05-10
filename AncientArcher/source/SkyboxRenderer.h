#pragma once
#include "TextureLoader.h"
#include <vector>
#include <string>

class SkyboxRenderer
{
public:
  
  void loadProjectionMatrix();
  void loadViewMatrix();

  SkyboxRenderer();
  ~SkyboxRenderer();

  void render();

private:

  unsigned int skyboxVAO = 0;
  unsigned int skyboxVBO = 0;

  void loadSkybox();

  TextureLoader loader;

  unsigned int cubemapTexture = 0;

  std::vector<std::string> skymapFiles = {
    "skybox/skybox_right.jpg",
    "skybox/skybox_left.jpg",
    "skybox/skybox_top.jpg",
    "skybox/skybox_bottom.jpg",
    "skybox/skybox_front.jpg",
    "skybox/skybox_back.jpg"
  };

};
