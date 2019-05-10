#pragma once
#include<string>

/**  
 * Class to load up textures into the texture bank in the default shader: shader_vertex.glsl.
 *   1.  must be a .png
 *   2.  must have alpha channel
 *   3.  must have width divisable by 4
 */

class TextureBank {
public:

  //static constexpr int MAXTEXTURES = 32;

  void loadTexture(std::string path);

  //unsigned int textureIDs[MAXTEXTURES];

  //void activateTexture(int n);

private:
  
  //int currentActiveTexture = 0;
  
  //int numberOfLoadedTextures = 0;
  
  //bool initiated = false;

};
