#include "dry/delay.h"
#include "engineBaseCase.h"
#include "enginePauseBaseCase.h"
#include "engineAllFuncBaseCase.h"
#include "engineFuncsDemo_1.h"
#include "engineFuncsDemo_2.h"
#include "engineFuncsDemo_3.h"
#include "engineFuncsDemo_4.h"
#include <testlib.h>
#include "assimpModel.h"
int runAllTests()
{
  //testAssimpBaseLoading(test::model1);
  //testAssimpBaseLoading(test::model2);
  //testAssimpBaseLoading(test::model3);
  //testAssimpBaseLoading(test::model4);
  //testAssimpBaseLoading(test::model5);
  //testAssimpBaseLoading(test::model6);
  //testAssimpBaseLoading(test::model7);
  //testAssimpBaseLoading(test::model8);
  //testAssimpBaseLoading(test::model9);
  //testAssimpBaseLoading(test::model10);
  //testAssimpBaseLoading(test::model11);
  //testAssimpBaseLoading(test::model12);
  //testAssimpBaseLoading(test::model13);

  std::cout << testlib::ret1() << '\n';

  delay(120);
  testEngineBaseCase();
  delay(120); 
  testEnginePause();
  delay(120);
  testEngineFuncsBaseCase();
  delay(120);
  testEngineFuncsDemo_1();
  delay(120);
  testEngineFuncsDemo_2();
  delay(120);
  testEngineFuncsDemo_3();
  delay(120);
  testEngineFuncsDemo_4();

  return 0;
}

int main()
{
  //runAllTests();

  testEngineFuncsDemo_4();
  
  holdAtCommandPrompt();
}