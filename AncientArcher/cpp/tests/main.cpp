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

  std::cout << "local working dir: " << getexepath() << '\n';
  // Test 1 - Pathing to and getting header info to a Model Using Assimp
  //std::string model_sixColorSquare = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  //std::string model_random1 = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\random1.obj";

  //testAssimpBaseLoading(model_sixColorSquare);
  //testAssimpBaseLoading(model_random1);
  //testAssimpBaseLoading("This/is/a/test/");

  //AADisplay::getInstance()->setWindowSize(800, 600, 1920 / 2 - 400, 1080 / 2 - 300);                // test changing window size (and position)

  //testLaunchingDisplay();
  //delay(120);
  //testRunBaseEngineLoop();
  //delay(120);
  testStressEngineFunctions();
  //delay(120);
  //testEnginePause();
  //delay(120);

  return 0;
}

