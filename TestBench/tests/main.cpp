#include "dry/delay.h"
#include <testlib.h>
#include "assimpModel.h"
#include "engineLoopBase.h"
#include "engineDemoWorld.h"

int runAllTests()
{
  testAssimpBaseLoading("../assets/6ColorSquare.obj");

  std::cout << testlib::ret1() << '\n';

  delay(120);
  testEngineLoopBase();
  delay(120);
  testEngineDemoWorld();

  return 0;
}

int main()
{
  runAllTests();

  holdAtCommandPrompt();
}