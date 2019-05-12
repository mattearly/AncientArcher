#pragma once
#include <AAEngine.h>
#include <vector>
#include <string>
#include <memory>

class SkyboxRenderer
{
public:
  
  void loadProjectionMatrix();
  void loadViewMatrix();

  SkyboxRenderer();

  void render();

private:

  unsigned int skyboxVAO = 0;
  unsigned int skyboxVBO = 0;

  void loadSkybox();

  TextureLoader loader;

  std::unique_ptr< Shader > skyboxShader;

  unsigned int cubemapTexture = 0;

  std::vector<std::string> skymapFiles = {
    "../AncientArcher/cpp/pckgs/skybox/skybox_right.jpg",
    "../AncientArcher/cpp/pckgs/skybox/skybox_left.jpg",
    "../AncientArcher/cpp/pckgs/skybox/skybox_top.jpg",
    "../AncientArcher/cpp/pckgs/skybox/skybox_bottom.jpg",
    "../AncientArcher/cpp/pckgs/skybox/skybox_front.jpg",
    "../AncientArcher/cpp/pckgs/skybox/skybox_back.jpg"
  };

};
