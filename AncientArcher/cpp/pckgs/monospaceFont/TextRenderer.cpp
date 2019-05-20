#include "TextRenderer.h"
#include <GlyphLoader.h>
#include <AAEngine.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
TextRenderer::TextRenderer()
{
  textShader = std::make_unique<Shader>(
    "../AncientArcher/cpp/pckgs/monospaceFont/text.vert",
    "../AncientArcher/cpp/pckgs/monospaceFont/text.frag"
    );

  textShader->use();

  // not sure if this is necessary when only one texture
  textShader->setInt("textBitmap", 0);
  
  glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(display.window_width), 0.0f, static_cast<GLfloat>(display.window_width));
  textShader->setMat4("projection", projection);
}

void TextRenderer::init(std::string path, unsigned int width, unsigned int height, std::string chars, unsigned int glyphsPerLine)
{
  glyphMap = loadGlyphMap(path, width, height, chars, glyphsPerLine);

  /* set up an area to store vertex data */
  glGenVertexArrays(1, &textVAO);
  glGenBuffers(1, &textVBO);

  glBindBuffer(GL_ARRAY_BUFFER, textVBO);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

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
}

void TextRenderer::render(std::string text)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  textShader->use();

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, glyphMap._texID);

  for (auto t : text)
  {
    for (auto g : glyphMap._glyph)
    {
      if (g._char == t)
      {
        renderText(g);
      }
    }
  }

}

void TextRenderer::renderText(Glyph g)
{
  glBindVertexArray(textVAO);
  glEnableVertexAttribArray(0); 
  glEnableVertexAttribArray(1); 
  //textShader->setVec3("colour", glm::vec3(1.f, 0.f, 0.f));
  //textShader->setVec2("translation", glm::vec2((float)g._x, (float)g._y));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindVertexArray(0);
  
  
  textShader->stop();
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}
