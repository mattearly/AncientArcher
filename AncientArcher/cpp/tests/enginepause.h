#pragma once
#pragma once
#include "../engine/AAEngine.h"
#include <memory>

void testEnginePause()
{
  glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), false);

  AADisplay::getInstance()->setWindowTitle("Engine Pause - Press [ENTER] To Continue");  // test changing window title
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(.0,.1,.0));        // test changing window clear color

  AAEngine engine;

  auto deltacout = [](float step) {
    static float timePassed = 0;
    timePassed += step;
    float newcolor = abs(sin(timePassed));
    AADisplay::getInstance()->setWindowClearColor(
      glm::vec3(
        newcolor,
        newcolor,
        newcolor
      )
    );        // test changing window clear color to random values
  };
  engine.addToDeltaUpdate(deltacout);

  auto handleKeys = [](AAKeyBoardInput& keys) {
    if (keys.enter)
    {
      glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), true);
    }
  };
  engine.addToKeyHandling(handleKeys);

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
    break;
  }
}