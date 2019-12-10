#include "model.h"
#include "engineloop.h"
#include <glm/glm.hpp>
#include "enginepause.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "enginefunctions.h"
#include "enginedisplay.h"
#include "workingdir.h"
#include "modelPaths.h"
#include "engineBaseCase.h"

void delay(int time)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void holdAtCommandPrompt()
{
  std::cout << "Press Enter To Continue\n";
  char a;
  a = std::cin.get();
}

int main()
{
  //std::cout << "local working dir: " << getexepath() << '\n';

  //testAssimpBaseLoading(test::model);
  //testAssimpBaseLoading(test::model2);
  //testAssimpBaseLoading(test::model3);
  //testAssimpBaseLoading(test::model4);
  //testAssimpBaseLoading(test::model5);
  //testAssimpBaseLoading(test::model6);
  //testAssimpBaseLoading(test::model7);
  //testAssimpBaseLoading("This/is/a/test/");

  //AADisplay::getInstance()->setWindowSize(800, 600, 1920 / 2 - 400, 1080 / 2 - 300);                // test changing window size (and position)

  //testLaunchingDisplay();
  //delay(120);
  //testRunBaseEngineLoop();
  //delay(120);


  testEngineBaseCase();
  //delay(120);
  testStressEngineFunctions();


  //delay(120);
  //testEnginePause();
  //delay(120);


  //holdAtCommandPrompt();
  return 0;

}

