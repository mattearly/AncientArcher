#include "DiffuseTexture.h"
#include <glad/glad.h>

#include"stb_image.h"
#include<iostream>
#include<sstream>

#include "../shaders/Shader.h"

extern Shader shader;

DiffuseTexture::DiffuseTexture() {
  num_loaded_diffuse = 0;
  active_diffuse = 0;
  initiated = false;
}

DiffuseTexture::~DiffuseTexture() {}

void DiffuseTexture::loadDiffuseTexture(std::string path) {
  // makes it so the use gets this message instead of loading more than MAXDIFFUSETEXTURES textures
  if (num_loaded_diffuse >= MAXDIFFUSETEXTURES) {
    std::cout << "Maximum number of diffuse textures have been loaded (" << num_loaded_diffuse << ") ... Aborting texture load for '" << path << "'\n";
    return;
  }

  if (!initiated) {
    glGenTextures(MAXDIFFUSETEXTURES, texture);
  }
  glActiveTexture(GL_TEXTURE16 + num_loaded_diffuse);

  glBindTexture(GL_TEXTURE_2D, texture[num_loaded_diffuse]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image, create texture and generate mipmaps
  //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannel, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load diffuse texture" << std::endl;
  }
  stbi_image_free(data);

  // attach the texture number to the end of the texture name 
  // (for passing to the right one in the shader)
  std::stringstream texture_shader_name("specular", std::ios_base::app | std::ios_base::out);
  texture_shader_name << num_loaded_diffuse;
  std::string n = texture_shader_name.str();
  shader.use();
  shader.setInt(n.c_str(), num_loaded_diffuse+16);

  // console notification of loaded texture
  if (num_loaded_diffuse == 0)
    std::cout << "   # | load path\n";
  std::cout << num_loaded_diffuse << "    | " << path << "\n";

  // increment to be ready for the next texture
  num_loaded_diffuse++;
}

void DiffuseTexture::activate(int n) {
  if (active_diffuse != n) {
    shader.setInt("specnum", n);
    active_diffuse = n;
  }
}
