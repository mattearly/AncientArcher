#pragma once
#pragma once
#include "../engine/AAEngine.h"
#include <memory>

void testEnginePause()
{
  AAEngine engine;

  AADisplay::getInstance()->setWindowTitle("PAUSE - close window to continue");  // test changing window title
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(.0, .1, .0));        // test changing window clear color

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