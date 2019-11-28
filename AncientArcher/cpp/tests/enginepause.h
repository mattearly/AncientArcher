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
  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();           // keyinput set for engine to update
  engine.setKeyStruct(keys);                                                   // set keys to process keys/mouse    
  std::shared_ptr<AAMouseInput> mouse = std::make_shared<AAMouseInput>();      // mouseinput for the display
  engine.setMouseStruct(mouse);                                                //
  std::shared_ptr<AAScrollInput> scroll = std::make_shared<AAScrollInput>();   // 
  engine.setScrollStruct(scroll);                                              //

  auto handleKeys = [](std::shared_ptr<AAKeyInput>& keys) {
    if (keys->enter)
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