#include "TextureBank.h"
#include <glad/glad.h>

#include"stb_image.h"
#include<iostream>
#include<sstream>

#include "globals.h"

TextureBank::TextureBank() {
  numberOfLoadedTextures = 0;
  currentActiveTexture = 0;
  initiated = false;
}


/** Adds a new texture to the default shader: shader_vertex.glsl and iterates numberOfLoadedTextures.
 * Maxes out at 32.
 * @param[in] path  Path to the the texture.
 */
void TextureBank::loadTexture(std::string path) {

  // makes it so the use gets this message instead of loading more than MAXTEXTURES textures
  if (numberOfLoadedTextures >= MAXTEXTURES) {
    std::cout << "Maximum number of textures have been loaded (" << numberOfLoadedTextures << ") ... Aborting texture load for '" << path << "'\n";
    return;
  }

  //glGenTextures(1, &texture[num_textures]);

  if (!initiated) {
    glGenTextures(MAXTEXTURES, texture);
  }
  glActiveTexture(GL_TEXTURE0 + numberOfLoadedTextures);

  glBindTexture(GL_TEXTURE_2D, texture[numberOfLoadedTextures]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

  // UNCOMMENT BELOW FOR GL_REPEAT
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)

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

  
  //
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image, create texture and generate mipmaps
  stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannel, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // attach the texture number to the end of the texture name (for passing to the right one in the shader)
  std::stringstream texture_shader_name("texture", std::ios_base::app | std::ios_base::out);
  texture_shader_name << numberOfLoadedTextures;
  std::string n = texture_shader_name.str();
  shader.use();
  shader.setInt(n.c_str(), numberOfLoadedTextures);

  // console notification of loaded texture
  if (numberOfLoadedTextures == 0)
    std::cout << "tex# | load path\n";
  std::cout << numberOfLoadedTextures << "    | " << path << "\n";

  // increment to be ready for the next texture
  numberOfLoadedTextures++;

}

void TextureBank::activate(int n) {
  if (currentActiveTexture != n) {
    shader.setInt("texnum", n);
    currentActiveTexture = n;
  }
}
