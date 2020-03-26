#pragma once
#include <string>
#include <vector>
#include <glm\ext\vector_float4.hpp>

struct TextureInfo
{
  unsigned int id = 0;
  std::string type;
  std::string path;
};

struct MeshDrawInfo
{
  MeshDrawInfo(unsigned int a, std::vector<unsigned int> el, std::vector<TextureInfo> t, float shine, glm::vec4 spec);
  unsigned int vao = 0;
  //unsigned int vbo = 0;
  //unsigned int ebo = 0;
  std::vector<TextureInfo> textures;
  float shininess;
  glm::vec4 specular;
  std::vector<unsigned int> elements;
};
