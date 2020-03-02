/*
  This file is for setting up defaults and movement commands on a First Person Perspective Camera.
  It is intended for testing moving around your scene while building it.
  After calling this on your engine, the camera will be able to fly freely.
  Hotkeys:
    MOUSE - look
    WASD - move
    SPACE - up
    LSHIFT+SPACE - down
    MOUSEWHEEL - change move speed
    TAB - toggle cursor
*/
#pragma once
#include <iostream>
#include "FPPWorldLocation.h"
#include "../../AAEngine.h"

#define UP mearly::FPPWorldLocation::getWorldUp()

const float startingSpeed = 33.5f;

extern AAWorld mainWorld;

// returns unique cam id
int setGodCamWithMovement()
{
  mainWorld.setToPerspectiveMouseHandling();
  mainWorld.setCursorToDisabled();

  static int mainCamId;
  mainCamId = mainWorld.addCamera();

  mainWorld.setRenderDistance(mainCamId, 10000.f);

  static constexpr float MAXFLYSPEED = 400.f;
  static float currFlySpeed = startingSpeed;
  static float prevFlySpeed = startingSpeed;
  static constexpr float FlyIncrement = 2.5f;

  // position testing help
  static auto showLocation = []()
  {
    // show camera location
    //std::cout << "X: " << AAViewport::getInstance()->getPosition()->x
    //  << "  Z: " << AAViewport::getInstance()->getPosition()->z
    //  << "  Y: " << AAViewport::getInstance()->getPosition()->y << '\n';
    std::cout << "location info of camera turned off in code\n";
  };
  static auto showPitchAndYaw = []()
  {
    //std::cout << "PITCH: " << AAViewport::getInstance()->getPitch()
    //  << "  YAW: " << AAViewport::getInstance()->getYaw() << '\n';
    std::cout << "pitch yaw loc turned off in code\n";
  };

  const auto fpsKBNoClipFlying = [](AAKeyBoardInput& keys)
  {
    // this is a debug cam mover with no colliding
    static float fps60velocity = 0.f;
    static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    static glm::vec3 moveFront = glm::vec3(*mainWorld.getCamera(mainCamId).getFront());

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
      directionPlacement -= *mainWorld.getCamera(mainCamId).getRight() * fps60velocity;
    }
    if (keys.d)
    {
      directionPlacement += *mainWorld.getCamera(mainCamId).getRight() * fps60velocity;
    }

    // process going up and down
    if (!keys.leftShift && keys.spacebar)
    {
                              //world up
      directionPlacement += UP * fps60velocity;
    }
    if (keys.leftShift && keys.spacebar)
    {
      directionPlacement -= UP * fps60velocity;
    }

    mainWorld.getCamera(mainCamId).shiftCurrentPosition(directionPlacement);

    // reset local variables for next frame processing
    directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    moveFront = glm::vec3(*mainWorld.getCamera(mainCamId).getFront());
  };
  mainWorld.addToKeyHandling(fpsKBNoClipFlying);

  auto fpsMouseMovement = [](AAMouseInput& mouse)
  {
    mainWorld.getCamera(mainCamId).shiftYawAndPith(mouse.xOffset, mouse.yOffset);
  };
  mainWorld.addToMouseHandling(fpsMouseMovement);

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
  mainWorld.addToScrollHandling(fpsScrollChangesMoveSpeed);

  const auto escapeTogglesMouseDisplay = [](AAKeyBoardInput& keys)
  {
    if (keys.tab)
    {
      DISPLAY->toggleCursor();
      return true;
    }
    return false;
  };
  mainWorld.addToTimedOutKeyHandling(escapeTogglesMouseDisplay);


  return mainCamId;
}