#pragma once
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "../Renderer/OpenGL/OGLShader.h"
namespace AA {
class Skybox {
public:
  Skybox(std::vector<std::string> incomingSkymapFiles);
  void Render();
  void SetProjectionMatrix(glm::mat4 proj_mat);
  void SetViewMatrix(glm::mat4 view_mat);
private:
  unsigned int mSkyboxVAO;
  unsigned int mCubemapTexId = 0;
  std::unique_ptr<OGLShader> mSkyboxShader;
};
}