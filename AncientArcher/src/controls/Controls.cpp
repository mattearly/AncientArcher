#include <glm/glm.hpp>

#include "../globals.h"
#include "Controls.h"

#include "../Constraints.h"
#include "../player/Player.h"
#include "../sound/Sound.h"
#include "../game/Game.h"

#include <iostream>
#include <math.h>


Controls::Controls() {
  firstMouse = true;
  mouseSensitivity = 0.09f;
  lastX = 0.f;
  lastY = 0.f;
}

Controls::~Controls() {}

void Controls::mouseMovement(float xpos, float ypos) {

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  camera.Yaw += xoffset;
  camera.Pitch += yoffset;

  if (camera.Pitch > 89.0f) {
    camera.Pitch = 89.0f;
  }
  else if (camera.Pitch < -89.0f) {
    camera.Pitch = -89.0f;
  }

  camera.updateCameraVectors();

}

void Controls::keyboardInput(Player *player, Pickups *pickups, float dtime) {

  movedir.positionChanged = true;  //for footsteps

  if (glfwGetMouseButton(display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // main left click action 
    //if (player->getLastAttackTime() + player->getAttackSpeed() < gameTime) {
      //player->attack(gametime);
    //}
  }

  if (glfwGetKey(display.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(display.window, true);
  }

  if (glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.boost = true;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_W) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.back = false;
      movedir.forward = true;
      movedir.positionChanged = true;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_S) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.forward = false;
      movedir.back = true;
      movedir.positionChanged = true;

    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_A) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.right = false;
      movedir.left = true;
      movedir.positionChanged = true;

    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_D) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.left = false;
      movedir.right = true;
      movedir.positionChanged = true;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (movedir.onGround && movedir.canJumpAgain) {  //can jump again is to make the spacebar spam by holding it down not work
      movedir.jumped = true;
      movedir.canJumpAgain = false;
    }
  }

  if (glfwGetKey(display.window, GLFW_KEY_0) == GLFW_PRESS) {
    player->selectWeapon(0);
  }

  if (glfwGetKey(display.window, GLFW_KEY_1) == GLFW_PRESS) {
    player->selectWeapon(1);
  }

  if (glfwGetKey(display.window, GLFW_KEY_2) == GLFW_PRESS) {
    player->selectWeapon(2);
  }

  if (glfwGetKey(display.window, GLFW_KEY_W) == GLFW_RELEASE) {
    movedir.forward = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_S) == GLFW_RELEASE) {
    movedir.back = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_A) == GLFW_RELEASE) {
    movedir.left = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_D) == GLFW_RELEASE) {
    movedir.right = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    movedir.boost = false;
  }

  if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    movedir.canJumpAgain = true;
  }

  if (movedir.back || movedir.forward || movedir.left || movedir.right) {

    float velocity = player->getRunSpeed() * dtime;  // MOVEMENT SPEED CALC : based on player stats

    if (movedir.forward) {  // half speed if moving left or right while forward
      if (movedir.left || movedir.right) {
        velocity = player->getRunSpeed() / 2 * dtime;
      }
    }

    if (movedir.boost && movedir.forward) {  // boost while moving forward
      //velocity *= 2.3;  // velocity power
      velocity *= 2.0;  // velocity power
    }

    // locks moving foward and backwards to the x and z axii. 
    // Note: you can use the camera.Front instead of movefront to do a fly type thing 
    // while the Y is unlocked or you are jumping
    if (movedir.back || movedir.forward) {
      glm::vec3 moveFront = { camera.Front.x, 0.0f, camera.Front.z };
      if (movedir.forward) camera.Position += moveFront * velocity;
      if (movedir.back) camera.Position -= moveFront * velocity;
    }

    if (movedir.right) camera.Position += camera.Right * velocity;
    if (movedir.left) camera.Position -= camera.Right * velocity;

    ///* clamp to level - x */
    //if (camera.Position.x > world_width) {
    //  if (movedir.forward) camera.Position.x -= camera.Front.x * velocity;
    //  if (movedir.back)    camera.Position.x += camera.Front.x * velocity;
    //  if (movedir.right)   camera.Position.x -= camera.Right.x * velocity;
    //  if (movedir.left)    camera.Position.x += camera.Right.x * velocity;
    //} else if (camera.Position.x < 0.0f) {
    //  if (movedir.forward) camera.Position.x -= camera.Front.x * velocity;
    //  if (movedir.back)    camera.Position.x += camera.Front.x * velocity;
    //  if (movedir.right)   camera.Position.x -= camera.Right.x * velocity;
    //  if (movedir.left)    camera.Position.x += camera.Right.x * velocity;
    //}

    ///* clamp to level - z */
    //if (camera.Position.z > world_width) {
    //  if (movedir.forward) camera.Position.z -= camera.Front.z * velocity;
    //  if (movedir.back)    camera.Position.z += camera.Front.z * velocity;
    //  if (movedir.right)   camera.Position.z -= camera.Right.z * velocity;
    //  if (movedir.left)    camera.Position.z += camera.Right.z * velocity;
    //} else if (camera.Position.z < 0.0f) {
    //  if (movedir.forward) camera.Position.z -= camera.Front.z * velocity;
    //  if (movedir.back)    camera.Position.z += camera.Front.z * velocity; 
    //  if (movedir.right)   camera.Position.z -= camera.Right.z * velocity;
    //  if (movedir.left)    camera.Position.z += camera.Right.z * velocity;
    //}

    /* stop player from walking through impassable entities */
    for (auto e : entities) {
      if (e.collider != nullptr &&                                                           //collider is not null (potentially a blocker)
        abs(e.collider->impasse.location[0] - camera.Position.x) < (world_width / 2) + 1 &&  // and is close enough to be worth checking
        abs(e.collider->impasse.location[1] - camera.Position.y) < (world_width / 4) + 1 &&
        abs(e.collider->impasse.location[2] - camera.Position.z) < (world_width / 2) + 1)
      {
        float xPosOverlapLT = e.collider->impasse.location[0] + e.collider->impasse.size[0] / 2;
        float xPosOverlapGT = e.collider->impasse.location[0] - e.collider->impasse.size[0] / 2;
        float yPosOverlapLT = e.collider->impasse.location[2] + e.collider->impasse.size[2] / 2;
        float yPosOverlapGT = e.collider->impasse.location[2] - e.collider->impasse.size[2] / 2;
        float yTop = e.collider->impasse.location[1] + e.collider->impasse.size[1] / 2;
        float yBot = e.collider->impasse.location[1] - e.collider->impasse.size[1] / 2;
        if (camera.Position.x < xPosOverlapLT && camera.Position.x > xPosOverlapGT &&         // if inside an impassable on the X and Z of a square
          camera.Position.z < yPosOverlapLT && camera.Position.z > yPosOverlapGT &&
          (camera.Position.y < yTop && camera.Position.y > yBot))
        {
          if (movedir.forward) camera.Position.x -= camera.Front.x * velocity;          // X logic
          if (movedir.back)    camera.Position.x += camera.Front.x * velocity;
          if (movedir.right)   camera.Position.x -= camera.Right.x * velocity;
          if (movedir.left)    camera.Position.x += camera.Right.x * velocity;

          if (movedir.forward) camera.Position.z -= camera.Front.z * velocity;          // Z logic
          if (movedir.back)    camera.Position.z += camera.Front.z * velocity;
          if (movedir.right)   camera.Position.z -= camera.Right.z * velocity;
          if (movedir.left)    camera.Position.z += camera.Right.z * velocity;
        }
      }
    }

    //shader.setVec3("lightPos", camera.Position.x, camera.Position.y+0.4f, camera.Position.z);  // setting in game constructor
    lighting.movePointLight(0, *camera.getPosition());

    // FOOTSTEP SOUNDS
    if (movedir.timeSinceLastStep > TIMEBETWEENFOOTSTEPS - player->getRunSpeed() / 100.0f || (movedir.boost && !movedir.back && movedir.timeSinceLastStep > TIMEBETWEENFOOTSTEPS - (player->getRunSpeed() / 100.0f) * 2)) {
      if (movedir.positionChanged && movedir.onGround) {
        playfootstepsound();
        movedir.timeSinceLastStep = 0;
      }
    }
    else {
      movedir.timeSinceLastStep += dtime;
      //movedir.timeSinceLastStep += dtime;
    }

    // LEGPOWER PICKUP
    if (pickups->speedBoostAvail) {
      if (
        //y because boost loc is only x and y
        camera.Position.z >= pickups->speedBoostLoc.y - 1 &&
        camera.Position.z <= pickups->speedBoostLoc.y + 1 &&
        camera.Position.x >= pickups->speedBoostLoc.x - 1 &&
        camera.Position.x <= pickups->speedBoostLoc.x + 1) {
        player->increaseLegPower(15.0f);
        pickups->speedBoostAvail = false;
        playequipgearsound();
      }
    }

    // ATTACKSPEED PICKUP
    if (pickups->attackBoostAvail) {
      if (
        //y because boost loc is only x and y
        camera.Position.z >= pickups->attackBoostLoc.y - 1 &&
        camera.Position.z <= pickups->attackBoostLoc.y + 1 &&
        camera.Position.x >= pickups->attackBoostLoc.x - 1 &&
        camera.Position.x <= pickups->attackBoostLoc.x + 1) {
        player->increaseAttackSpeed(0.1f);
        pickups->attackBoostAvail = false;
        playequipgearsound();
      }
    }
  }

  /* Jump System */

  // PHASE 1: frame of liftoff
  if (movedir.jumped) {
    movedir.onGround = false;
    movedir.jumped = false;
    playgruntsound();
  }

  // PHASE 2: rising velocity
  else if (!movedir.onGround && !movedir.falling) {

    camera.Position.y += camera.WorldUp.y * player->getRisingSpeed() * dtime; // RISING SPEED CALC: jump speed based on LegPower Player Stat
    
    if (camera.Position.y > player->getJumpHeight() + camera.camstart[1]) { // MAX HEIGHT CALC: jump height based on LegPower Player Stat
      movedir.falling = true;
    }
  }

  // PHASE 3: falling velocity
  else if (movedir.falling && !movedir.onGround) {

    camera.Position.y -= camera.WorldUp.y * 5.2f * dtime;  // GRAVITY PULL DOWN CALC: static value, todo: make dynamic based on falling time
    
    if (camera.Position.y <= camera.camstart[1]) {
      movedir.onGround = true;
      movedir.falling = false;
      playlandingsound();
    }
  }


}