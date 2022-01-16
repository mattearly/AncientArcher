#pragma once
#include "../Renderer/OpenGL/OGLShader.h"
#include "../Renderer/MeshInfo.h"
#include "Camera.h"
#include <vector>
#include <string>
#include <memory>
namespace AA {
class Skybox {
public:
  Skybox(std::vector<std::string> incomingSkymapFiles);
  void Render(const std::shared_ptr<Camera>& cam);
private:
  void SetViewMatrix(glm::mat4 view_mat);
  void SetProjectionMatrix(glm::mat4 proj_mat);
  unsigned int mCubemapTexId = 0;
  void setup_shader();
  void setup_cube_geometry();
  void setup_incoming_textures(std::vector<std::string>& incomingSkymapFiles);
};
}
