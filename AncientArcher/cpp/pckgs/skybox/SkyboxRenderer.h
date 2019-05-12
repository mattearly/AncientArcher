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
  ~SkyboxRenderer();

  void render();

private:

  unsigned int skyboxVAO = 0;
  unsigned int skyboxVBO = 0;

  void loadSkybox();

  TextureLoader loader;

  std::unique_ptr< Shader > skyboxShader;

  unsigned int cubemapTexture = 0;

  std::vector<std::string> skymapFiles = {
    "../AncientArcher/cpp/packages/skybox/skybox_right.jpg",
    "../AncientArcher/cpp/packages/skybox/skybox_left.jpg",
    "../AncientArcher/cpp/packages/skybox/skybox_top.jpg",
    "../AncientArcher/cpp/packages/skybox/skybox_bottom.jpg",
    "../AncientArcher/cpp/packages/skybox/skybox_front.jpg",
    "../AncientArcher/cpp/packages/skybox/skybox_back.jpg"
  };

};
