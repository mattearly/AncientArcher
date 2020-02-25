#include "dry/delay.h"
#include "assimpModel.h"
#include "engineLoopBase.h"
#include "engineDemoWorld.h"

int runAllTests()
{
  int result = 0;
  result = testEngineLoopBase();
  delay(120);
  result = testEngineDemoWorld();

  return result;
}

int main()
{
  return runAllTests();
}