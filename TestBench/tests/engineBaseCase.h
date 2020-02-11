#pragma once
#include "../world/AAWorld.h"

void testEngineBaseCase()
{
  //DISPLAY->setWindowTitle("AA Base Test - Close Window To Continue");
  AAWorld world;
  world.run();
}