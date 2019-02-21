#include "Collision.h"
#include "../globals.h"
#include "../player/Player.h"
#include "../controls/Controls.h" // for movedir
#include "../sound/Sound.h"
#include <iostream>

extern Movement movedir;

Collision::Collision()
{
}


Collision::~Collision()
{
}

void Collision::process(Player *player, float dtime) {

  glm::vec3 playerIntendedLocation = camera.Position;
  float velocity;

  if (movedir.back || movedir.forward || movedir.left || movedir.right || movedir.jumped || !movedir.onGround) {

    velocity = player->getRunSpeed() * dtime;  // MOVEMENT SPEED CALC : based on player stats

    if (movedir.forward) {  // half speed if moving left or right while forward
      if (movedir.left || movedir.right) {
        velocity = player->getRunSpeed() / 2 * dtime;
      }
    }

    if (movedir.boost && movedir.forward) {  // boost while moving forward
      velocity *= 2.0;  // velocity power
    }

    // locks moving foward and backwards to the x and z axii. 
    // Note: you can use the camera.Front instead of movefront to do a fly type thing 
    // while the Y is unlocked or you are jumping
    if (movedir.back || movedir.forward) {
      glm::vec3 moveFront = { camera.Front.x, 0.0f, camera.Front.z };
      if (movedir.forward) playerIntendedLocation += moveFront * velocity;
      if (movedir.back) playerIntendedLocation -= moveFront * velocity;
    }
    if (movedir.right) playerIntendedLocation += camera.Right * velocity;
    if (movedir.left) playerIntendedLocation -= camera.Right * velocity;

    /* Jump System */
    // PHASE 1: frame of liftoff
    if (movedir.jumped) {
      movedir.onGround = false;
      movedir.jumped = false;
      playgruntsound();
    }
    // PHASE 2: rising velocity
    else if (!movedir.onGround && !movedir.falling) {
      playerIntendedLocation.y += camera.WorldUp.y * player->getRisingSpeed() * dtime; // RISING SPEED CALC: jump speed based on LegPower Player Stat
      if (playerIntendedLocation.y > player->getJumpHeight() + camera.camstart[1]) { // MAX HEIGHT CALC: jump height based on LegPower Player Stat
        movedir.falling = true;
      }
    }
    // PHASE 3: falling velocity
    else if (movedir.falling && !movedir.onGround) {
      playerIntendedLocation.y += GRAVITY * dtime;  // GRAVITY PULL DOWN CALC: static value, todo: make dynamic based on falling time
      if (playerIntendedLocation.y <= camera.camstart[1]) {
        movedir.onGround = true;
        movedir.falling = false;
        playlandingsound();
      }
    }
    movedir.positionChanged = true; 
  }

  /* stop player from walking through impassable entities */
  for (auto const & e : entities) {   // const by reference - entities should not be modified here
    if (e.collider != nullptr &&                                                           //collider is not null (potentially a blocker)
      abs(e.collider->impasse.location[0] - playerIntendedLocation.x) < (world_width / 2) + 1 &&  // and is close enough to be worth checking
      abs(e.collider->impasse.location[1] - playerIntendedLocation.y) < (world_width / 4) + 1 &&
      abs(e.collider->impasse.location[2] - playerIntendedLocation.z) < (world_width / 2) + 1)
    {
      float yTop = e.collider->impasse.location[1] + e.collider->impasse.size[1] / 2;
      float yBot = e.collider->impasse.location[1] - e.collider->impasse.size[1] / 2;

      if (playerIntendedLocation.y < yTop && playerIntendedLocation.y > yBot) {
        float xPosOverlapLT = e.collider->impasse.location[0] + e.collider->impasse.size[0] / 2;
        float xPosOverlapGT = e.collider->impasse.location[0] - e.collider->impasse.size[0] / 2;

        if (playerIntendedLocation.x < xPosOverlapLT && playerIntendedLocation.x > xPosOverlapGT) {
          float yPosOverlapLT = e.collider->impasse.location[2] + e.collider->impasse.size[2] / 2;
          float yPosOverlapGT = e.collider->impasse.location[2] - e.collider->impasse.size[2] / 2;

          if (playerIntendedLocation.z < yPosOverlapLT && playerIntendedLocation.z > yPosOverlapGT) {
            movedir.positionChanged = false;
            if (playerIntendedLocation.y+0.5f > yTop) {
              movedir.onGround = true;
              camera.Position.y += 0.5f;
            }
          }
        }
      }
    }
  }

  if (movedir.positionChanged) camera.Position = playerIntendedLocation;

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
  }
}
