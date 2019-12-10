#pragma once
#include "../engine/AAEngine.h"
#include <memory>
#include <iostream>

void testRunBaseEngineLoop()
{
  glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), false);

  AADisplay::getInstance()->setWindowTitle("Test Engine Function in Loops - Press [ENTER] To Continue");  // test changing window title
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(0));        // test changing window clear color

  AAEngine engine;

  auto begincout = []() {std::cout << "->One Time Begin Run\n"; };                             // test adding functions to the 4 engine function lists
  engine.addToOnBegin(begincout);

  auto deltacout = [](float step) {std::cout << ".    process delta update: " << step << '\n'; };
  engine.addToDeltaUpdate(deltacout);

  auto rendercout = []() {std::cout << "..   process render\n"; };
  engine.addToOnRender(rendercout);

  auto customhandleinput = [](AAKeyBoardInput& keys) {
    std::cout << "...  process keyboard/mouse \n";
    if (keys.enter)
    {
      glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), true);
    }
  };
  engine.addToKeyHandling(customhandleinput);

  auto updatecout = []() {std::cout << ".... process update\n"; };
  engine.addToUpdate(updatecout);

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