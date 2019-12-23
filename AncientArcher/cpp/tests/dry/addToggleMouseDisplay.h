#pragma once
#include "../../engine/AAEngine.h"


void addEscTogglesMouseDisplay(AAEngine& engine)
{
  auto escapeTogglesMouseDisplay = [](AAKeyBoardInput& keys)
  {
    static bool isFPS = true;
    if (keys.esc)
    {
      if (isFPS)
      {
        AADisplay::getInstance()->setCursorToVisible();
        isFPS = false;
        return true;
      }
      else
      {
        AADisplay::getInstance()->setCursorToDisabled();
        isFPS = true;
        return true;
      }
    }
    return false;
  };
  engine.addToTimedOutKeyHandling(escapeTogglesMouseDisplay);

}
