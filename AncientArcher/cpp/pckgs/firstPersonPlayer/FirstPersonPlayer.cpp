#include "FirstPersonPlayer.h"
#include <Controls.h>
#include <Camera.h>
#include <Sound.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <TextureLoader.h>

extern Controls g_controls;    // from display.cpp (engine)
extern Camera g_camera;        // from game.cpp    (game)

/**
 *  default constructor
 */
FirstPersonPlayer::FirstPersonPlayer()
{
  init();
}

/**
 *  constructor with custom leg_power value
 *  @param[in] leg_power  sets to 0 to 100 rating of player legpower
 */
FirstPersonPlayer::FirstPersonPlayer(float leg_power)
{
  init();
  legPower = leg_power;
}

/**
 * Main update function. Checks keyboard interaction and player move statuses to move accordingly.
 * Does not check collision.
 * @param[in] deltaTime  for calculation movement distances.
 */
void FirstPersonPlayer::update(float deltaTime)
{
  // KEYBOARD
  g_controls.fppKeyboardIn(this);

  // PRE-MOVEMENT 
  float velocity;
  moves.positionChanged = true;
  model.get()->getFirstEntity()->syncLocation();

  // IF NOT ON GROUND
  if (!moves.onGround)
  {
    if (moves.falling)
    {
      model.get()->getFirstEntity()->moveBy(model->getFirstEntity()->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.back, moves.jumped, moves.falling, moves.left, moves.right));
    }
    else // not on ground, not falling, must be flying or rising or floating
    {
      // a ghetto 1 second jumper
      static auto risingTime = 0.f;
      risingTime += deltaTime;
      if (risingTime > 1.0f)
      {
        risingTime = 0.f;
        moves.falling = true;
      }
      else
      {
        model.get()->getFirstEntity()->moveBy(glm::vec3(0, getRisingSpeed() * deltaTime, 0));
      }
      // end ghetto 1 second jumper
    }
  }
  else // moves.onGround
  {
    // --- FIGURE OUT SPEED --- // 
    velocity = getRunSpeed();                         // get player's statebased movement
    if (moves.forward && (moves.left || moves.right))
    {
      velocity = getRunSpeed() / 2;                   // LEFT+FORWARD OR RIGHT+FORWARD = HALF SPEED
    }
    if (moves.boost && moves.forward)
    {
      velocity *= 2.0;                                // BOOSTING?
    }
    velocity *= deltaTime;                            // Final Result calculated with deltaTime.


    // --- SEND DIRECTION --- //


    // --- FOWARD / BACKWARDS --- //
    if (moves.back || moves.forward) {
      // locks moving foward and backwards to the x and z axii, if y is added in this is become a flyer 
      glm::vec3 moveFront = glm::vec3((float)g_camera.getFront()->x, 0.0f, (float)g_camera.getFront()->z); //get looking direction from the cam;
      if (moves.forward)     model.get()->getFirstEntity()->moveBy(moveFront * velocity);
      else if (moves.back)   model.get()->getFirstEntity()->moveBy(-moveFront * velocity);
    }
    // --- LEFT / RIGHT STRAFING --- // 
    if (moves.right) model.get()->getFirstEntity()->moveBy(*g_camera.getRight() * velocity);
    else if (moves.left) model.get()->getFirstEntity()->moveBy(-(*g_camera.getRight()) * velocity);



    //// ------------ JUMP SYSTEM -------------- //
    //// * 3 phase system

    //// PHASE 1: Liftoff
    //// Liftoff is where the jump is triggered and sounds/animations are played.
    //if (movedir.jumped) { // if a jump is triggered
    //  movedir.onGround = false; // the player leaves the ground
    //  movedir.jumped = false; // the jump is untriggered
    //  //liftoff animation here
    //  playgruntsound();
    //}

    //// PHASE 2: Arc
    //// Rising and falling are now combined into one function which is
    //// just a quadratic equation based on the length of the jump action
    //else if (!movedir.onGround) { // && !movedir.falling) { 
    //  jumpTimer += deltaTime; // Elapsed time of the player being 'in air'
    //  float jumpMod = 0.5f / BASE_PLAYER_WEIGHT; // Modifier based on the user weight in 'kg' (probably needs rework)
    //  float jumpPos = 8.0f; // Initial Y intercept of jump, I think.. not sure why its 8.0f ('c' term in equation below)
    //  float jumpVel = getRisingSpeed() * jumpTimer; // Velocity of jump ('bt' term in equation below) // RISING SPEED CALC: jump speed based on LegPower Player Stat
    //  float jumpAccel = -32.1522f * pow(jumpTimer, 2.f); // Accelaration of jump due top gravity in 'feet' (at^2 term in equation below)
    //              // modifier * (	  c	   +	bt	 +	 at^2	) 
    //  playerIntendedLocation.y += jumpMod * (jumpPos + jumpVel + jumpAccel); // Parabolic equation based on time
    //  //std::cout << previousPlayerLocation.y - playerIntendedLocation.y << "\n";
    //  //if (playerIntendedLocation.y > getJumpHeight() + movedir.lastOnGroundHeight) // MAX HEIGHT CALC: jump height based on LegPower Player Stat
    //  if (previousPlayerLocation.y - playerIntendedLocation.y > 0) {
    //    movedir.falling = true;
    //    // falling animation here
    //  }
    //  else if (previousPlayerLocation.y - playerIntendedLocation.y < 0) {
    //    // rising animation here
    //  }
  }

  //// PHASE 3: Landing
  //// Landing will now just be triggered by the general collision
  //// checking but it may make sense to put animations or sounds
  //// here for landing 
  //if (movedir.positionChanged) { // If the player moves
  //  for (auto const& e : *entities) { // For every entity
  //    bool didCollide = checkBoundCollisionWithEntity(e, collider, playerIntendedLocation); // check if the player collided
  //    if (didCollide) { // if it did collide
  //      if (!movedir.onGround) { // if player is off the ground
  //        playerIntendedLocation = *camera.getPosition(); // move the collision checker to the camera position
  //        movedir.falling = true; // player is falling
  //      }
  //      else { // if player is on the ground
  //        movedir.positionChanged = false; // the position doesn't change
  //      }
  //      float ytop = e.collider->impasse.loc[1] + e.collider->impasse.sz[1] / 2; // top of the entity
  //      if (playerIntendedLocation.y > ytop) { // if the player Y locations is higher than the top of the entity
  //        movedir.falling = false; // player isn't falling
  //        movedir.onGround = true; // player is on groung
  //        jumpTimer = 0.0f; // reset the jump timer
  //        movedir.lastOnGroundHeight = camera.getPosition()->y; // the last ground height is the current position
  //        // landing animation here
  //        playlandingsound();
  //      }
  //    }
  //  }
  //}
}

