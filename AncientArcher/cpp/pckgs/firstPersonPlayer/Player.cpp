#include <AAEngine.h>
#include <Controls.h>
#include "Movement.h"
#include "Player.h"
#include <glm/glm.hpp>
#include <iostream>

extern Controls controls;

extern Camera camera;

Movement movedir;

void Player::update(float deltaTime)
{
  // process keys
  controls.playerKeyboardInput();



  // move the camera accordingly
  //camera.update();

}

void Player::processCommands(float deltaTime, std::vector<Entity>* entities)
{

  glm::vec3 playerIntendedLocation = *camera.getPosition();
  float velocity;
  movedir.positionChanged = true;

  velocity = getRunSpeed() * deltaTime;  // MOVEMENT SPEED CALC : based on player stats
  if (movedir.forward) {  // half speed if moving left or right while forward
    if (movedir.left || movedir.right) {
      velocity = getRunSpeed() / 2 * deltaTime;
    }
  }

  if (movedir.boost && movedir.forward) {  // boost while moving forward
    velocity *= 2.0;  // velocity power
  }

  // locks moving foward and backwards to the x and z axii. 
  // Note: you can use the camera.Front instead of movefront to do a fly type thing 
  // while the Y is unlocked or you are jumping
  if (movedir.back || movedir.forward) {
    glm::vec3 moveFront = { (float)camera.getFront()->x, 0.0f, (float)camera.getFront()->z };
    if (movedir.forward) playerIntendedLocation += moveFront * velocity;
    if (movedir.back) playerIntendedLocation -= moveFront * velocity;
  }
  if (movedir.right) playerIntendedLocation += *camera.getRight() * velocity;
  if (movedir.left) playerIntendedLocation -= *camera.getRight() * velocity;

  // ------------ JUMP SYSTEM -------------- //
  // * 3 phase system

  // PHASE 1: frame of liftoff
  if (movedir.jumped) {
    movedir.onGround = false;
    movedir.jumped = false;
    playgruntsound();
  }
  // PHASE 2: rising velocity
  else if (!movedir.onGround) { // && !movedir.falling) {

	float jumpMult = 0.025f;

	jumpTimer += (deltaTime * 85.0f * jumpMult);

	//std::cout << "Delta: " << deltaTime << " Jump: " << jumpTimer << "\n";
	//std::cout << jumpMult * 1.0f << " + " << jumpMult * getRisingSpeed() * jumpTimer << " + " << jumpMult * -32.1522 * pow(jumpTimer, 2.0) << " = " << (camera.getWorldUp()->y + getRisingSpeed() * jumpTimer + -0.98 * pow(jumpTimer, 2.0)) << "\n";

    playerIntendedLocation.y += jumpMult * (1.0f + jumpMult * getRisingSpeed() * jumpTimer + jumpMult * -32.21522 * pow(jumpTimer, 2.0) );   // RISING SPEED CALC: jump speed based on LegPower Player Stat
    //if (playerIntendedLocation.y > getJumpHeight() + movedir.lastOnGroundHeight) { // MAX HEIGHT CALC: jump height based on LegPower Player Stat
    //  movedir.falling = true;
    //}
  }
  // PHASE 3: falling velocity
  //else if (movedir.falling && !movedir.onGround) {
  //  playerIntendedLocation.y += GRAVITY * deltaTime;
  //}

  // ------------ GENERAL MOVEMENT COLLISION -------------- //
  // * A point vs Boundry Boxes (any entity with collision on)

  if (movedir.positionChanged)
  {
    for (auto const& e : *entities)
    {
      if (e.collider != nullptr &&
        abs(e.collider->impasse.loc[0] - playerIntendedLocation.x) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1 &&
        abs(e.collider->impasse.loc[1] - playerIntendedLocation.y) < (ENGINE_LOGIC_CHECKING_DISTANCE / 4) + 1 &&
        abs(e.collider->impasse.loc[2] - playerIntendedLocation.z) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1) {   //close enough to be worth checking
        float ytop = e.collider->impasse.loc[1] + e.collider->impasse.sz[1] / 2;
        float ybot = e.collider->impasse.loc[1] - e.collider->impasse.sz[1] / 2;
        if (playerIntendedLocation.y < ytop && playerIntendedLocation.y > ybot) {  // inbetween the y
          float xposoverlaplt = e.collider->impasse.loc[0] + e.collider->impasse.sz[0] / 2;
          float xposoverlapgt = e.collider->impasse.loc[0] - e.collider->impasse.sz[0] / 2;
          if (playerIntendedLocation.x < xposoverlaplt && playerIntendedLocation.x > xposoverlapgt) {  // inbetween the x & y
            float yposoverlaplt = e.collider->impasse.loc[2] + e.collider->impasse.sz[2] / 2;
            float yposoverlapgt = e.collider->impasse.loc[2] - e.collider->impasse.sz[2] / 2;
            if (playerIntendedLocation.z < yposoverlaplt && playerIntendedLocation.z > yposoverlapgt) {  // in between the x & y & z
              if (!movedir.onGround)
              {
                playerIntendedLocation = *camera.getPosition();
                // movedir.falling = true;
              }
              else
              {
                movedir.positionChanged = false;
              }

              if (playerIntendedLocation.y > ytop)
              {
                // movedir.falling = false;
                movedir.onGround = true;
				jumpTimer = 0.0f;
                movedir.lastOnGroundHeight = camera.getPosition()->y;
                playlandingsound();
              }
            }
          }
        }
      }
    }
  }

  if (movedir.positionChanged) {
    camera.setPosition(playerIntendedLocation);
    //lighting.movePointLight(0, playerIntendedLocation, Shader *shader);
    //playerEntity->moveTo(glm::vec3(playerIntendedLocation.x, playerIntendedLocation.y - .2f, playerIntendedLocation.z));

    if (movedir.onGround) {
      static const float TimeBetweenFootsteps = 0.6f;
      static const float TimeBetweenFootstepsRunning = 0.4f;
      static float accumulatedTime = 0.f;
      accumulatedTime += deltaTime;
      if (movedir.isBoosted() && accumulatedTime > TimeBetweenFootstepsRunning) {
        playfootstepsound();
        accumulatedTime = 0.f;
      }
      else if (accumulatedTime > TimeBetweenFootsteps) {
        playfootstepsound();
        accumulatedTime = 0.f;
      }
    }
  }
}

void Player::increaseLegPower(float add) {
  if (legPower < 100.00f) {
    legPower += add;
    if (legPower > 100.00f) {
      legPower = 100.00f;
    }
  }
}
//
//Entity * Player::getEntity() const
//{
//  return playerEntity;
//}

float Player::getRunSpeed() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_SPEED;
}

float Player::getRisingSpeed() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_JUMP_SPEED;
}

float Player::getJumpHeight() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_JUMP_HEIGHT;
}

Player::Player() {
  /*
    playerEntity = new Entity(
      BOX, glm::vec3(1.5f, 0.26f, 1.5f), glm::vec3(0.05f, 0.1f, 0.05f), -1, true
    );*/

    //lighting.addPointLight(*camera.getPosition());
    //camera.updateProjectionMatrix();

  legPower = 10.0f;
  jumpTimer = 0.0f;

}

Player::Player(float leg_power) {

  //playerEntity = new Entity(
  //  BOX, glm::vec3(1.5f, 0.26f, 1.5f), glm::vec3(0.05f, 0.1f, 0.05f), -1, true
  //);

  //lighting.addPointLight(camera.Position);
  //camera.updateProjectionMatrix();

  legPower = leg_power;
  jumpTimer = 0.0f;

}
