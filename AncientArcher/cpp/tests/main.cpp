#include "model.h"
#include "display.h"
#include "engineloop.h"
#include <glm/glm.hpp>
#include "enginepause.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "enginefunctions.h"

void delay(int time)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void holdAtCommandPrompt()
{
  // Software has chosen to exit hold on command prompt so we can view output
  std::cout << "Press Enter To Continue\n";
  char a;
  a = std::cin.get();
}

int main()
{
  // Test 1 - Pathing to and getting header info to a Model Using Assimp
  std::string model_sixColorSquare = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  std::string model_random1 = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\random1.obj";

  testAssimpBaseLoading(model_sixColorSquare);
  testAssimpBaseLoading(model_random1);
  testAssimpBaseLoading("This/is/a/test/");

  AADisplay::getInstance()->setWindowSize(800, 600, 1920/2 - 400, 1080/2 - 300);                // test changing window size (and position)

  testEngineMainDisplay();
  delay(100);
  testEngineLoop();
  delay(100);
  testEngineFunctionsPossibilities();
  delay(100);
  testEnginePause();

  return 0;
}

