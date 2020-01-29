#pragma once
#include "../engine/AAEngine.h"

void testEngineBaseCase()
{
  AAEngine engineInstance;
  DISPLAY->setWindowTitle("AA Base Test - Close Window To Continue");
  engineInstance.run();
}