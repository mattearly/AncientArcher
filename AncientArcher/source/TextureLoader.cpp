#include "TextureLoader.h"
#include "stb_image.h"
#include "Shader.h"
#include <glad/glad.h>

#include <iostream>

/**
 * This code loads in a cube map texture.
 * @param[in] files to the textures
 * @return textureID
 */
unsigned int TextureLoader::loadCubeTexture(const std::vector<std::string>& files)
{
  if (files.size() != 6)
  {
    throw std::runtime_error("Not enough files for cube map. A cube map needs to include 6 textures.");
  }

  unsigned int texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

  int width, height, nrChannel;
  for (auto f : files)
  {
    static unsigned int i = 0;
    std::string path = "../AncientArcher/resource/" + f;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannel, 0);
    if (data)
    {
      std::cout << "cube tex: " << i << std::endl;
      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0,
        GL_RGB,
        width,
        height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data
      );
      stbi_image_free(data);
    }
    else
    {
      stbi_image_free(data);
      throw std::runtime_error("A cubemap texture was not able to be loaded.");
    }
    i++;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  std::cout << " cube map texture ID is " << texID << std::endl;
  return texID;
}
