#pragma once
#include "../engine/AAEngine.h"
#include "../engine/AAInput.h"
#include <memory>
#include <iostream>

void testEngineMainDisplay()
{

  {
    AADisplay::getInstance()->setWindowTitle("Set by testEngineMainDisplay()");  // test changing window title
    AADisplay::getInstance()->setWindowSize(1024, 768, 500, 250);                // test changing window size (and position)
    AADisplay::getInstance()->setWindowClearColor(glm::vec3(.5, .5, .5));        // test changing window clear color
  }

  // Test 2 - Main Engine
  AAEngine engine;  // init engine

  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();           // keyinput set for engine to update
  engine.setKeyStruct(keys);                                                   // set keys to process keys/mouse    
  std::shared_ptr<AAMouseInput> mouse = std::make_shared<AAMouseInput>();      // mouseinput for the display
  engine.setMouseStruct(mouse);                                                //
  std::shared_ptr<AAScrollInput> scroll = std::make_shared<AAScrollInput>();   // 
  engine.setScrollStruct(scroll);                                              //

  auto handleKeys = [](std::shared_ptr<AAKeyInput>& keys) {
    std::cout << "Handling QWERTY keys\n";
    if (keys->esc)
    {
      glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), true);
    }
  };
  engine.addToKeyHandling(handleKeys);

  std::cout << "Press ESC To Continue...\n";
  int engine_ret = engine.run();                                                // test run (main loop)
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