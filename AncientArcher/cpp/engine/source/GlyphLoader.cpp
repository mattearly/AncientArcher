#include <GlyphLoader.h>
#include <TextureLoader.h>

/**
 * Loads up and returns a GlyphMap with texture and Glyphs set up. Each glyph must be the same size height and width.
 * If some are different size, this will not work as expected.
 *
 * @param[in] path           Path to the texture file that contains the glyphs.
 * @param[in] width          Width of each glyph in the texture file.
 * @param[in] height         Height of each glyph in the texture file.
 * @param[in] charMap        A string list of all the characters in the glyph to ID each glyph as.
 * @param[in] glyphsPerLine  The number of glyphs on each line of the texture.
 *
 * @return GlyphMap          A populated GlyphMap.
 */
GlyphMap loadGlyphMap(std::string path, unsigned int width, unsigned int height, std::string charMap, unsigned int glyphsPerLine)
{
  GlyphMap glyphMapToReturn;
  TextureLoader texLoader;
  glyphMapToReturn._texID = texLoader.load2DTexture(path);
  glyphMapToReturn._w = width;
  glyphMapToReturn._h = height;

  Glyph newGlyph;
  unsigned int xLoc = 0;
  unsigned int yLoc = 0;
  unsigned int lineCount = 0;
  for (auto c : charMap)
  {
    // set up the variables of this glyph
    newGlyph._char = c;
    newGlyph._x = xLoc;
    newGlyph._y = yLoc;
    // put it on the array
    glyphMapToReturn._glyph.push_back(newGlyph);

    // prep for next
    lineCount++;
    if (lineCount > glyphsPerLine)
    {
      // set top left and top right current (ready for next line)
      xLoc = 0;
      yLoc += height;
      // reset glyph per line count as we are going to the next line in the bitmap
      glyphsPerLine = 0;
    }
    else
    {
      // advance
      xLoc += width;
    }
  }

  return glyphMapToReturn;   // copy
}
