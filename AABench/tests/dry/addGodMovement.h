#pragma once
#include <iostream>
#include "FPPWorldLocation.h"
#include "../../AAEngine.h"

#define UP mearly::FPPWorldLocation::getWorldUp()

static constexpr float startingSpeed = 10.5f;
static constexpr float MAXFLYSPEED = 400.f;
static constexpr float FlyIncrement = 1.f;

extern AAWorld mainWorld;

/**
 * setGodCamWithMovement is for setting up defaults and movement commands on a First Person Perspective Camera.
 * * After calling this, the camera view will be able to fly freely.
 * Hotkeys:
 * * MOUSE          :  look
 * * WASD           :  move
 * * SPACE          :  up
 * * LSHIFT+SPACE   :  down
 * * MOUSEWHEEL+/-  :  change move speed
 * * TAB            :  toggle mouse cursor mode
*/
int setGodCamWithMovement()
{
  mainWorld.setCursorToDisabled();
  mainWorld.setToPerspectiveMouseHandling();

  static int mainCamId;
  mainCamId = mainWorld.addCamera();

  static float currFlySpeed = startingSpeed;
  static float prevFlySpeed = startingSpeed;

  // allows wasd too move you around basedon the direction the cam is facing
  const auto flying_wasd = [](AAKeyBoardInput& keys)
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
  mainWorld.addToKeyHandling(flying_wasd);

  // allows mouse movement to change the cam look direction while the mouse cursor is disabled
  const auto mouse_movement_look_while_cursor_disabled = [](AAMouseInput& mouse)
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
  mainWorld.addToMouseHandling(mouse_movement_look_while_cursor_disabled);

  // allows mouse scroll wheel to adjust move speed
  const auto mouse_scroll_changes_wasd_speed = [](AAScrollInput& scroll)
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
  mainWorld.addToScrollHandling(mouse_scroll_changes_wasd_speed);

  // allows tab to toggle the cursor from disabled(fpp movement mode) or visible(inventory mode)
  const auto tab_toggles_cursor_disabled_or_visible = [](AAKeyBoardInput& keys)
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
  mainWorld.addToTimedOutKeyHandling(tab_toggles_cursor_disabled_or_visible);

  return mainCamId;

}