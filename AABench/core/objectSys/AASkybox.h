#pragma once
#include "../cameraSys/AACamera.h"
#include "shaderSys/AAOGLShader.h"
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

class AASkybox
{
public:

  //AASkybox(std::shared_ptr<AACamera>& viewport);
  //AASkybox(std::vector<std::string> incomingSkymapFiles);

  AASkybox(std::vector<std::string> incomingSkymapFiles);

  void render(const AACamera& cam);

  //void render(const AAOGLShader& shader);

  // to be manually called as needed
  void setProjectionMatrix(const AACamera& cam);

private:

  void loadSkybox();
  void loadProjectionMatrix(const AACamera& cam);
  void loadViewMatrix(const AACamera& cam);

  unsigned int mSkyboxVAO = 0;
  unsigned int cubemapTexture = 0;
  std::unique_ptr<AAOGLShader> skyboxShader;
  //std::shared_ptr<AACamera>& mCamera;

};