#pragma once
#include "AAInput.h"
#include <memory>
class AAControls
{
public:
  static AAControls* getInstance();
  
  void setMouse(std::shared_ptr<AAMouseInput>& mouse);
  void setScroll(std::shared_ptr<AAScrollInput>& scroll);  
  
  void keyInput(std::shared_ptr<AAKeyInput>& keyInput);

  friend class AADisplay;  // for access to mouseMovement and mouseScrollWheelMovement

private:
  float mMouseSensitivity = 0.1f;

  std::shared_ptr<AAMouseInput> mMouse;
  std::shared_ptr<AAScrollInput> mScroll;

  void mouseMovement(float xpos, float ypos);
  void mouseScrollWheelMovement(float xpos, float ypos);

};
