#pragma once
#include <GLFW\glfw3.h>
#include "../engine/AADisplay.h"
#include "../engine/AAEngine.h"
#include <iostream>
#include <mearly\Random.h>

void  testEngineFunctionsPossibilities()
{
  glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), false);

  AADisplay::getInstance()->setWindowTitle("Test Screen Color Pulse - Press [ENTER] To Continue");  // test changing window title

  AAEngine engine;
  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();           // keyinput set for engine to update
  engine.setKeyStruct(keys);                                                   // set keys to process keys/mouse    
  std::shared_ptr<AAMouseInput> mouse = std::make_shared<AAMouseInput>();      // mouseinput for the display
  engine.setMouseStruct(mouse);                                                //
  std::shared_ptr<AAScrollInput> scroll = std::make_shared<AAScrollInput>();   // 
  engine.setScrollStruct(scroll);                                              //

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

  auto customhandleinput = [](std::shared_ptr<AAKeyInput>& keys) {
    if (keys->enter)
    {
      glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), true);
    }
  };
  engine.addToKeyHandling(customhandleinput);

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