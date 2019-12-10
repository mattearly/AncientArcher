#pragma once
#include "../engine/AAEngine.h"
#include "../engine/AAInput.h"
#include <memory>
#include <iostream>

void testLaunchingDisplay()
{
  {
    AADisplay::getInstance()->setWindowTitle("Test Engine Startup - Press [ENTER] To Continue");  // test changing window title
    AADisplay::getInstance()->setWindowClearColor(glm::vec3(.2,.5,.1));        // test changing window clear color
  }

  // Test 2 - Main Engine
  AAEngine engine;                                                               // init engine                                           //

  auto handleKeys = [](AAKeyBoardInput& keys) {
    if (keys.enter)
    {
      glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), true);
    }
  };
  engine.addToKeyHandling(handleKeys);

  switch (engine.run())
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
    break;
  }
}
