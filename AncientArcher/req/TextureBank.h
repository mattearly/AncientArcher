#pragma once
#include<string>
#include<Shader.h>
class TextureBank {
public:
  TextureBank();
  ~TextureBank();
  
  static constexpr unsigned int MAXTEXTURES = 16;  // 0 - 15

  unsigned int texture[MAXTEXTURES];

  /*static*/ int num_textures;

  /* loadTexture adds a new texture to the bank of usable textures */
  void loadTexture(std::string path, Shader *shader, bool alpha = false);

  int width, height, nrChannels;


};

