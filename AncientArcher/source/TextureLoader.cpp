#include "TextureLoader.h"
#include "stb_image.h"
#include "Shader.h"

#include <glad/glad.h>

extern std::vector<int> skyboxTextures;
extern Shader skyboxShader;

/*
 * Loads a set of cube textures.
 * @return int the ID of the cube texture.
 */
void loadCubeTexture(const std::vector<std::string>& files)
{
  GLuint textureID;
  glGenTextures(1, &textureID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannel;
  //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
  int itr = 0;
  for (auto f : files)
  {
    unsigned char* data = stbi_load(f.c_str(), &width, &height, &nrChannel, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + itr, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      //glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    itr++;
  }
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  skyboxTextures.push_back(textureID);

  skyboxShader.use();
  skyboxShader.setInt("cubeMap", textureID);
}
