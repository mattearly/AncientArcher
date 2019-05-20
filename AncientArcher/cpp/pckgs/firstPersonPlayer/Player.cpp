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

  previousPlayerLocation = playerIntendedLocation;
  playerIntendedLocation = *camera.getPosition();

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

  // PHASE 1: Liftoff
  // Liftoff is where the jump is triggered and sounds/animations are played.
  if (movedir.jumped) { // if a jump is triggered
    movedir.onGround = false; // the player leaves the ground
    movedir.jumped = false; // the jump is untriggered
    //liftoff animation here
    playgruntsound();
  }

  // PHASE 2: Arc
  // Rising and falling are now combined into one function which is
  // just a quadratic equation based on the length of the jump action
  else if (!movedir.onGround) { // && !movedir.falling) { 
    jumpTimer += deltaTime; // Elapsed time of the player being 'in air'
    float jumpMod = 0.5f / BASE_PLAYER_WEIGHT; // Modifier based on the user weight in 'kg' (probably needs rework)
    float jumpPos = 8.0f; // Initial Y intercept of jump, I think.. not sure why its 8.0f ('c' term in equation below)
    float jumpVel = getRisingSpeed() * jumpTimer; // Velocity of jump ('bt' term in equation below) // RISING SPEED CALC: jump speed based on LegPower Player Stat
    float jumpAccel = -32.1522f * pow(jumpTimer, 2.f); // Accelaration of jump due top gravity in 'feet' (at^2 term in equation below)
                // modifier * (	  c	   +	bt	 +	 at^2	) 
    playerIntendedLocation.y += jumpMod * (jumpPos + jumpVel + jumpAccel); // Parabolic equation based on time
    //std::cout << previousPlayerLocation.y - playerIntendedLocation.y << "\n";
    //if (playerIntendedLocation.y > getJumpHeight() + movedir.lastOnGroundHeight) // MAX HEIGHT CALC: jump height based on LegPower Player Stat
    if (previousPlayerLocation.y - playerIntendedLocation.y > 0) {
      movedir.falling = true;
      // falling animation here
    }
    else if (previousPlayerLocation.y - playerIntendedLocation.y < 0) {
      // rising animation here
    }
  }

  // PHASE 3: Landing
  // Landing will now just be triggered by the general collision
  // checking but it may make sense to put animations or sounds
  // here for landing 
  if (movedir.positionChanged) { // If the player moves
    for (auto const& e : *entities) { // For every entity
      bool didCollide = checkBoundCollisionWithEntity(e, collider, playerIntendedLocation); // check if the player collided
      if (didCollide) { // if it did collide
        if (!movedir.onGround) { // if player is off the ground
          playerIntendedLocation = *camera.getPosition(); // move the collision checker to the camera position
          movedir.falling = true; // player is falling
        }
        else { // if player is on the ground
          movedir.positionChanged = false; // the position doesn't change
        }
        float ytop = e.collider->impasse.loc[1] + e.collider->impasse.sz[1] / 2; // top of the entity
        if (playerIntendedLocation.y > ytop) { // if the player Y locations is higher than the top of the entity
          movedir.falling = false; // player isn't falling
          movedir.onGround = true; // player is on groung
          jumpTimer = 0.0f; // reset the jump timer
          movedir.lastOnGroundHeight = camera.getPosition()->y; // the last ground height is the current position
          // landing animation here
          playlandingsound();
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

/**
 * Checks for collision between the player and an Entity object
 */
bool Player::checkBoundCollisionWithEntity(Entity e, Collider* playerCollider, glm::vec3 playerPosition)
{

  // ------------ GENERAL MOVEMENT COLLISION -------------- //
  // * A point vs Boundry Boxes (any entity with collision on)

  if (e.collider != nullptr && // isn't null the check if its close enough to be worth checking
    abs(e.collider->impasse.loc[0] - playerPosition.x) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1 && // X direction
    abs(e.collider->impasse.loc[1] - playerPosition.y) < (ENGINE_LOGIC_CHECKING_DISTANCE / 4) + 1 && // Y direction
    abs(e.collider->impasse.loc[2] - playerPosition.z) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1) { // Z direction

    bool yOverlap = false;
    bool xOverlap = false;
    bool zOverlap = false;

    float ytop = e.collider->impasse.loc[1] + e.collider->impasse.sz[1] / 2; // set the top of the object in the Y direction
    float ybot = e.collider->impasse.loc[1] - e.collider->impasse.sz[1] / 2; // set the bottom of the object in the Y direction
    float xposoverlaplt = e.collider->impasse.loc[0] + e.collider->impasse.sz[0] / 2; // set X lower bound
    float xposoverlapgt = e.collider->impasse.loc[0] - e.collider->impasse.sz[0] / 2; // set X upper bound
    float yposoverlaplt = e.collider->impasse.loc[2] + e.collider->impasse.sz[2] / 2; // set Z lower bound
    float yposoverlapgt = e.collider->impasse.loc[2] - e.collider->impasse.sz[2] / 2; // set Z upper bound

    float ptop = playerPosition.y + playerCollider->impasse.sz[1] / 2; // set the top of the player in the Y direction
    float pbot = playerPosition.y - playerCollider->impasse.sz[1] / 2; // set the bottom of the player in the Y direction
    float plft = playerPosition.x + playerCollider->impasse.sz[0] / 2; // set the left of the player in the X direction
    float prgt = playerPosition.x - playerCollider->impasse.sz[0] / 2; // set the right of the player in the X direction
    float pbck = playerPosition.z + playerCollider->impasse.sz[2] / 2; // set the back of the player in the Z direction
    float pfrn = playerPosition.z - playerCollider->impasse.sz[2] / 2; // set the front of the player in the Z direction

    if (ptop < ytop && ptop > ybot || pbot < ytop && pbot > ybot) {  // if the players top is inbetween the Y top and bottom of the entity
      yOverlap = true;
    }

    if (plft < xposoverlaplt && plft > xposoverlapgt || prgt < xposoverlaplt && prgt > xposoverlapgt) {  // inbetween the X & Y bounds
      xOverlap = true;
    }

    if (pbck < yposoverlaplt && pbck > yposoverlapgt || pfrn < yposoverlaplt && pfrn > yposoverlapgt) {  // inbetween the X & Y & Z bounds
      zOverlap = true;
    }

    if (yOverlap && xOverlap && zOverlap) { // collision
      return true;
    }

  }

  return false;

}

/**
 * Checks for collision between the player and an Entity object
 */
bool Player::checkPointCollisionWithEntity(Entity e, glm::vec3 playerPosition)
{

  // ------------ GENERAL MOVEMENT COLLISION -------------- //
  // * A point vs Boundry Boxes (any entity with collision on)

  if (e.collider != nullptr && // isn't null the check if its close enough to be worth checking
    abs(e.collider->impasse.loc[0] - playerPosition.x) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1 && // X direction
    abs(e.collider->impasse.loc[1] - playerPosition.y) < (ENGINE_LOGIC_CHECKING_DISTANCE / 4) + 1 && // Y direction
    abs(e.collider->impasse.loc[2] - playerPosition.z) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1) { // Z direction

    float ytop = e.collider->impasse.loc[1] + e.collider->impasse.sz[1] / 2; // set the top of the object in the Y direction
    float ybot = e.collider->impasse.loc[1] - e.collider->impasse.sz[1] / 2; // set the bottom of the object in the Y direction

    if (playerPosition.y < ytop && playerPosition.y > ybot) {  // if the player Y location is inbetween the Y top and bottom of the entity

      float xposoverlaplt = e.collider->impasse.loc[0] + e.collider->impasse.sz[0] / 2; // set X lower bound
      float xposoverlapgt = e.collider->impasse.loc[0] - e.collider->impasse.sz[0] / 2; // set X upper bound

      if (playerPosition.x < xposoverlaplt && playerPosition.x > xposoverlapgt) {  // inbetween the X & Y bounds

        float yposoverlaplt = e.collider->impasse.loc[2] + e.collider->impasse.sz[2] / 2; // set Z lower bound
        float yposoverlapgt = e.collider->impasse.loc[2] - e.collider->impasse.sz[2] / 2; // set Z upper bound

        if (playerPosition.z < yposoverlaplt && playerPosition.z > yposoverlapgt) {  // inbetween the X & Y & Z boundss

          return true; // collision

        }
      }
    }
  }

  return false;
}

/**
 * Adds a point light at the player location specified.
 */
void Player::addPointLight(glm::vec3 pos, Shader* shader)
{
  playerLight.addPointLight(pos, shader);
}

/**
 * Moves the first point light to the newpos.
 */
void Player::movePlayerLight(glm::vec3 newpos, Shader* shader)
{
  playerLight.movePointLight(0, newpos, shader);
}

void Player::increaseLegPower(float add) {
  if (legPower < 100.00f) {
    legPower += add;
    if (legPower > 100.00f) {
      legPower = 100.00f;
    }
  }
}

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

  collider = new Collider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 3.0f, 0.5f));

  previousPlayerLocation = glm::vec3(0.0, 0.0, 0.0);
  playerIntendedLocation = glm::vec3(0.0, 0.0, 0.0);

  //playerEntity = new Entity(
  //	BOX, glm::vec3(1.5f, 0.26f, 1.5f), glm::vec3(0.05f, 0.1f, 0.05f), -1, true
  //);

  //lighting.addPointLight(*camera.getPosition());
  //camera.updateProjectionMatrix();

  legPower = 10.0f;
  jumpTimer = 0.0f;



}

Player::Player(float leg_power) {

  collider = new Collider(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 3.0f, 0.5f));

  previousPlayerLocation = glm::vec3(0.0, 0.0, 0.0);
  playerIntendedLocation = glm::vec3(0.0, 0.0, 0.0);

  //playerEntity = new Entity(
  //  BOX, glm::vec3(1.5f, 0.26f, 1.5f), glm::vec3(0.05f, 0.1f, 0.05f), -1, true
  //);

  //lighting.addPointLight(camera.Position);
  //camera.updateProjectionMatrix();

  legPower = leg_power;
  jumpTimer = 0.0f;


}
