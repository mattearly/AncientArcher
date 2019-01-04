#include "TextureBank.h"
#include <glad/glad.h>

#include"stb_image.h"
#include<iostream>
#include<sstream>

#include "../shaders/Shader.h"

extern Shader shader;

TextureBank::TextureBank() {

  num_loaded_textures = 0;
  active_tex = 0;
  initiated = false;

}

TextureBank::~TextureBank() {}

void TextureBank::loadTexture(std::string path) {

  // makes it so the use gets this message instead of loading more than MAXTEXTURES textures
  if (num_loaded_textures >= MAXTEXTURES) {
    std::cout << "Maximum number of textures have been loaded (" << num_loaded_textures << ") ... Aborting texture load for '" << path << "'\n";
    return;
  }

  //glGenTextures(1, &texture[num_textures]);

  if (!initiated) {
    glGenTextures(MAXTEXTURES, texture);
  }
  glActiveTexture(GL_TEXTURE0 + num_loaded_textures);

  glBindTexture(GL_TEXTURE_2D, texture[num_loaded_textures]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

 // set the texture wrapping parameters
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  // set texture filtering parameters
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
  texture_shader_name << num_loaded_textures;
  std::string n = texture_shader_name.str();
  shader.use();
  shader.setInt(n.c_str(), num_loaded_textures);

  // console notification of loaded texture
  if (num_loaded_textures == 0)
    std::cout << "tex# | load path\n";
  std::cout << num_loaded_textures << "    | " << path << "\n";

  // increment to be ready for the next texture
  num_loaded_textures++;

}

void TextureBank::activate(int n) {
  if (active_tex != n) {
    shader.setInt("texnum", n);
    active_tex = n;
  }
}
