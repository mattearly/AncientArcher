#include "Game.h"
#include "Display.h"
#include "TextureBank.h"
#include "Lighting.h"
#include "Shader.h"

// globals, #include "globals.h" to use
Display display;
Shader texBankShader(
  "../AncientArcher/resource/shaderTextureBank32Vert.glsl",
  "../AncientArcher/resource/shaderTextureBank32Frag.glsl"
);
TextureBank texBank;
Lighting lighting;
std::vector<Entity> entities;

int main() 
{
  Game game;
  game.mainLoop();
	return 0;
}

