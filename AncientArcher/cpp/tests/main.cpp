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
  std::string model = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  std::string model2 = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\robot_thing.obj";
  std::string model3 = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\HeatSink.obj";
  std::string model4 = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\metaball.obj";
  std::string model5 = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\susan_not_triangulated.obj";
  std::string model6 = "C:/Users/matt/Dropbox_me298414/Dropbox/My3DModels/cone_textured_not_triangulated.obj";
  std::string model7 = "C:/Users/matt/Dropbox_me298414/Dropbox/My3DModels/water_plane_not_triangulated.obj";

  testAssimpBaseLoading(model);
  testAssimpBaseLoading(model2);
  testAssimpBaseLoading(model3);
  testAssimpBaseLoading(model4);
  testAssimpBaseLoading(model5);
  testAssimpBaseLoading(model6);
  testAssimpBaseLoading(model7);
  testAssimpBaseLoading("This/is/a/test/");

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

