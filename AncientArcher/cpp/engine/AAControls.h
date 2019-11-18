#pragma once
#include "AAKeyInput.h"
class AAControls
{
public:
  static AAControls* getInstance();
  void mouseMovement(float xpos, float ypos);
  void mouseScrollWheelMovement(float xpos, float ypos);
  void keyInput(AAKeyInput* keyInput);
private:
};