/**
 * Run at the end of the loop before syncing cam an rendering.
 * @param[in] entities  Pointer to a vector of entities to check the player box against.
 */
void FirstPersonPlayer::finalCollisionCheck(const std::vector<Entity>* entities)
{
  if (entities->empty())
  {
    std::cout << "finalCollisionCheck entities empty! returning...\n";
    return;
  }

  std::cout << "playerloc: "
    << model.get()->getFirstEntity()->gameItem.loc.x << ","
    << model.get()->getFirstEntity()->gameItem.loc.y << ","
    << model.get()->getFirstEntity()->gameItem.loc.z << "\n";

  // DEMO REVISION 2
  for (auto const& e : *entities) {

    //std::cout << "entityloc: " << e.gameItem.loc.x << "," << e.gameItem.loc.y << "," << e.gameItem.loc.z
    //  << " e prevLoc: : " << e.gameItem.prevLoc.x << "," << e.gameItem.prevLoc.y << "," << e.gameItem.prevLoc.z << "\n";


    bool didCollide = cHandler.get()->AABB_vs_AABB_3D(e.collider->impasse, model.get()->getFirstEntity()->collider->impasse);

    if (didCollide) {
      if (!moves.onGround && moves.falling)
      {
        moves.onGround = true;
        moves.falling = false;
        std::cout << "Ran into something while falling, setting moves.onGround to TRUE, moves.falling to FALSE\n";
        // set y pos to prev position
        model.get()->getFirstEntity()->moveTo(glm::vec3(
          model.get()->getFirstEntity()->gameItem.loc.x,
          model.get()->getFirstEntity()->gameItem.prevLoc.y,
          model.get()->getFirstEntity()->gameItem.loc.z)
        );
      }
      else if (!moves.onGround && !moves.falling)
      {
        moves.falling = true;
        std::cout << "Ran into something while rising, setting move.falling to TRUE\n";
      }
      else // onGround
      {
        if (moves.forward) { moves.forward = false; }
        if (moves.back) { moves.back = false; }
        if (moves.left) { moves.left = false; }
        if (moves.right) { moves.right = false; }
        std::cout << "Ran into something while on the ground, setting directional movement to FALSE\n";
        // set x and z loc to prev loc
        model.get()->getFirstEntity()->moveTo(glm::vec3(
          model.get()->getFirstEntity()->gameItem.prevLoc.x,
          model.get()->getFirstEntity()->gameItem.loc.y,
          model.get()->getFirstEntity()->gameItem.prevLoc.z)
        );
      }
    }
  }
}

