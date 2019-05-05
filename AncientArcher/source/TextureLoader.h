#pragma once
#include "Shader.h"
#include <vector>
#include <string>
class TextureLoader
{
public:
  int loadCubeTexture(const std::vector<std::string> &files, Shader *skyShader);
};

