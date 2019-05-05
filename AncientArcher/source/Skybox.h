#pragma once
#include "Shader.h"
#include <glm/glm.hpp>
class Skybox
{
public:
  void loadProjectionMatrix(Shader *skyboxShader);
  void loadViewMatrix(Shader *skyboxShader);

private:

};
