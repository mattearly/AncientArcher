#pragma once
#include "../../Scene/Camera.h"
#include "OGLShader.h"
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace AA
{
class Skybox
{
public:

  //Skybox(std::shared_ptr<Camera>& viewport);
  //Skybox(std::vector<std::string> incomingSkymapFiles);

  Skybox(std::vector<std::string> incomingSkymapFiles, bool useInternalShaders = true, const char* vertpath = "", const char* fragpath = "");

  void render(const Camera& cam);

  //void render(const AAOGLShader& shader);

  // to be manually called as needed
  void setProjectionMatrix(const Camera& cam);

private:

  void loadSkybox();
  void loadProjectionMatrix(const Camera& cam);
  void loadViewMatrix(const Camera& cam);

  unsigned int mSkyboxVAO = 0;
  unsigned int cubemapTexture = 0;
  std::unique_ptr<OGLShader> skyboxShader;
};
}  // end namespace AA
