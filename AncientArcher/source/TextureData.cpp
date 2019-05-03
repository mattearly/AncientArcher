#include "TextureData.h"



TextureData::TextureData()
{
}


TextureData::TextureData(char* buf, int w, int h)
{
  buffer.sputn(buf, sizeof buf);
  width = w;
  height = h;
}

TextureData::~TextureData()
{
}

int TextureData::getWidth()
{
  return width;
}

int TextureData::getHeight()
{
  return height;
}

char* TextureData::getBuffer()
{
  char* buf;
  buffer.sgetn(buf, sizeof buffer);
  return buf;
}
