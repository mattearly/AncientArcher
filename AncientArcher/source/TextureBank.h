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
  TextureBank();
  void loadTexture(std::string path);

  /** Signals the renderer to use this texture ID. Update this before doing draw or render calls.
   * @param[in] n   sets currentActiveTexture to n.
   */
  void activate(int n);

private:

  static constexpr int MAXTEXTURES = 32;  // 0 - 31

  int currentActiveTexture;

  int numberOfLoadedTextures;

  unsigned int texture[MAXTEXTURES];

  int width, height, nrChannel;

  bool initiated;

};

