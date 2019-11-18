#pragma once
#include "../engine/AAEngine.h"
#include <memory>
#include <iostream>

void testEngineMainDisplay()
{
  // Test 2 - Main Engine
  AAEngine engine;                                                    // init engine

  AADisplay::getInstance()->setWindowTitle("this is a test title");   // test changing window title
  AADisplay::getInstance()->setWindowSize(1024, 768, 500, 250);       // test changing window size (and position)
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(.5, .5, .5));    // test changing window clear color

  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();  // keyinput set for engine to update
  engine.setKeyStruct(keys);                                          // set keys to process keys/mouse  

  std::cout << "Engine running...\n";
  int engine_ret = engine.run();

  // test run (main loop)
  switch (engine_ret)
  {
  case -1:
    std::cout << "Keys not set.\n";
    break;
  }

  std::cout << "Engine stopped.\n";

}