/*
  This file is for setting up defaults and movement commands on a First Person Perspective Camera.
  It is intended for testing moving around your scene while building it.
  After calling this on your engine, the camera will be able to fly freely.
  Hotkeys:
    MOUSE          :  look
    WASD           :  move
    SPACE          :  up
    LSHIFT+SPACE   :  down
    MOUSEWHEEL+/-  :  change move speed
    TAB            :  toggle mouse cursor
*/
#pragma once
#include <iostream>
#include "FPPWorldLocation.h"
#include "../../AAEngine.h"

#define UP mearly::FPPWorldLocation::getWorldUp()

const float startingSpeed = 10.5f;

extern AAWorld mainWorld;

// returns unique cam id
int setGodCamWithMovement()
{
  mainWorld.setCursorToDisabled();
  mainWorld.setToPerspectiveMouseHandling();

  static int mainCamId;
  mainCamId = mainWorld.addCamera();
  mainWorld.setRenderDistance(mainCamId, 10000.f);

  static constexpr float MAXFLYSPEED = 400.f;
  static float currFlySpeed = startingSpeed;
  static float prevFlySpeed = startingSpeed;
  static constexpr float FlyIncrement = 2.5f;

  const auto flying_wasd = [](AAKeyBoardInput& keys)
  {
    // this is a debug cam mover with no colliding
    static float fps60velocity = 0.f;
    static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
    static glm::vec3 moveFront = glm::vec3(*mainWorld.getCamera(mainCamId).getFront());

    if (mainWorld.getMouseReportingMode() == MouseReporting::PERSPECTIVE)
    {
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
    }
  };
  mainWorld.addToKeyHandling(flying_wasd);

  auto fps_or_standard_togglable = [](AAMouseInput& mouse)
  {
    if (mainWorld.getMouseReportingMode() == MouseReporting::PERSPECTIVE)
    {
      mainWorld.getCamera(mainCamId).shiftYawAndPith(mouse.xOffset, mouse.yOffset);
    }
    else // if (mainWorld.getMouseReportingMode() == MouseReporting::STANDARD)
    {
      // dont change the cam location
    }
  };
  mainWorld.addToMouseHandling(fps_or_standard_togglable);

  auto scrollChangesMoveSpeed = [](AAScrollInput& scroll)
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
      prevFlySpeed = currFlySpeed;
    }
  };
  mainWorld.addToScrollHandling(scrollChangesMoveSpeed);

  const auto escapeTogglesMouseDisplay = [](AAKeyBoardInput& keys)
  {
    if (keys.tab)
    {
      if (mainWorld.getMouseReportingMode() == MouseReporting::PERSPECTIVE)
      {
        // show the mouse
        mainWorld.setCursorToEnabled();
        // change to a new reporting method
        mainWorld.setToStandardMouseHandling();

        // turn the key handling to not move the camera around, preserving cam  look the cam look
        // todo
      }
      else  // it is in standard calc, toggle to perspective
      {
        // dont show the mouse and change it to first person live control
        mainWorld.setCursorToDisabled();
        // set to perspective view handling which reports the mouse
        mainWorld.setToPerspectiveMouseHandling();
      }
      return true;
    }
    return false;
  };
  mainWorld.addToTimedOutKeyHandling(escapeTogglesMouseDisplay);

  return mainCamId;
}