#pragma once
#include <GLFW\glfw3.h>
#include "../engine/AADisplay.h"
#include "../engine/AAEngine.h"
#include "../engine/AAViewport.h"
#include <iostream>

void  testStressEngineFunctions()
{
  AADisplay::getInstance()->setWindowTitle("Stress Functions - Press [ESC] To Quit");
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(.05, .5, .3));

  AAEngine engine;
  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();           // keyinput set for engine to update
  engine.setKeyStruct(keys);                                                   // set keys to process keys/mouse    
  std::shared_ptr<AAMouseInput> mouse = std::make_shared<AAMouseInput>();      // mouseinput for the display
  engine.setMouseStruct(mouse);                                                //
  std::shared_ptr<AAScrollInput> scroll = std::make_shared<AAScrollInput>();   // 
  engine.setScrollStruct(scroll);

  AAViewport::getInstance()->setToPerspective();
  AADisplay::getInstance()->setCursorToDisabled();

  std::string model = "..\\AncientArcher\\localdata\\6ColorSquare.obj";
  std::string model2 = "..\\AncientArcher\\localdata\\qBertSquare.obj";
  std::string model3 = "..\\AncientArcher\\localdata\\cyl.obj";
  std::string model4 = "..\\AncientArcher\\localdata\\hello.obj";
  //std::string model_sixColorSquare = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  static AAGameObject gameObj = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(model);
  static AAGameObject gameObj2 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(model2);
  static AAGameObject gameObj3 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(model3);
  static AAGameObject gameObj4 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(model4);
  static const float FlyIncrement = 0.4f;
  static float flySpeed = 5.f;
  static float prevFlySpeed = 0.f;

  // funcs we can use in our lambdas
  static auto showLocation = []()
  {      // debug show location
    std::cout << "Loc(x,z,y): ("
      << AAViewport::getInstance()->getPosition()->x << ","
      << AAViewport::getInstance()->getPosition()->z << ","
      << AAViewport::getInstance()->getPosition()->y << ")\n";
  };
  static auto showPitchAndYaw = []()
  {
    std::cout << "pitch: " << AAViewport::getInstance()->getPitch() << " yaw: " << AAViewport::getInstance()->getYaw() << '\n';
  };

  // lambda funcs for the engine
  auto customKeyInput = [](std::shared_ptr<AAKeyInput>& keys)
  {
    if (keys->esc)
    {
      glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), true);
    }

    // this is a debug cam mover with no colliding - called every frame with deltaTime
    static float fps60velocity = 0.f;
    static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    static glm::vec3 moveFront = glm::vec3(*AAViewport::getInstance()->getFront());

    // setting our velocity based on 60fps guess (since no delta time avail here)
    fps60velocity = 0.0166f * flySpeed;

    // process wasd
    if (keys->w)
    {
      directionPlacement += moveFront * fps60velocity;
    }
    if (keys->s)
    {
      directionPlacement -= moveFront * fps60velocity;
    }
    if (keys->a)
    {
      directionPlacement -= *AAViewport::getInstance()->getRight() * fps60velocity;
    }
    if (keys->d)
    {
      directionPlacement += *AAViewport::getInstance()->getRight() * fps60velocity;
    }

    // process going up and down
    if (!keys->leftShift && keys->spacebar)  // spacebar goes up
    {
      directionPlacement += AAViewport::getInstance()->WORLD_UP * fps60velocity;
    }
    if (keys->leftShift && keys->spacebar)   // shift + spacebar goes down
    {
      directionPlacement -= AAViewport::getInstance()->WORLD_UP * fps60velocity;
    }

    if (keys->n1)
    {
      std::cout << "num1 pressed\n";
    }

    if (keys->n2)
    {
      std::cout << "num2 pressed\n";
    }

    AAViewport::getInstance()->shiftCurrentPosition(directionPlacement);
    //_lighting->movePointLight(0, *AAViewport::getInstance()->getPosition(), _shader.get());  // debug point light stays at cam

    // reset local variables for next frame processing
    directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    moveFront = glm::vec3(*AAViewport::getInstance()->getFront());
  };
  auto customScrollInput = [](std::shared_ptr<AAScrollInput>& scroll)
  {
    // set flyspeed when mouse wheel moves
    if (scroll->yOffset > 0.1f)
    {
      flySpeed += FlyIncrement;
      scroll->yOffset = 0;
    }
    if (scroll->yOffset < -0.1f)
    {
      flySpeed -= FlyIncrement;
      scroll->yOffset = 0;
    }
    // cap flyspeed
    if (flySpeed >= 10.f)
    {
      flySpeed = 10.f;

    }
    if (flySpeed <= 1.f)
    {
      flySpeed = 1.000001f;
    }

    if (flySpeed != prevFlySpeed)  // show flySpeed in debug console if it changed since last frame.
    {
      std::cout << "flySpeed: " << flySpeed << std::endl;
      prevFlySpeed = flySpeed;
    }
  };
  auto customMouseInput = [](std::shared_ptr<AAMouseInput>& mouse)
  {
    AAViewport::getInstance()->shiftYawAndPith(mouse->xOffset, mouse->yOffset);
  };
  auto moveSquare = [](float dt) {
    gameObj.translate(glm::vec3(sin(dt), 0, 0));
    gameObj2.translate(glm::vec3(-sin(dt), 0, 0));
    gameObj3.translate(glm::vec3(0, 0, -sin(dt)));
    gameObj4.translate(glm::vec3(0, 0, sin(dt)));
  };
  auto drawsquare = []()
  {
    gameObj.draw();
    gameObj2.draw();
    gameObj3.draw();
    gameObj4.draw();
  };

  // add functions to parts of engine loops
  engine.addToDeltaUpdate(moveSquare);
  //engine.addToOnBegin(beginfunc);
  //engine.addToUpdate(updatefunc);
  engine.addToKeyHandling(customKeyInput);
  engine.addToScrollHandling(customScrollInput);
  engine.addToMouseHandling(customMouseInput);
  engine.addToOnRender(drawsquare);


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