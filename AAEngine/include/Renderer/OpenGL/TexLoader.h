#pragma once
#include <vector>
#include <string>
#include <assimp/texture.h>
namespace AA
{
class TexLoader
{
public:
	static unsigned int loadCubeTexture(const std::vector<std::string>& files);
	static unsigned int textureFromFile(const char* filepath, bool gamma = false);
	static unsigned int textureFromData(const aiTexture* tex);
private:
};
} // end namespace AA
