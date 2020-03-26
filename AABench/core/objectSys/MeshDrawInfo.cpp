#include "MeshDrawInfo.h"

MeshDrawInfo::MeshDrawInfo(unsigned int a, std::vector<unsigned int> el, std::vector<TextureInfo> t, float shine, glm::vec4 spec)
{
  vao = a; elements = el; textures = t; shininess = shine; specular = spec;
}
