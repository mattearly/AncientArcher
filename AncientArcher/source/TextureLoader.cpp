#include "TextureLoader.h"
#include "stb_image.h"
#include "Shader.h"

#include <glad/glad.h>

#include <iostream>

/*
 * Loads a set of cube textures.
 * @param[in] cubeMapFiles   Set of cube files to load in.
 * @param[in] cubeMapShader  Shader that holds the cubeMap texture. Cube texture is set to this shader after this function has ran successfully.
 * @return                   Texture ID
 */
int TextureLoader::loadCubeTexture(const std::vector<std::string>& cubeMapFiles, Shader* cubeMapShader)
{
  GLuint textureID;
  cubeMapShader->use();
  glGenTextures(1, &textureID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannel;
  unsigned char* data;
  //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

  for (auto f : cubeMapFiles)
  {
    static int i = 0;
    std::string path = "../AncientArcher/resource/" + f + ".png";
    data = stbi_load(path.c_str(), &width, &height, &nrChannel, 0);
    if (data)
    {
      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0, GL_RGBA, width, height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, data
      );
      stbi_image_free(data);
    }
    else
    {
      std::cout << "texture not loaded\n";
      stbi_image_free(data);

    }
      // not sure if this is needed
      //glGenerateMipmap(GL_TEXTURE_2D);

    i++;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  //cubeMapShader->use();
  cubeMapShader->setInt("cubeMap", 0);
  //cubeMapShader->stop();

  std::cout << " cube map texture ID is " << textureID << std::endl;
  return textureID;

}
