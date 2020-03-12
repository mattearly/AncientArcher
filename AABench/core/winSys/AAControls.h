#pragma once
#include "AAInput.h"
enum class STANDARDMOUSEZEROS { DEFAULT, TOP_LEFT_0_to_1, BOT_LEFT_0_to_1, TOP_LEFT_FULL_RANGE, BOT_LEFT_FULL_RANGE };
class AAControls
{
public:

  static AAControls* getInstance();

  float getFPPMouseSensitivity() const noexcept;
  void setFPPMouseSensitivity(float sensitivity) noexcept;

  friend class AADisplay;  // for access to mouseMovement and mouseScrollWheelMovement
  friend class AAWorld;   // for access to Input

private:

  STANDARDMOUSEZEROS mStandardMouseZeros = STANDARDMOUSEZEROS::BOT_LEFT_0_to_1;

  void pullButtonStateEvents();

  AAKeyBoardInput mButtonState;
  AAMouseInput    mMousePosition;
  AAScrollInput   mMouseWheelScroll;

  float mFPPMouseSensitivity = 0.1f;  ///< mouse sensitivity while in first person perspective
  bool mRenewFPP = true;

  void perspectiveMouseMovement(float xpos, float ypos) noexcept;
  void standardMouseMovement(float xpos, float ypos);
  void mouseScrollWheelMovement(float xpos, float ypos) noexcept;

  void resetControlVars() noexcept;

};
