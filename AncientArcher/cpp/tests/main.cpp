#include "model.h"
#include "display.h"
#include <glm/glm.hpp>
#include <iostream>

void hold();

int main()
{
  // Test 1 - Pathing to and getting header info to a Model Using Assimp
  std::string model_sixColorSquare = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  std::string model_random1 = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\random1.obj";
  testAssimpBaseLoading(model_sixColorSquare);
  testAssimpBaseLoading(model_random1);
  testAssimpBaseLoading("\\asdf\\asdf\\asdf:\\asdf");
  testEngineMainDisplay();





  hold();
  return 0;
}


void hold()
{
  // Software has chosen to exit hold on command prompt so we can view output
  std::cout << "Press Enter To Exit\n";
  char a;
  a = std::cin.get();
}