#pragma once
#include "AAInput.h"
class AAControls
{

public:

  static AAControls* getInstance();

  void pullButtonStateEvents();

  friend class AADisplay;  // for access to mouseMovement and mouseScrollWheelMovement
  friend class AAEngine;   // for access to Input 

private:

  AAKeyBoardInput mButtonState;
  AAMouseInput    mMousePosition;
  AAScrollInput   mMouseWheelScroll;

  float mMouseSensitivity = 0.1f;

  void mouseMovement(float xpos, float ypos);
  void mouseScrollWheelMovement(float xpos, float ypos);

};
