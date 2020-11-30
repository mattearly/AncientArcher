#pragma once
#include <vector>
#include <string>
#include <assimp/texture.h>
namespace AA
{
class TexLoader
{
public:
	static TexLoader* getInstance();
	unsigned int loadCubeTexture(const std::vector<std::string>& files);
	unsigned int textureFromFile(const char* filepath, bool gamma = false);
	unsigned int textureFromData(const aiTexture* tex);
private:
	TexLoader();
};
} // end namespace AA
