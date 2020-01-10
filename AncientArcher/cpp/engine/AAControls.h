#pragma once
#include "AAInput.h"
class AAControls
{

public:

  static AAControls* getInstance();

  float getMouseSensitivity() const;
  void setMouseSensitivity(float sensitivity);

  friend class AADisplay;  // for access to mouseMovement and mouseScrollWheelMovement
  friend class AAEngine;   // for access to Input 

private:

  void pullButtonStateEvents();

  AAKeyBoardInput mButtonState;
  AAMouseInput    mMousePosition;
  AAScrollInput   mMouseWheelScroll;

  float mMouseSensitivity = 0.1f;

  void perspectiveMouseMovement(float xpos, float ypos);
  void standardMouseMovement(float xpos, float ypos);
  void mouseScrollWheelMovement(float xpos, float ypos);

  void resetControlVars();
};
