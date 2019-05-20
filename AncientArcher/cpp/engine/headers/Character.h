
/**
 * Simple data structure class holding information about a certain glyph in the
 * font texture atlas. All sizes are for a font-size of 1.
 *
 * @author Karl : (YT: ThinMatrix, https://www.youtube.com/watch?v=mnIQEQoHHCU&t=287s)
 * @modifier Matthew Early : Changed to C++ public struct
 *
 */
struct Character 
{
  int    _id;
  double _xTextureCoord;
  double _yTextureCoord;
  double _xMaxTextureCoord;
  double _yMaxTextureCoord;
  double _xOffset;
  double _yOffset;
  double _sizeX;
  double _sizeY;
  double _xAdvance;
  /**
   * @param id
   *            - the ASCII value of the character.
   * @param xTextureCoord
   *            - the x texture coordinate for the top left corner of the
   *            character in the texture atlas.
   * @param yTextureCoord
   *            - the y texture coordinate for the top left corner of the
   *            character in the texture atlas.
   * @param xTexSize
   *            - the width of the character in the texture atlas.
   * @param yTexSize
   *            - the height of the character in the texture atlas.
   * @param xOffset
   *            - the x distance from the curser to the left edge of the
   *            character's quad.
   * @param yOffset
   *            - the y distance from the curser to the top edge of the
   *            character's quad.
   * @param sizeX
   *            - the width of the character's quad in screen space.
   * @param sizeY
   *            - the height of the character's quad in screen space.
   * @param xAdvance
   *            - how far in pixels the cursor should advance after adding
   *            this character.
   */
  Character(int id, double xTextureCoord, double yTextureCoord, double xTexSize, double yTexSize,
    double xOffset, double yOffset, double sizeX, double sizeY, double xAdvance) {
    _id = id;
    _xTextureCoord = xTextureCoord;
    _yTextureCoord = yTextureCoord;
    _xOffset = xOffset;
    _yOffset = yOffset;
    _sizeX = sizeX;
    _sizeY = sizeY;
    _xMaxTextureCoord = xTexSize + xTextureCoord;
    _yMaxTextureCoord = yTexSize + yTextureCoord;
    _xAdvance = xAdvance;
  }

};