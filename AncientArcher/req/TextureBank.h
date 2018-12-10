#pragma once
#include<string>
#include<Shader.h>
class TextureBank {
public:
  TextureBank();
  ~TextureBank();
  
  /* loadTexture adds a new texture to the bank of usable textures */
  void loadTexture(std::string path, Shader *shader, bool alpha = false);

  /* signal to use a certain texture */
  void activate(Shader *shader, int n);

private:

  int active_tex;

  int num_textures;

  static constexpr unsigned int MAXTEXTURES = 16;  // 0 - 15

  unsigned int texture[MAXTEXTURES]; // 16 array

  int width, height, nrChannel;  

  bool initiated;

};

