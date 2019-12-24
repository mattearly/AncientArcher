#pragma once
#include "../../engine/AAEngine.h"

void addPreferredDefaults(AAEngine& engine)
{
  AAViewport::getInstance()->setRenderDistance(3000.f);
  AADisplay::getInstance()->setWindowSize(1600, 900, 140, 90);
}
