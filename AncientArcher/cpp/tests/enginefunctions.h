#pragma once
#include <GLFW\glfw3.h>
#include "../engine/AADisplay.h"
#include "../engine/AAEngine.h"
#include "../engine/AAViewport.h"
#include "../engine/AALights.h"
#include <iostream>
#include "modelPaths.h"

void  testStressEngineFunctions()
{
  AAEngine engine;

  static DirectionalLight dirLight;
  dirLight.Direction = glm::vec3(0.15f, -1.f, 0.15f);
  dirLight.Ambient = glm::vec3(0.2f);
  dirLight.Diffuse = glm::vec3(0.5f);
  dirLight.Specular = glm::vec3(1.f);

  //AADisplay::getInstance()->setWindowTitle("Stress Functions - Press [ESC] To Quit");
  //AADisplay::getInstance()->setWindowClearColor(glm::vec3(.05, .5, .3));
  //AADisplay::getInstance()->setCursorToDisabled();
  //AAViewport::getInstance()->setRenderDistance(500.f);

  //static AAGameObject gameObj = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model, false, Shading::CELL);
  static AAGameObject gameObj2 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model2, false, Shading::CELL);
  //static AAGameObject gameObj3 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model3, false, Shading::CELL);
  //static AAGameObject gameObj4 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model4, false, Shading::CELL);
  //static AAGameObject gameObj5 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model5, true, Shading::CELL);
  static AAGameObject gameObj6 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model6, true, Shading::IMGTEX);
  //static AAGameObject gameObj7 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model7, true, Shading::IMGTEX);
  //static AAGameObject gameObj8 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model8, true, Shading::IMGTEX);
  static AAGameObject gameObj9 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model9, true, Shading::IMGTEX);

  static const float FlyIncrement = 0.4f;
  static float flySpeed = 10.f;
  static float prevFlySpeed = 0.f;

  // position testing help
  static auto showLocation = []()
  {
    // debug show location
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
  auto startFunc = []() {
    AAViewport::getInstance()->setCurrentPosition(glm::vec3(0, 5, 10));
    AAViewport::getInstance()->setCurrentPitch(-20.f);
    AAViewport::getInstance()->setCurrentYaw(270.f);
    gameObj2.translate(glm::vec3(0, 22, 10));
    //gameObj5.translate(glm::vec3(5,5,-5));
    gameObj6.translate(glm::vec3(0));
    //gameObj7.translate(glm::vec3(-51.625,0,0));
    //gameObj8.translate(glm::vec3(0));
    gameObj9.translate(glm::vec3(0));
    AAViewport::getInstance()->setToPerspective();
    AAViewport::getInstance()->setRenderDistance(500.f);
    AADisplay::getInstance()->setCursorToDisabled();
    AADisplay::getInstance()->setWindowClearColor(glm::vec3(.18f, .28f, .91f));
    AAViewport::getInstance()->setDirectionalLight(dirLight);
  };
  auto escapeTogglesMouseDisplay = [](AAKeyBoardInput& keys)
  {
    static bool isFPS = true;
    if (keys.esc)
    {
      if (isFPS)
      {
        AADisplay::getInstance()->setCursorToVisible();
        isFPS = false;
        return true;
      }
      else
      {
        AADisplay::getInstance()->setCursorToDisabled();
        isFPS = true;
        return true;
      }
    }
    return false;
  };
  auto changeRenderWithPageUpDown = [](AAKeyBoardInput& keys)
  {
    static const float MAX_RENDER_DISTANCE = 500.f;
    static const float MIN_RENDER_DISTANCE = 65.f;
    static const float INCR = 10.f;

    float render_distance = AAViewport::getInstance()->getRenderDistance();

    if (keys.pageUp)
    {
      render_distance += INCR;
    }
    if (keys.pageDown)
    {
      render_distance -= INCR;
    }

    if (render_distance > MAX_RENDER_DISTANCE)
    {
      render_distance = MAX_RENDER_DISTANCE;
    }
    else if (render_distance < MIN_RENDER_DISTANCE)
    {
      render_distance = MIN_RENDER_DISTANCE;
    }

    AAViewport::getInstance()->setRenderDistance(render_distance);

  };
  auto fpsKBNoClipFlying = [](AAKeyBoardInput& keys)
  {
    // this is a debug cam mover with no colliding - called every frame with deltaTime
    static float fps60velocity = 0.f;
    static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    static glm::vec3 moveFront = glm::vec3(*AAViewport::getInstance()->getFront());

    // setting our velocity based on 60fps (a guess since we don't have delta time here)
    fps60velocity = 0.0166f * flySpeed;

    // process WASD movement
    if (keys.w)
    {
      directionPlacement += moveFront * fps60velocity;
    }
    if (keys.s)
    {
      directionPlacement -= moveFront * fps60velocity;
    }
    if (keys.a)
    {
      directionPlacement -= *AAViewport::getInstance()->getRight() * fps60velocity;
    }
    if (keys.d)
    {
      directionPlacement += *AAViewport::getInstance()->getRight() * fps60velocity;
    }

    // process going up and down
    if (!keys.leftShift && keys.spacebar)
    {
      directionPlacement += AAViewport::getInstance()->WORLD_UP * fps60velocity;
    }
    if (keys.leftShift && keys.spacebar)
    {
      directionPlacement -= AAViewport::getInstance()->WORLD_UP * fps60velocity;
    }

    if (keys.n1)
    {
      std::cout << "num1 pressed\n";
    }

    if (keys.n2)
    {
      std::cout << "num2 pressed\n";
    }

    AAViewport::getInstance()->shiftCurrentPosition(directionPlacement);
    //_lighting->movePointLight(0, *AAViewport::getInstance()->getPosition(), _shader.get());  // debug point light stays at cam

    // reset local variables for next frame processing
    directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    moveFront = glm::vec3(*AAViewport::getInstance()->getFront());
  };
  auto fpsScrollChangesMoveSpeed = [](AAScrollInput& scroll)
  {
    // set flyspeed when mouse wheel moves
    if (scroll.yOffset > 0.1f)
    {
      flySpeed += FlyIncrement;
      scroll.yOffset = 0;
    }
    if (scroll.yOffset < -0.1f)
    {
      flySpeed -= FlyIncrement;
      scroll.yOffset = 0;
    }
    // cap flyspeed
    if (flySpeed >= 20.f)
    {
      flySpeed = 20.f;
    }
    if (flySpeed <= 1.f)
    {
      flySpeed = 1.000001f;
    }
    if (flySpeed != prevFlySpeed)
    {
      std::cout << "flySpeed: " << flySpeed << std::endl;    // show flySpeed in debug console if it changed since last frame.
      prevFlySpeed = flySpeed;
    }
  };
  auto fpsMouseMovement = [](AAMouseInput& mouse)
  {
    AAViewport::getInstance()->shiftYawAndPith(mouse.xOffset, mouse.yOffset);
  };
  auto deltaMoveObjects = [](float dt)
  {
    static float totalTime = 0;
    totalTime += dt;
    //gameObj.translate(glm::vec3(dt * .5, 0, 0));
    gameObj2.translate(glm::vec3(0, 0, -sin(totalTime)));
    //gameObj3.translate(glm::vec3(0, 0, 0));
    //gameObj3.rotate(dt * .5f, glm::vec3(0, 1, 0));
    //gameObj4.translate(glm::vec3(0, 0, sin(totalTime)));
    //gameObj5.rotate(dt * .3f, glm::vec3(0, 1, 0));
    gameObj6.rotate(dt * .6f, glm::vec3(0, 1, 0));
  };
  auto drawObjects = []()
  {
    //gameObj.draw();
    gameObj2.draw();
    //gameObj3.draw();
    //gameObj4.draw();
    //gameObj5.draw();
    gameObj6.draw();
    //gameObj7.draw();
    //gameObj8.draw();
    gameObj9.draw();
  };

  // add functions to parts of engine loops
  engine.addToOnBegin(startFunc);
  engine.addToDeltaUpdate(deltaMoveObjects);
  engine.addToOnRender(drawObjects);
  engine.addToKeyHandling(fpsKBNoClipFlying);
  engine.addToKeyHandling(changeRenderWithPageUpDown);
  engine.addToTimedOutKeyHandling(escapeTogglesMouseDisplay);
  engine.addToScrollHandling(fpsScrollChangesMoveSpeed);
  engine.addToMouseHandling(fpsMouseMovement);
  //engine.addToUpdate(updatefunc);

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
