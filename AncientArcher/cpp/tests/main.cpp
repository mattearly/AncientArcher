#include "../engine/AAEngine.h"
#include "loadModel.h"
#include <glm/glm.hpp>
#include <iostream>
int main()
{

  // Test 1 - Pathing to and getting header info to a Model Using Assimp
  std::string model_sixColorSquare = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  showModelHeaderInfo(model_sixColorSquare);

  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();  // for engine use

  // Test 2 - Main Engine
  AAEngine engine;                                                    // init engine

  engine.setKeyStruct(keys);                                          // required to process keys/mouse

  AADisplay::getInstance()->setWindowTitle("this is a test title");   // test changing window title
  AADisplay::getInstance()->setWindowSize(1024, 768, 500, 250);       // test changing window size (and position)
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(0,0,0));    //test changing window clear color

  std::cout << "Engine running...\n";
  engine.run();                                                       // test run (main loop)
  std::cout << "Engine stopped.\n";

  std::cout << "Press Enter To Exit\n";
  char a;
  a = std::cin.get();

  return 0;

}