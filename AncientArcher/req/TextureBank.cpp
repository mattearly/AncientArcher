#include "TextureBank.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<stb_image.h>
#include<iostream>
#include<sstream>

TextureBank::TextureBank() {
  num_textures = 0;
  //active_tex = 0;
  initiated = false;
}

TextureBank::~TextureBank() {}

void TextureBank::loadTexture(std::string path, Shader *shader, bool alpha) {

  if (num_textures >= MAXTEXTURES) {
    std::cout << "Maximum number of textures have been loaded (" << num_textures << ") ... Aborting texture load for '" << path << "'\n";
    return;
  }

  //glGenTextures(1, &texture[num_textures]);

  if (!initiated) {
    glGenTextures(MAXTEXTURES, texture);
  }

  switch (num_textures) {
  case 0:
    glActiveTexture(GL_TEXTURE0);
    break;
  case 1:
    glActiveTexture(GL_TEXTURE1);
    break;
  case 2:
    glActiveTexture(GL_TEXTURE2);
    break;
  case 3:
    glActiveTexture(GL_TEXTURE3);
    break;
  case 4:
    glActiveTexture(GL_TEXTURE4);
    break;
  case 5:
    glActiveTexture(GL_TEXTURE5);
    break;
  case 6:
    glActiveTexture(GL_TEXTURE6);
    break;
  case 7:
    glActiveTexture(GL_TEXTURE7);
    break;
  case 8:
    glActiveTexture(GL_TEXTURE8);
    break;
  case 9:
    glActiveTexture(GL_TEXTURE9);
    break;
  case 10:
    glActiveTexture(GL_TEXTURE10);
    break;
  case 11:
    glActiveTexture(GL_TEXTURE11);
    break;
  case 12:
    glActiveTexture(GL_TEXTURE12);
    break;
  case 13:
    glActiveTexture(GL_TEXTURE13);
    break;
  case 14:
    glActiveTexture(GL_TEXTURE14);
    break;
  case 15:
    glActiveTexture(GL_TEXTURE15);
    break;
  case 16:
    glActiveTexture(GL_TEXTURE16);
    break;
  case 17:
    glActiveTexture(GL_TEXTURE17);
    break;
  case 18:
    glActiveTexture(GL_TEXTURE18);
    break;
  case 19:
    glActiveTexture(GL_TEXTURE19);
    break;
  case 20:
    glActiveTexture(GL_TEXTURE20);
    break;
  case 21:
    glActiveTexture(GL_TEXTURE21);
    break;
  case 22:
    glActiveTexture(GL_TEXTURE22);
    break;
  case 23:
    glActiveTexture(GL_TEXTURE23);
    break;
  case 24:
    glActiveTexture(GL_TEXTURE24);
    break;
  case 25:
    glActiveTexture(GL_TEXTURE25);
    break;
  case 26:
    glActiveTexture(GL_TEXTURE26);
    break;
  case 27:
    glActiveTexture(GL_TEXTURE27);
    break;
  case 28:
    glActiveTexture(GL_TEXTURE28);
    break;
  case 29:
    glActiveTexture(GL_TEXTURE29);
    break;
  case 30:
    glActiveTexture(GL_TEXTURE30);
    break;
  case 31:
    glActiveTexture(GL_TEXTURE31);
    break;
  default:;
  }
  //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture - doesn't seem needed
  glBindTexture(GL_TEXTURE_2D, texture[num_textures]); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

 // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)

 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
 
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
 // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
 // float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
 // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  // set texture filtering parameters
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load image, create texture and generate mipmaps
  //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
  
  unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (data) {
    if (alpha) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  std::stringstream texture_shader_name("texture", std::ios_base::app | std::ios_base::out);
  texture_shader_name << num_textures;

  std::string n = texture_shader_name.str();

  shader->use();

  shader->setInt(n.c_str(), num_textures);
  /*
  switch (num_textures) {
  case 0:
    glActiveTexture(GL_TEXTURE0);
    break;
  case 1:
    glActiveTexture(GL_TEXTURE1);
    break;
  case 2:
    glActiveTexture(GL_TEXTURE2);
    break;
  case 3:
    glActiveTexture(GL_TEXTURE3);
    break;
  case 4:
    glActiveTexture(GL_TEXTURE4);
    break;
  case 5:
    glActiveTexture(GL_TEXTURE5);
    break;
  case 6:
    glActiveTexture(GL_TEXTURE6);
    break;
  case 7:
    glActiveTexture(GL_TEXTURE7);
    break;
  case 8:
    glActiveTexture(GL_TEXTURE8);
    break;
  case 9:
    glActiveTexture(GL_TEXTURE9);
    break;
  case 10:
    glActiveTexture(GL_TEXTURE10);
    break;
  case 11:
    glActiveTexture(GL_TEXTURE11);
    break;
  case 12:
    glActiveTexture(GL_TEXTURE12);
    break;
  case 13:
    glActiveTexture(GL_TEXTURE13);
    break;
  case 14:
    glActiveTexture(GL_TEXTURE14);
    break;
  case 15:
    glActiveTexture(GL_TEXTURE15);
    break;
  case 16:
    glActiveTexture(GL_TEXTURE16);
    break;
  case 17:
    glActiveTexture(GL_TEXTURE17);
    break;
  case 18:
    glActiveTexture(GL_TEXTURE18);
    break;
  case 19:
    glActiveTexture(GL_TEXTURE19);
    break;
  case 20:
    glActiveTexture(GL_TEXTURE20);
    break;
  case 21:
    glActiveTexture(GL_TEXTURE21);
    break;
  case 22:
    glActiveTexture(GL_TEXTURE22);
    break;
  case 23:
    glActiveTexture(GL_TEXTURE23);
    break;
  case 24:
    glActiveTexture(GL_TEXTURE24);
    break;
  case 25:
    glActiveTexture(GL_TEXTURE25);
    break;
  case 26:
    glActiveTexture(GL_TEXTURE26);
    break;
  case 27:
    glActiveTexture(GL_TEXTURE27);
    break;
  case 28:
    glActiveTexture(GL_TEXTURE28);
    break;
  case 29:
    glActiveTexture(GL_TEXTURE29);
    break;
  case 30:
    glActiveTexture(GL_TEXTURE30);
    break;
  case 31:
    glActiveTexture(GL_TEXTURE31);
    break;
  default:;
  }
  glBindTexture(GL_TEXTURE_2D, texture[num_textures]);
  */
  std::cout <<
    num_textures << ". " << path << "\n";


  num_textures++;

}

void TextureBank::activate(Shader * shader, int n) {
  if (active_tex != n) {
    shader->setInt("texnum", n);
    active_tex = n;
  }
}
