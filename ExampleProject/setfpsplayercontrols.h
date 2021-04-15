#pragma once
#include <AncientArcher/AncientArcher.h>
#include <glm/glm.hpp>

bool fps_set = false;
bool is_inventory_open = false;

void setfpsplayercontrols(int cam) {
  if (fps_set)
    return;
  fps_set = true;

  AA::SetMouseToDisabled();
  AA::SetMouseReadToFPP();

  static int inherited_cam = cam;

  // for our move speed controls
  static const float DEFAULTMOVESPEED = 10.f;
  static const float MAXSPEED = 400.f;
  static const float FLYINCR = 1.f;

  // for our move direction and speed
  static float currFlySpeed = DEFAULTMOVESPEED;
  static float prevFlySpeed = currFlySpeed;
  static glm::vec3 moveDir = glm::vec3(0.f);
  static glm::vec3 frontFacingDir = AA::GetCamFront(inherited_cam);
  static glm::vec3 rightFacingDir = AA::GetCamRight(inherited_cam);

  static struct MoveBlock {
    bool forward = 0, backwards = 0, left = 0, right = 0;
  } move;
  // add WASD key first person movement function
  const auto wasd = [](AA::KeyboardInput& key) {
    if (is_inventory_open)
      return;

    if (key.w) {
      move.forward = true;
      frontFacingDir = AA::GetCamFront(inherited_cam);
      rightFacingDir = AA::GetCamRight(inherited_cam);

    } else     if (!key.w) {
      move.forward = false;
    }

    if (key.s) {
      move.backwards = true;
      frontFacingDir = AA::GetCamFront(inherited_cam);
      rightFacingDir = AA::GetCamRight(inherited_cam);

    } else if (!key.s) {
      move.backwards = false;

    }

    if (key.a) {
      move.left = true;
      frontFacingDir = AA::GetCamFront(inherited_cam);
      rightFacingDir = AA::GetCamRight(inherited_cam);


    } else if (!key.a) {
      move.left = false;
    }



    if (key.d) {
      move.right = true;
      frontFacingDir = AA::GetCamFront(inherited_cam);
      rightFacingDir = AA::GetCamRight(inherited_cam);

    } else if (!key.d) {
      move.right = false;

    }
  };
  AA::AddToKeyHandling(wasd);

  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb)-> bool {
    if (kb.tab) {
      if (!is_inventory_open) {
        // (open inventory) mouse on screen
        AA::SetMouseToNormal();
        AA::SetMouseReadToNormal();
        is_inventory_open = true;
        return true;
      } else {
        // (close inventory) mouse in fpp hidden snapping ot middle mode
        AA::SetMouseToDisabled();
        AA::SetMouseReadToFPP();
        is_inventory_open = false;
        return true;
      }
    }
    return false;
  });

  const auto camMove = [](float dt) {
    float frameCalculatedVelocity = 0.f;
    frameCalculatedVelocity = dt * currFlySpeed;
    if (move.forward) {
      moveDir += frontFacingDir;
      AA::ShiftCamPosition(inherited_cam, moveDir * frameCalculatedVelocity);
      moveDir = glm::vec3(0.f);

    }
    if (move.backwards) {
      moveDir -= frontFacingDir;
      AA::ShiftCamPosition(inherited_cam, moveDir * frameCalculatedVelocity);
      moveDir = glm::vec3(0.f);

    }
    if (move.right) {
      moveDir += rightFacingDir;
      AA::ShiftCamPosition(inherited_cam, moveDir * frameCalculatedVelocity);
      moveDir = glm::vec3(0.f);
    }
    if (move.left) {
      moveDir -= rightFacingDir;
      AA::ShiftCamPosition(inherited_cam, moveDir * frameCalculatedVelocity);
      moveDir = glm::vec3(0.f);
    }
  };
  AA::AddToDeltaUpdate(camMove);

  // add mouse movement to change our view direction
  const auto mouselook = [](AA::MouseInput& cursor)
  {
    if (!is_inventory_open)
      AA::ShiftCamPitchAndYaw(inherited_cam, cursor.yOffset, cursor.xOffset);
  };
  AA::AddToMouseHandling(mouselook);

  // add mouse scroll wheel to change fly speed
  const auto mousewheelflyspeed = [](AA::ScrollInput& wheel)
  {
    // set flyspeed when mouse wheel moves
    if (wheel.yOffset > 0.1f) {
      currFlySpeed += FLYINCR;
    } else if (wheel.yOffset < -0.1f) {
      currFlySpeed -= FLYINCR;
    }

    // cap flyspeed
    if (currFlySpeed >= MAXSPEED) {
      currFlySpeed = MAXSPEED;
    } else if (currFlySpeed <= 1.f) {
      currFlySpeed = 1.000001f;
    }

    // set flyspeed if it changed
    if (currFlySpeed != prevFlySpeed) {
      prevFlySpeed = currFlySpeed;
    }
  };
  AA::AddToScrollHandling(mousewheelflyspeed);
}