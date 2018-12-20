#pragma once
#include <vector>
class PrimativeManager {
public:
  PrimativeManager();
  ~PrimativeManager();
  
  void loadShape();

  void drawShape();

private:

  std::vector<unsigned int> vaos;

  static int VAOcount;

};

