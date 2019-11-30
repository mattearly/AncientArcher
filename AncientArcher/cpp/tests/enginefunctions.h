#pragma once
#include <GLFW\glfw3.h>
#include "../engine/AADisplay.h"
#include "../engine/AAEngine.h"
#include "../engine/AAViewport.h"
#include <iostream>
#include <mearly\Random.h>

void  testStressEngineFunctions()
{
  glfwSetWindowShouldClose(AADisplay::getInstance()->getWindow(), false);

  AADisplay::getInstance()->setWindowTitle("Test Engine Game Objects - Press [ENTER] To Continue");  // test changing window title
  //AADisplay::getInstance()->setWindowClearColor(glm::vec3(.5));        // test changing window clear color

  AAEngine engine;                                                             //
  //AAViewport::getInstance()->setToPerspective();
  std::shared_ptr<AAKeyInput> keys = std::make_shared<AAKeyInput>();           // keyinput set for engine to update
  engine.setKeyStruct(keys);                                                   // set keys to process keys/mouse    
  std::shared_ptr<AAMouseInput> mouse = std::make_shared<AAMouseInput>();      // mouseinput for the display
  engine.setMouseStruct(mouse);                                                //
  std::shared_ptr<AAScrollInput> scroll = std::make_shared<AAScrollInput>();   // 
  engine.setScrollStruct(scroll);                                              //

  auto initfunction = []() {
    AADisplay::getInstance()->setCursorToDisabled();
    AAViewport::getInstance()->setToPerspective();
  };
  engine.addToOnBegin(initfunction);

  //std::string model = "C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj";
  std::string model = "..\\AncientArcher\\localdata\\6ColorSquare.obj";
  static AAGameObject gameObj = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(model);
  auto drawsquare = []()
  {
    gameObj.draw();
  };
  engine.addToOnRender(drawsquare);

  static const float FlyIncrement = 0.4f;
  static float flySpeed = 5.f;
  static float prevFlySpeed = 0.f;

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

  auto customKeyInput = [](std::shared_ptr<AAKeyInput>& keys)
  {
    if (keys->enter)
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
      showLocation();
    }
    if (keys->s)
    {
      directionPlacement -= moveFront * fps60velocity;
      showLocation();
    }
    if (keys->a)
    {
      directionPlacement -= *AAViewport::getInstance()->getRight() * fps60velocity;
      showLocation();
    }
    if (keys->d)
    {
      directionPlacement += *AAViewport::getInstance()->getRight() * fps60velocity;
      showLocation();
    }

    // process going up and down
    if (!keys->leftShift && keys->spacebar)  // spacebar goes up
    {
      directionPlacement += AAViewport::getInstance()->WORLD_UP * fps60velocity;
      showLocation();
    }
    if (keys->leftShift && keys->spacebar)   // shift + spacebar goes down
    {
      directionPlacement -= AAViewport::getInstance()->WORLD_UP * fps60velocity;
      showLocation();
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
  engine.addToKeyHandling(customKeyInput);
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
  engine.addToScrollHandling(customScrollInput);
  auto customMouseInput = [](std::shared_ptr<AAMouseInput>& mouse)
  {
    AAViewport::getInstance()->shiftYawAndPith(mouse->xOffset, mouse->yOffset);
    showPitchAndYaw();
  };
  engine.addToMouseHandling(customMouseInput);

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