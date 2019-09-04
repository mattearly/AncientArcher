#pragma once
#include <vector>
#include <string>
class TextureLoader
{
public:
  static TextureLoader* getTextureLoader();
  unsigned int loadCubeTexture(const std::vector<std::string>& files);
  unsigned int load2DTexture(std::string path);
};
