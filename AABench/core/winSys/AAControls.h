#pragma once
#include "AAInput.h"
class AAControls
{

public:

  static AAControls* getInstance();

  float getMouseSensitivity() const noexcept;
  void setMouseSensitivity(float sensitivity) noexcept;

  friend class AADisplay;  // for access to mouseMovement and mouseScrollWheelMovement
  friend class AAWorld;   // for access to Input

private:

  void pullButtonStateEvents();

  AAKeyBoardInput mButtonState;
  AAMouseInput    mMousePosition;
  AAScrollInput   mMouseWheelScroll;

  float mFirstPersonPerspectiveMouseSensitivity = 0.1f;  ///< mouse sensitivity while in first person perspective
  bool mRenewPerspective = true;

  void perspectiveMouseMovement(float xpos, float ypos) noexcept;
  void standardMouseMovement(float xpos, float ypos);
  void mouseScrollWheelMovement(float xpos, float ypos) noexcept;

  void resetControlVars() noexcept;

};
