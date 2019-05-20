#pragma once
#include <Glyph.h>
#include <Shader.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
class TextRenderer
{
public:
  TextRenderer();
  void init(std::string path, unsigned int width, unsigned int height, std::string chars, unsigned int glyphsPerLine);
  void render(std::string text);
private:
  void renderText(Glyph g);
  GlyphMap glyphMap;
  std::unique_ptr< Shader > textShader;
  unsigned int textVAO = 0;
  unsigned int textVBO = 0;
};