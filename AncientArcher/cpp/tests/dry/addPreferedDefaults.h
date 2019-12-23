#pragma once
#include "../../engine/AAEngine.h"


void addPreferredDefaults(AAEngine& engine)
{
  auto preferredDefaults = []() {
    AAViewport::getInstance()->setRenderDistance(3000.f);
    AADisplay::getInstance()->setCursorToDisabled();
    AADisplay::getInstance()->setWindowSize(800, 600, 1000, 200);
  };
  engine.addToOnBegin(preferredDefaults);
}