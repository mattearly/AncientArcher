#pragma once
#include <AncientArcher/AncientArcher.h>
#include <glm/glm.hpp>

bool is_fps_controls_set = false;
extern bool is_inventory_open;

void setfpsplayercontrols(int cam) {
  if (is_fps_controls_set)
    return;
  is_fps_controls_set = true;

  AA::SetMouseToDisabled();
  AA::SetMouseReadToFPP();

  static int inherited_cam = cam;

  // starting cam position
  AA::SetCamPosition(inherited_cam, AA::vec3(0, 175, 175));
  AA::SetCamPitch(inherited_cam, 0);
  AA::SetCamYaw(inherited_cam, -90);

  // for our move speed controls
  static const float DEFAULTMOVESPEED = 100.0f;

  // character stats
  static float sprint_bonus = 100.0f;

  // for our move direction and speed
  static float currFlySpeed = DEFAULTMOVESPEED;
  static float prevFlySpeed = currFlySpeed;
  static glm::vec3 moveDir = glm::vec3(0.f);
  static glm::vec3 frontFacingDir = AA::GetCamFront(inherited_cam);
  static glm::vec3 rightFacingDir = AA::GetCamRight(inherited_cam);

  const glm::vec3 GRAVITY = glm::vec3(0,-1,0);

  static struct MoveBlock {
    bool forward = 0, backwards = 0, left = 0, right = 0;
    bool sprint = 0;
  } move;

  std::cout << "fps controls: ";
  std::cout << "move = wasd";
  // add WASD key first person movement function
  AA::AddToKeyHandling([](AA::KeyboardInput& key) {
    if (is_inventory_open)
      return;

    if (key.w) {
      move.forward = true;
      frontFacingDir = AA::GetCamFront(inherited_cam);
      rightFacingDir = AA::GetCamRight(inherited_cam);
    } else if (!key.w) {
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

    if (key.leftShift) {
      move.sprint = true;
    } else if (!key.leftShift) {
      move.sprint = false;
    }

  });

  // Velocified Movement every delta frame
  AA::AddToDeltaUpdate([](float dt) {
    if (is_inventory_open)
      return;

    float frameCalculatedVelocity = 0.f;
    frameCalculatedVelocity = dt * (move.sprint ? sprint_bonus + currFlySpeed : currFlySpeed);
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
  });

  std::cout << ", look = mouse";
  // add mouse movement to change our view direction
  AA::AddToMouseHandling([](AA::MouseInput& cursor) {
    if (is_inventory_open)
      return;

    AA::ShiftCamPitchAndYaw(inherited_cam, cursor.yOffset, cursor.xOffset);

  });

}