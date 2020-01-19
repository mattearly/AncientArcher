#include "engineBaseCase.h"
#include "engineAllFuncBaseCase.h"
#include "enginePauseBaseCase.h"
#include "engineFuncsDemo_1.h"
#include "engineFuncsDemo_2.h"
#include <mearly/delay.h>
#include "assimpModel.h"

int runAllTests()
{
  testAssimpBaseLoading(test::model1);
  testAssimpBaseLoading(test::model2);
  testAssimpBaseLoading(test::model3);
  testAssimpBaseLoading(test::model4);
  testAssimpBaseLoading(test::model5);
  testAssimpBaseLoading(test::model6);
  testAssimpBaseLoading(test::model7);
  testAssimpBaseLoading(test::model8);
  testAssimpBaseLoading(test::model9);
  testAssimpBaseLoading(test::model10);
  testAssimpBaseLoading(test::model11);
  testAssimpBaseLoading(test::model12);
  testAssimpBaseLoading(test::model13);

  delay(120);
  testEngineBaseCase();
  delay(120);
  testEngineFuncsBaseCase();
  delay(120);
  testEnginePause();
  delay(120);
  testEngineFuncsDemo_1();
  delay(120);
  testEngineFuncsDemo_2();

  return 0;
}

int main()
{

  runAllTests();

  //holdAtCommandPrompt();

}