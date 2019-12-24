#pragma once
#include <glm\ext\vector_float3.hpp>
#include "../../engine/AAEngine.h"
#include <iostream>

void addGodMovement(AAEngine& engine)
{  
  static const float MAXFLYSPEED = 400.f;
  static float currFlySpeed = 20.f;  
  static float prevFlySpeed = 0.f;
  static const float FlyIncrement = 9.f;  

  // position testing help
  static auto showLocation = []()
  {
    // show camera location
    std::cout << "X: " << AAViewport::getInstance()->getPosition()->x
      << "  Z: " << AAViewport::getInstance()->getPosition()->z
      << "  Y: " << AAViewport::getInstance()->getPosition()->y << '\n';
  };
  static auto showPitchAndYaw = []()
  {
    std::cout << "PITCH: " << AAViewport::getInstance()->getPitch()
      << "  YAW: " << AAViewport::getInstance()->getYaw() << '\n';

  };
  auto fpsKBNoClipFlying = [](AAKeyBoardInput& keys)
  {
    // this is a debug cam mover with no colliding - called every frame with deltaTime
    static float fps60velocity = 0.f;
    static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    static glm::vec3 moveFront = glm::vec3(*AAViewport::getInstance()->getFront());

    // setting our velocity based on 60fps (a guess since we don't have delta time here)
    fps60velocity = 0.0166f * currFlySpeed;

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

    }

    if (keys.n2)
    {

    }

    if (keys.mouseButton1)
    {

    }

    if (keys.mouseButton3)
    {

    }

    AAViewport::getInstance()->shiftCurrentPosition(directionPlacement);

    // reset local variables for next frame processing
    directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    moveFront = glm::vec3(*AAViewport::getInstance()->getFront());
  };
  engine.addToKeyHandling(fpsKBNoClipFlying);
  
  auto fpsMouseMovement = [](AAMouseInput& mouse)
  {
    AAViewport::getInstance()->shiftYawAndPith(mouse.xOffset, mouse.yOffset);
  };
  engine.addToMouseHandling(fpsMouseMovement);

  auto fpsScrollChangesMoveSpeed = [](AAScrollInput& scroll)
  {
    // set flyspeed when mouse wheel moves
    if (scroll.yOffset > 0.1f)
    {
      currFlySpeed += FlyIncrement;
      scroll.yOffset = 0;
    }
    if (scroll.yOffset < -0.1f)
    {
      currFlySpeed -= FlyIncrement;
      scroll.yOffset = 0;
    }
    // cap flyspeed
    if (currFlySpeed >= MAXFLYSPEED)
    {
      currFlySpeed = MAXFLYSPEED;
    }
    if (currFlySpeed <= 1.f)
    {
      currFlySpeed = 1.000001f;
    }
    if (currFlySpeed != prevFlySpeed)
    {
      std::cout << "+++\n";
      std::cout << "FLYSPEED: " << currFlySpeed << std::endl;    // show flySpeed in debug console if it changed since last frame.
      showLocation();
      showPitchAndYaw();
      prevFlySpeed = currFlySpeed;
    }
  };
  engine.addToScrollHandling(fpsScrollChangesMoveSpeed);

  auto escapeTogglesMouseDisplay = [](AAKeyBoardInput& keys)
  {
    bool mouseLocked = true;
    if (keys.esc)
    {
      AADisplay::getInstance()->toggleCursor();
      return true;
    }
    return false;
  };
  engine.addToTimedOutKeyHandling(escapeTogglesMouseDisplay);
}