/**
 *  Move the cam based on offset to near the player model.
 */
void FirstPersonPlayer::syncCam()
{
  g_camera.setPosition(model.get()->getFirstEntity()->gameItem.loc + _camOffset);
}

/**
 * Moves the first point light to the newpos.
 * @param[in] newpos  New position to move the light to.
 * @param[in] shader  Shader to send the light info to.
 */
void FirstPersonPlayer::movePlayerLight(glm::vec3 newpos, Shader* shader)
{
  light->movePointLight(0, newpos, shader);
}

/**
 * Renders the player model.
 */
void FirstPersonPlayer::render() const
{
  if (model)
  {
    model.get()->render();
  }
}

/**
 * Returns the calculated Unboosted Movement Speed of the player. Based on player stats.
 */
float FirstPersonPlayer::getRunSpeed() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_SPEED;
}
/**
 * Returns the calculated Rising Speed of the player. Based on player stats.
 */
float FirstPersonPlayer::getRisingSpeed() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_JUMP_SPEED;
}
/**
 * Returns the calculated Jump Height of the player. Based on player stats.
 */
float FirstPersonPlayer::getJumpHeight() const {
  return (legPower / STAT_DIVISOR) + BASE_PLAYER_JUMP_HEIGHT;
}

/**
 * Increase player stat legpower by amount to add. Caps at 100.
 * @param[in] add  Amount to add to the player legPower.
 */
void FirstPersonPlayer::increaseLegPower(float add) {
  if (legPower < 100.00f) {
    legPower += add;
    if (legPower > 100.00f) {
      legPower = 100.00f;
    }
  }
}

/**
 * Adds a point light at the player location specified.
 * @param[in] pos     Position for the light to start at.
 * @param[in] shader  Which shader to send the light info to.
 */
void FirstPersonPlayer::addPointLight(glm::vec3 pos, Shader* shader)
{
  light->addPointLight(pos, shader);
}

/**
 * Common init ran from the constructors.
 */
void FirstPersonPlayer::init()
{
  model = std::make_unique<PrimativeRenderer>();

  TextureLoader tLoader;
  unsigned int texID = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/firstPersonPlayer/foot.png");

  Entity e(
    ENTITYTYPE::CUBE,
    glm::vec3(0, 1, 0),
    glm::vec3(.008f, 2.0f, .008f),
    texID,
    true,
    true
  );
  model.get()->addToPrimativeEntities(e);

  light = std::make_unique<Lighting>();
  light->setConstantLight(model.get()->getShader());

  cHandler = std::make_unique<CollisionHandler>();

  g_camera.setPosition(model.get()->getFirstEntity()->gameItem.loc + _camOffset);

  legPower = 10.f;
  jumpTimer = 0.0f;

}
