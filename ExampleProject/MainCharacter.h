#pragma once
#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Types.h>
#include "BattleStats.h"
#include "FlashLight.h"

#if _DEBUG
#include <iostream>
#endif
#include "old/setplayerlightradius.h"

bool is_inventory_open = false;
bool is_fps_controls_set = false;
int character_cam = -1;
FlashLight flashlight;

struct MainCharacter : public BattleStats {

public:
  void Setup() {
    if (isInitialized) return;
    character_cam = AA::AddCamera();
    AA::SetMouseToDisabled();  // set to first person mode
    AA::SetMouseReadToFPP();
    AA::SetCamPosition(character_cam, AA::vec3(0, 0, 0));
    AA::SetCamPitch(character_cam, 0);
    AA::SetCamYaw(character_cam, -90);  // look down the -z axis

    // SETUP MOVEMENT SYSTEM
    {
      // for our move speed controls
      static const float DEFAULTMOVESPEED = 100.0f;
      static float sprint_bonus = 100.0f;
      // for our move direction and speed
      static float currFlySpeed = DEFAULTMOVESPEED;
      static float prevFlySpeed = currFlySpeed;
      static glm::vec3 moveDir = glm::vec3(0.f);
      static glm::vec3 frontFacingDir = AA::GetCamFront(character_cam);
      static glm::vec3 rightFacingDir = AA::GetCamRight(character_cam);

      const glm::vec3 GRAVITY = glm::vec3(0, -1, 0);

      static struct MoveBlock {
        bool forward = 0, backwards = 0, left = 0, right = 0;
        bool sprint = 0;
      } move;

#if _DEBUG
      std::cout << "fps controls: ";
      std::cout << "move = wasd";
#endif
      // set WASD key first person movement function
      AA::AddToKeyHandling([](AA::KeyboardInput& key) {
        if (is_inventory_open)
          return;

        if (key.w) {
          move.forward = true;
          frontFacingDir = AA::GetCamFront(character_cam);
          rightFacingDir = AA::GetCamRight(character_cam);
        } else if (!key.w) {
          move.forward = false;
        }

        if (key.s) {
          move.backwards = true;
          frontFacingDir = AA::GetCamFront(character_cam);
          rightFacingDir = AA::GetCamRight(character_cam);
        } else if (!key.s) {
          move.backwards = false;
        }

        if (key.a) {
          move.left = true;
          frontFacingDir = AA::GetCamFront(character_cam);
          rightFacingDir = AA::GetCamRight(character_cam);
        } else if (!key.a) {
          move.left = false;
        }

        if (key.d) {
          move.right = true;
          frontFacingDir = AA::GetCamFront(character_cam);
          rightFacingDir = AA::GetCamRight(character_cam);
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
          AA::ShiftCamPosition(character_cam, moveDir * frameCalculatedVelocity);
          moveDir = glm::vec3(0.f);
        }
        if (move.backwards) {
          moveDir -= frontFacingDir;
          AA::ShiftCamPosition(character_cam, moveDir * frameCalculatedVelocity);
          moveDir = glm::vec3(0.f);
        }
        if (move.right) {
          moveDir += rightFacingDir;
          AA::ShiftCamPosition(character_cam, moveDir * frameCalculatedVelocity);
          moveDir = glm::vec3(0.f);
        }
        if (move.left) {
          moveDir -= rightFacingDir;
          AA::ShiftCamPosition(character_cam, moveDir * frameCalculatedVelocity);
          moveDir = glm::vec3(0.f);
        }
      });

#if _DEBUG
      std::cout << ", look = mouse";
#endif
      // add mouse movement to change our view direction
      AA::AddToMouseHandling([](AA::MouseInput& cursor) {
        if (is_inventory_open)
          return;

        AA::ShiftCamPitchAndYaw(character_cam, cursor.yOffset, cursor.xOffset);

      });

    }

    // SETUP INVENTORY SYSTEM
    {
      // sounds
      static int open_sound_id = AA::AddSoundEffect("..\\ExampleProject\\res\\open_inv.ogg");
      static int close_sound_id = AA::AddSoundEffect("..\\ExampleProject\\res\\close_inv.ogg");
#if _DEBUG
      std::cout << ", tab = open inventory";
#endif   
      AA::SetGUIVisibility(is_inventory_open);        // start hidden or not depending on default
      AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb)-> bool { // set TAB to open/close Inventory
        if (kb.tab) {
          if (!is_inventory_open) {                   // Open Inventory
            AA::SetMouseToNormal();                   // show mouse when inventory is open
            AA::SetMouseReadToNormal();
            is_inventory_open = true;
            AA::SetGUIVisibility(is_inventory_open);
            AA::PlaySoundEffect(open_sound_id);
            return true;
          } else {                                    // Close Inventory
            AA::SetMouseToDisabled();                 // hide mouse when inventory is closed
            AA::SetMouseReadToFPP();
            is_inventory_open = false;
            AA::SetGUIVisibility(is_inventory_open);
            AA::PlaySoundEffect(close_sound_id);
            return true;
          }
        }
        return false;
      });

      // bunch of tiled buttons
      for (float i = .4f; i < .9f; i += .1f) {
        for (float j = -.8f; j < -.1f; j += .1f) {
          AA::AddButton(AA::vec2(i, j), AA::vec2(.045, .045), .85f, "..\\ExampleProject\\res\\textures\\tile.bmp");
        }
      }
    }

    // SETUP PLAYER RADIUS LIGHT
    //setplayerlightradius(character_cam);


    // SETUP FLASHLIGHT TOGGLE
    {
      AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb)-> bool {
        if (kb.f) {
          if (flashlight.isOn) 
          {
            flashlight.turn_off();
          }
          else
          {
            flashlight.turn_on();
          }
          return true;
        }
      });
    }

    isInitialized = true;
  };

  void Update() {
    if (flashlight.isOn){
      flashlight.position = AA::GetCamPosition(character_cam);
      flashlight.direction = AA::GetCamFront(character_cam);
      flashlight.lock_in_movement();
    }
  };

  void Update(float dt) {};

  void Teardown() {};

private:
  bool isInitialized = false;
};
