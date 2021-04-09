#pragma once
#include <AncientArcher/AncientArcher.h>
#include <glm/glm.hpp>

bool fps_set = false;
void setfpscontrols(int cam)
{
  if (fps_set)
    return;
  fps_set = true;

  AA::SetReadMouseCurorAsFPP();
  AA::SetCursorToDisabled();

  static int the_cam_to_move = cam;

  // for our move speed controls
  static const float DEFAULTMOVESPEED = 10.f;
  static const float MAXSPEED = 400.f;
  static const float FLYINCR = 1.f;

  // for our move direction and speed
  static float currFlySpeed = DEFAULTMOVESPEED;
  static float prevFlySpeed = currFlySpeed;
  static glm::vec3 moveDir = glm::vec3(0.f);
  static glm::vec3 frontFacingDir = AA::GetCamFront(the_cam_to_move);
  static glm::vec3 rightFacingDir = AA::GetCamRight(the_cam_to_move);

  // add WASD key first person movement function
  const auto wasd = [](AA::KeyboardInput& key) {
    if (key.w)
    {
      moveDir += frontFacingDir;
    }
    else if (key.s)
    {
      moveDir -= frontFacingDir;
    }
    if (key.a)
    {
      moveDir -= rightFacingDir;
    }
    else if (key.d)
    {
      moveDir += rightFacingDir;
    }
  };
  AA::AddToKeyHandling(wasd);

  const auto camMove = [](float dt) {
    static float frameCalculatedVelocity = 0.f;
    frameCalculatedVelocity = dt * currFlySpeed;
    AA::ShiftCamPosition(the_cam_to_move, moveDir * frameCalculatedVelocity);
    moveDir = glm::vec3(0.f);
    frontFacingDir = AA::GetCamFront(the_cam_to_move);
    rightFacingDir = AA::GetCamRight(the_cam_to_move);
  };
  AA::AddToDeltaUpdate(camMove);

  // add mouse movement to change our view direction
  const auto mouselook = [](AA::MouseInput& cursor)
  {
    AA::ShiftCamPitchAndYaw(the_cam_to_move, cursor.yOffset, cursor.xOffset);
  };
  AA::AddToMouseHandling(mouselook);

  // add mouse scroll wheel to change fly speed
  const auto mousewheelflyspeed = [](AA::ScrollInput& wheel)
  {
    // set flyspeed when mouse wheel moves
    if (wheel.yOffset > 0.1f)
    {
      currFlySpeed += FLYINCR;
      wheel.yOffset = 0;
    }
    else if (wheel.yOffset < -0.1f)
    {
      currFlySpeed -= FLYINCR;
      wheel.yOffset = 0;
    }

    // cap flyspeed
    if (currFlySpeed >= MAXSPEED)
    {
      currFlySpeed = MAXSPEED;
    }
    else if (currFlySpeed <= 1.f)
    {
      currFlySpeed = 1.000001f;
    }

    // set flyspeed if it changed
    if (currFlySpeed != prevFlySpeed)
    {
      prevFlySpeed = currFlySpeed;
    }
  };
  AA::AddToScrollHandling(mousewheelflyspeed);
}