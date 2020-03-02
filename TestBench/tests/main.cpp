#include "../AAEngine.h"
#include "dry/delay.h"
#include "coreTests.h"

AAWorld mainWorld;

int runAllTests()
{
  int result = 0;
  result = testEngineLoopBase();  delay(120);
  result = testEngineDemoWorld();

  return result;
}

int main()
{
  return runAllTests();
}