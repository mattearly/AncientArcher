#pragma once
#include "../engine/AAInput.h"
#include "../engine/AAEngine.h"

void addgodmovement(AAEngine& engine)
{  
  static float flySpeed = 20.f;  
  static float prevFlySpeed = 0.f;
  static const float FlyIncrement = 0.4f;  

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
    //_lighting->movePointLight(0, *AAViewport::getInstance()->getPosition(), _shader.get());  // debug point light stays at cam

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
      //std::cout << "flySpeed: " << flySpeed << std::endl;    // show flySpeed in debug console if it changed since last frame.
      prevFlySpeed = flySpeed;
    }
  };
  engine.addToScrollHandling(fpsScrollChangesMoveSpeed);

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
  engine.addToTimedOutKeyHandling(escapeTogglesMouseDisplay);
}