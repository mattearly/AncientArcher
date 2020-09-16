#pragma once
#include <vector>
#include <string>
namespace AA
{
class TexLoader
{
public:
  static TexLoader* getInstance();
  unsigned int loadCubeTexture(const std::vector<std::string>& files);
  unsigned int textureFromFile(const char* filepath, bool gamma = false);
};
} // end namespace AA

