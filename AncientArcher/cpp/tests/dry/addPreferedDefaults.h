#pragma once
#include "../../engine/AAEngine.h"

void addPreferredDefaults(AAEngine& engine)
{
  AAViewport::getInstance()->setRenderDistance(3000.f);
  AADisplay::getInstance()->setCursorToDisabled();
  AADisplay::getInstance()->setWindowSize(800, 600, 1000, 200);
}
