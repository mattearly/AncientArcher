#pragma once
#include <GLFW\glfw3.h>
#include "../engine/AADisplay.h"
#include "../engine/AAEngine.h"
#include <iostream>
#include <mearly\Random.h>
#include "../engine/AAGameObject.h"
#include "../engine/AAOGLGraphics.cpp"

void  testEngineFunctionsPossibilities()
{
  glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), false);

  AADisplay::getInstance()->setWindowTitle("Engine Game Objects In functions Test");  // test changing window title
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(.1, .1, .7));        // test changing window clear color

  AAEngine engine;
  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();           // keyinput set for engine to update
  engine.setKeyStruct(keys);                                                   // set keys to process keys/mouse    
  std::shared_ptr<AAMouseInput> mouse = std::make_shared<AAMouseInput>();      // mouseinput for the display
  engine.setMouseStruct(mouse);                                                //
  std::shared_ptr<AAScrollInput> scroll = std::make_shared<AAScrollInput>();   // 
  engine.setScrollStruct(scroll);           
  
  std::string model = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  
  AAGameObject gameObj = AAOGLGraphics::getInstance()->loadModelWithAssimpToOpenGL(model);
  //auto beginFunc = []() {
  //  
  //};                             
  //engine.addToOnBegin(beginFunc);

  //auto deltacout = [](float step) {
  //  std::cout << ".    timestep: " << step << '\n';
  //};
  //engine.addToDeltaUpdate(deltacout);

  auto rendercout = []() 
  {
    gameObj.draw();
  };
  engine.addToOnRender(rendercout);

  auto customhandleinput = [](std::shared_ptr<AAKeyInput>& keys) 
  {
    std::cout << "...  process keyboard/mouse [SPACE TO STOP]\n";
    if (keys->spacebar)
    {
      glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), true);
    }
  };
  engine.addToKeyHandling(customhandleinput);

  auto updatecout = []() {std::cout << ".... update\n"; };
  engine.addToUpdate(updatecout);

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