#include <sstream>
#pragma once
class TextureData
{
public:
  TextureData(char* buf, int w, int h);
  ~TextureData();
  
  int getWidth();
  int getHeight();

  template <class Type>
  std::stringbuf& get(std::stringbuf& buf, Type& var)
  {
    buf.sgetn(reinterpret_cast<char*>(&var), sizeof(var));

    return buf;
  }

private:
  int width;
  int height;


  std::stringbuf buffer;
};

