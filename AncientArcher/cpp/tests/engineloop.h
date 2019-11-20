#pragma once
#include "../engine/AAEngine.h"
#include <memory>
#include <iostream>

void testEngineLoop()
{
  glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), false);

  AADisplay::getInstance()->setWindowTitle("Engine Loop Demo");  // test changing window title
  AADisplay::getInstance()->setWindowSize(1024, 768, 500, 250);                // test changing window size (and position)
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(0));        // test changing window clear color

  AAEngine engine;
  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();           // keyinput set for engine to update
  engine.setKeyStruct(keys);                                                   // set keys to process keys/mouse    
  std::shared_ptr<AAMouseInput> mouse = std::make_shared<AAMouseInput>();      // mouseinput for the display
  engine.setMouseStruct(mouse);                                                //
  std::shared_ptr<AAScrollInput> scroll = std::make_shared<AAScrollInput>();   // 
  engine.setScrollStruct(scroll);                                              //

  auto func0 = []() {std::cout << "on Begin\n"; };                             // test adding functions to the 4 engine function lists
  engine.addToOnBegin(func0);
  auto func1 = []() {std::cout << "on Update\n"; };
  engine.addToUpdate(func1);
  auto func2 = [](float step) {std::cout << "on Delta Update: step: " << step << '\n'; };
  engine.addToDeltaUpdate(func2);
  auto func3 = []() {std::cout << "on Render\n"; };
  engine.addToOnRender(func3);
  auto handleKeys = [](std::shared_ptr<AAKeyInput>& keys) {
    std::cout << "Handling QWERTY keys\n";
    if (keys->esc)
    {
      glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), true);
    }
  };
  engine.addToKeyHandling(handleKeys);

  std::cout << "Engine Loop with Demo functions running...\n";
  int engine_ret = engine.run();                                               // test run (main loop)  
  switch (engine_ret)
  {
  case -1:
    std::cout << "Keys not set.\n";
    break;
  case -2:
    std::cout << "Mouse not set.\n";
    break;
  case -3:
    std::cout << "Scroll not set.\n";
    break;
  default:
    std::cout << "Engine stopped.\n";
    break;
  }
}