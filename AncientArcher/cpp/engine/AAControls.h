#pragma once
#include "AAKeyInput.h"
class AAControls
{
public:
  static AAControls* getInstance();
  void mouseMovement(int xpos, int ypos);
  void mouseScrollWheelMovement(int xpos, int ypos);
  void keyInput(AAKeyInput* keyInput);
private:
};
