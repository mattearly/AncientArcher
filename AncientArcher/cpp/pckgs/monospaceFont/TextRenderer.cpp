#include "TextRenderer.h"
#include <GlyphLoader.h>
#include <glad/glad.h>
TextRenderer::TextRenderer()
{
  textShader = std::make_unique<Shader>(
    "../AncientArcher/cpp/pckgs/monospaceFont/text.vert",
    "../AncientArcher/cpp/pckgs/monospaceFont/text.frag"
    );

  // not sure if this is necessary when only one texture
  textShader->use();
  textShader->setInt("texture0", 0);
}

void TextRenderer::init(std::string path, unsigned int width, unsigned int height, std::string chars, unsigned int glyphsPerLine)
{
  //load up a front facing rect
  float vertices[] =
  {  // verticies            //texture coords
    -0.5f, -0.5f,      0.0f, 0.0f,
     0.5f, -0.5f,      1.0f, 0.0f,
     0.5f,  0.5f,      1.0f, 1.0f,
     0.5f,  0.5f,      1.0f, 1.0f,
    -0.5f,  0.5f,      0.0f, 1.0f,
    -0.5f, -0.5f,      0.0f, 0.0f
  };


  /* set up an area to store vertex data */
  glGenVertexArrays(1, &textVAO);
  glGenBuffers(1, &textVBO);

  glBindBuffer(GL_ARRAY_BUFFER, textVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

  glBindVertexArray(textVAO);

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

  // texture coord attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  // unbind buffers
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glyphMap = loadGlyphMap(path, width, height, chars, glyphsPerLine);

}

void TextRenderer::renderAt(unsigned int x, unsigned int y, std::string text)
{
  textShader->use();

  glBindVertexArray(textVAO);
  glEnableVertexAttribArray(0);

  //set position of text and render per char
  for (auto t : text)
  {
    for (auto g : glyphMap._glyph)
    {
      if (g._char == t)
      {
        //textShader->setVec2("position", g._x, g._y);
        glDrawArrays(GL_TRIANGLES, 0, 6);
      }
    }
  }
  glBindVertexArray(0);
}
