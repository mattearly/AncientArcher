#pragma once
#include <vector>

/**
 * Monospaced glyphs.
 */

 /**
  * Holds a single Glyph Character
  */
struct Glyph {
  ///< ASCII char that maps with the glyph.
  char _char;

  ///< X Location of the char in the bitmap.
  unsigned int _x;

  ///< Y Location of the char in the bitmap.
  unsigned int _y;
};

/**
 * GlyphMap holds informatino about a glyph bitmap.
 */
struct GlyphMap {
  ///< Texture ID of the bitmap.
  unsigned int _texID = 0;

  ///< An array list of the glyphs in the map.
  std::vector<Glyph> _glyph;

  ///< Width of each glyph.
  unsigned int _w = 0;

  ///< Height of each glyph.
  unsigned int _h = 0;
};