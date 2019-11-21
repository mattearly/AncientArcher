#include "model.h"
#include "display.h"
#include "engineloop.h"
#include <glm/glm.hpp>
#include "enginepause.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "enginefunctions.h"

void delay(int time);
void holdAtCommandPrompt();

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

  delay(1);

  testEnginePause();

  delay(1);

  testEngineLoop();

  delay(1);

  testEnginePause();

  delay(1);

  testEngineFunctionsPossibilities();

  return 0;
}

void delay(int time)
{
  std::this_thread::sleep_for(std::chrono::seconds(time));
}

void holdAtCommandPrompt()
{
  // Software has chosen to exit hold on command prompt so we can view output
  std::cout << "Press Enter To Continue\n";
  char a;
  a = std::cin.get();
}