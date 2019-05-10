#include "Game.h"
#include "Display.h"
#include "TextureBank.h"
#include "Lighting.h"
#include "Shader.h"

// Globals Variables
//  To use a global:
// #include "Globals.h"

Display display;
TextureBank texBank;
Shader texBankShader(
  "../AncientArcher/resource/shaderTextureBank32Vert.glsl",
  "../AncientArcher/resource/shaderTextureBank32Frag.glsl"
);
Shader skyboxShader(
  "../AncientArcher/resource/shaderSkyboxVert.glsl", 
  "../AncientArcher/resource/shaderSkyboxFrag.glsl"
);
Lighting lighting;
std::vector<Entity> entities;

int main() 
{
  Game game;
  game.mainLoop();
	return 0;
}
