#include <TextureLoader.h>
#include <Shader.h>
#include <stb_image.h>
#include <glad/glad.h>
#include <sstream>
#include <iostream>

TextureLoader* TextureLoader::getTextureLoader()
{
  static TextureLoader* texLoader = new TextureLoader();

  return texLoader;
}

/**
 * This code loads in a cube map texture.
 * @param[in] files       to the textures
 * @return    textureID   id to reference the loaded texture
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
  stbi_set_flip_vertically_on_load(false); // tell stb_image.h to not flip loaded texture's on the y-axis.
  int width, height, nrChannel;
  std::size_t size = files.size();
  for (auto i = 0; i < size; ++i)
  {
    unsigned char* data = stbi_load(files[i].c_str(), &width, &height, &nrChannel, 0);
    if (data)
    {
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
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  return texID;
}


/**
 * This code loads in a 2D map texture.
 * @param[in] files to the textures
 * @return    textureID
 */
unsigned int TextureLoader::load2DTexture(std::string path) {

  // makes it so the use gets this message instead of loading more than MAXTEXTURES textures
  //if (numberOfLoadedTextures >= MAXTEXTURES) {
  //  std::cout << "Maximum number of textures have been loaded (" << numberOfLoadedTextures << ") ... Aborting texture load for '" << path << "'\n";
  //  return;
  //}
  unsigned int texID;
  //glGenTextures(1, &texture[num_textures]);
  glGenTextures(1, &texID);
  //texBankShader.use();

  //if (!initiated) {
    //glGenTextures(MAXTEXTURES, textureIDs);
  //}
  //glActiveTexture(GL_TEXTURE0 + numberOfLoadedTextures);

  glBindTexture(GL_TEXTURE_2D, texID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

  // UNCOMMENT BELOW FOR GL_REPEAT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)

  // UNCOMMENT BELOW FOR GL_MIRROR_REPEAT
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // UNCOMMENT BELOW FOR GL_CLAMP_TO_EDGE
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // UNCOMMENT BELOW FOR GL_CLAMP_TO_BORDER
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  // NEEDS TO HAVE A ONE MIN_FILTER AND ONE MAG_FILTER
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image, create texture and generate mipmaps
  stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
  int width, height, nrChannel;

  unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannel, 0);
  if (data) 
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture at path: " << path << std::endl;
    stbi_image_free(data);
  }
  stbi_image_free(data);

  return texID;
}