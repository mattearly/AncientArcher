#include "model.h"
#include "display.h"
#include "engineloop.h"
#include <glm/glm.hpp>
#include <iostream>

void holdAtCommandPrompt();

int main()
{
  // Test 1 - Pathing to and getting header info to a Model Using Assimp
  std::string model_sixColorSquare = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  std::string model_random1 = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\random1.obj";

  testAssimpBaseLoading(model_sixColorSquare);
  testAssimpBaseLoading(model_random1);
  testAssimpBaseLoading("This/is/a/test/");

  testEngineMainDisplay();

  holdAtCommandPrompt();

  testEngineLoop();

  holdAtCommandPrompt();

  return 0;
}


void holdAtCommandPrompt()
{
  // Software has chosen to exit hold on command prompt so we can view output
  std::cout << "Press Enter To Continue\n";
  char a;
  a = std::cin.get();
}