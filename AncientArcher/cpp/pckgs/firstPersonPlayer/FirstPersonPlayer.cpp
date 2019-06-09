#include "FirstPersonPlayer.h"
#include <Controls.h>
#include <Camera.h>
#include <Sound.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

extern Controls g_controls;    // from display.cpp (engine)
extern Camera g_camera;        // from game.cpp    (game)

FirstPersonPlayer::FirstPersonPlayer()
{
  light = std::make_unique<Lighting>();
  legPower = 10.0f;
  jumpTimer = 0.0f;
}

FirstPersonPlayer::FirstPersonPlayer(float leg_power)
{
  legPower = leg_power;
  light = std::make_unique<Lighting>();
  jumpTimer = 0.0f;
}


void FirstPersonPlayer::render() const
{
  if (model)
  {
    //draw
  }
}

/**
 * Adds a point light at the player location specified.
 */
void FirstPersonPlayer::addPointLight(glm::vec3 pos, Shader* shader)
{
  light->addPointLight(pos, shader);
}

/**
 * Moves the first point light to the newpos.
 */
void FirstPersonPlayer::movePlayerLight(glm::vec3 newpos, Shader* shader)
{
  light->movePointLight(0, newpos, shader);
}

void FirstPersonPlayer::increaseLegPower(float add) {
  if (legPower < 100.00f) {
    legPower += add;
    if (legPower > 100.00f) {
      legPower = 100.00f;
    }
  }
}

void FirstPersonPlayer::update(float deltaTime)
{
  g_controls.fppKeyboardIn(this);

  //glm::vec3 playerIntendedLocation = g_camera.Position;
  float velocity;
  moves.positionChanged = true;

  velocity = getRunSpeed() * deltaTime;  // MOVEMENT SPEED CALC : based on player stats
  if (moves.forward) {  // half speed if moving left or right while forward
    if (moves.left || moves.right) {
      velocity = getRunSpeed() / 2 * deltaTime;
    }
  }

  if (moves.boost && moves.forward) {  // boost while moving forward
    velocity *= 2.0;  // velocity power
  }

  // locks moving foward and backwards to the x and z axii. 
  // Note: you can use the camera.Front instead of movefront to do a fly type thing 
  // while the Y is unlocked or you are jumping
  //if (moves.back || moves.forward) {
  //  glm::vec3 moveFront = glm::vec3(g_camera.getFront().x, 0.0f, g_camera.getFront().z);
  //  if (moves.forward) playerIntendedLocation += moveFront * velocity;
  //  if (moves.back) playerIntendedLocation -= moveFront * velocity;
  //}
  //if (moves.right) playerIntendedLocation += g_camera.Right * velocity;
  //if (moves.left) playerIntendedLocation -= g_camera.Right * velocity;

  ///* Jump System */
  //// PHASE 1: frame of liftoff
  //if (moves.jumped) {
  //  moves.onGround = false;
  //  moves.jumped = false;
  //  playgruntsound();
  //}
  //// PHASE 2: rising velocity
  //else if (!moves.onGround && !moves.falling) {
  //  playerIntendedLocation.y += g_camera.WorldUp.y * getRisingSpeed() * deltaTime;   // RISING SPEED CALC: jump speed based on LegPower Player Stat
  //  if (playerIntendedLocation.y > getJumpHeight() + moves.lastOnGroundHeight) { // MAX HEIGHT CALC: jump height based on LegPower Player Stat
  //    moves.falling = true;
  //  }
  //}
  //// PHASE 3: falling velocity
  //else if (moves.falling && !moves.onGround) {
  //  playerIntendedLocation.y += GRAVITY * deltaTime;
  //  //playerIntendedLocation.y += (GRAVITY * accumulated_delta_time < TERMINAL_VELOCITY) ? GRAVITY * accumulated_delta_time : TERMINAL_VELOCITY;
  //}

  ///* stop player from walking through impassable entities */
  ///*  - entities should not be modified here only checking and determining plafyer location */
  //if (moves.positionChanged) {        // only do this check if the player actually moved
  //  for (auto const& e : entities) {   // const by reference
  //    if (e.collider != nullptr &&      //collider is not null (potentially a blocker)
  //      abs(e.collider->impasse.loc[0] - playerIntendedLocation.x) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1 &&
  //      abs(e.collider->impasse.loc[1] - playerIntendedLocation.y) < (ENGINE_LOGIC_CHECKING_DISTANCE / 4) + 1 &&
  //      abs(e.collider->impasse.loc[2] - playerIntendedLocation.z) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1) {   //close enough to be worth checking
  //      float yTop = e.collider->impasse.loc[1] + e.collider->impasse.sz[1] / 2;
  //      float yBot = e.collider->impasse.loc[1] - e.collider->impasse.sz[1] / 2;
  //      if (playerIntendedLocation.y < yTop && playerIntendedLocation.y > yBot) {  // inbetween the y
  //        float xPosOverlapLT = e.collider->impasse.loc[0] + e.collider->impasse.sz[0] / 2;
  //        float xPosOverlapGT = e.collider->impasse.loc[0] - e.collider->impasse.sz[0] / 2;
  //        if (playerIntendedLocation.x < xPosOverlapLT && playerIntendedLocation.x > xPosOverlapGT) {  // inbetween the x & y
  //          float yPosOverlapLT = e.collider->impasse.loc[2] + e.collider->impasse.sz[2] / 2;
  //          float yPosOverlapGT = e.collider->impasse.loc[2] - e.collider->impasse.sz[2] / 2;
  //          if (playerIntendedLocation.z < yPosOverlapLT && playerIntendedLocation.z > yPosOverlapGT) {  // in between the x & y & z

  //            if (!moves.onGround) {
  //              /* just x and z */
  //              //playerIntendedLocation.x = g_camera.Position.x;
  //              //playerIntendedLocation.z = g_camera.Position.z;

  //              /* all 3 */
  //              playerIntendedLocation = g_camera.Position;
  //              moves.falling = true;
  //            }
  //            else {
  //              moves.positionChanged = false;
  //            }

  //            if (playerIntendedLocation.y > yTop) {
  //              moves.falling = false;
  //              moves.onGround = true;
  //              moves.lastOnGroundHeight = g_camera.Position.y;
  //              playlandingsound();
  //            }
  //          }
  //        }
  //      }
  //    }
  //  }

}

float FirstPersonPlayer::getRunSpeed() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_SPEED;
}

float FirstPersonPlayer::getRisingSpeed() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_JUMP_SPEED;
}

float FirstPersonPlayer::getJumpHeight() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_JUMP_HEIGHT;
}

