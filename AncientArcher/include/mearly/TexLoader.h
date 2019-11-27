#pragma once
#include <vector>
#include <string>
class TexLoader
{
public:
  static TexLoader* getInstance();
  unsigned int loadCubeTexture(const std::vector<std::string>& files);
  unsigned int load2DTexture(std::string path);
  unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false);
};