#include "FirstPersonPlayer.h"
#include <Controls.h>
extern Controls g_controls;    // from display.cpp (engine)

void FirstPersonPlayer::updateFromKeypresses()
{
  if (_keypress->w) {
    moves.forward = true;
  }
  if (_keypress->s) {
    moves.back = true;
  }
  if (_keypress->a) {
    moves.left = true;
  }
  if (_keypress->d) {
    moves.right = true;
  }
}

/**
 * Common init ran from the constructors.
 */
void FirstPersonPlayer::init()
{
  legPower = 10.f;
  jumpTimer = 0.0f;
  moves.falling = true;
  moves.onGround = false;
}

FirstPersonPlayer::FirstPersonPlayer(std::shared_ptr<Camera>& cam, std::shared_ptr<Shader>& shader,
  std::shared_ptr<keys>& kb)
{
  _playerCamera = cam;
  _playerShader = shader;
  _keypress = kb;

  init();

  //debug
  //std::cout << "cam use count in player init: " << _playerCamera.use_count() << std::endl;
  //std::cout << "shader use count in player init: " << _playerShader.use_count() << std::endl;
  // -- ok
}

/**
 * Main update function. Checks keyboard interaction and player move statuses to move accordingly.
 * Does not check collision.
 * @param[in] deltaTime  for calculation movement distances.
 */
void FirstPersonPlayer::update()
{
  
}

/**
 * Run at the end of the loop before syncing cam an rendering.
 * @param[in] entities  Pointer to a vector of entities to check the player box against.
 */
void FirstPersonPlayer::finalCollisionCheck(const std::vector<Entity>* entities)
{
  static bool needsToFall;

  if (entities->empty())
  {
    needsToFall = true;
    return;
  }
}

/**
 *  Move the cam based on offset to near the player model.
 */
void FirstPersonPlayer::syncCam()
{
  //_cam->setPosition(model->getFirstEntity()->gameItem.loc + _camOffset);
  //g_camera.setPosition(model->getFirstEntity()->gameItem.loc + _camOffset);
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



// ----> old first person controls
//void Player::processCommands(float deltaTime)
//{
//  glm::vec3 playerIntendedLocation = camera.Position;
//  float velocity;
//  movedir.positionChanged = true;
//
//  velocity = getRunSpeed() * deltaTime;  // MOVEMENT SPEED CALC : based on player stats
//  if (movedir.forward) {  // half speed if moving left or right while forward
//    if (movedir.left || movedir.right) {
//      velocity = getRunSpeed() / 2 * deltaTime;
//    }
//  }
//
//  if (movedir.boost && movedir.forward) {  // boost while moving forward
//    velocity *= 2.0;  // velocity power
//  }
//
//  // locks moving foward and backwards to the x and z axii. 
//  // Note: you can use the camera.Front instead of movefront to do a fly type thing 
//  // while the Y is unlocked or you are jumping
//  if (movedir.back || movedir.forward) {
//    glm::vec3 moveFront = { camera.Front.x, 0.0f, camera.Front.z };
//    if (movedir.forward) playerIntendedLocation += moveFront * velocity;
//    if (movedir.back) playerIntendedLocation -= moveFront * velocity;
//  }
//  if (movedir.right) playerIntendedLocation += camera.Right * velocity;
//  if (movedir.left) playerIntendedLocation -= camera.Right * velocity;
//
//  /* Jump System */
//  // PHASE 1: frame of liftoff
//  if (movedir.jumped) {
//    movedir.onGround = false;
//    movedir.jumped = false;
//    playgruntsound();
//  }
//  // PHASE 2: rising velocity
//  else if (!movedir.onGround && !movedir.falling) {
//    playerIntendedLocation.y += camera.WorldUp.y * getRisingSpeed() * deltaTime;   // RISING SPEED CALC: jump speed based on LegPower Player Stat
//    if (playerIntendedLocation.y > getJumpHeight() + movedir.lastOnGroundHeight) { // MAX HEIGHT CALC: jump height based on LegPower Player Stat
//      movedir.falling = true;
//    }
//  }
//  // PHASE 3: falling velocity
//  else if (movedir.falling && !movedir.onGround) {
//    playerIntendedLocation.y += GRAVITY * deltaTime;
//    //playerIntendedLocation.y += (GRAVITY * accumulated_delta_time < TERMINAL_VELOCITY) ? GRAVITY * accumulated_delta_time : TERMINAL_VELOCITY;
//  }
//
//  /* stop player from walking through impassable entities */
//  /*  - entities should not be modified here only checking and determining plafyer location */
//  if (movedir.positionChanged) {        // only do this check if the player actually moved
//    for (auto const& e : entities) {   // const by reference
//      if (e.collider != nullptr &&      //collider is not null (potentially a blocker)
//        abs(e.collider->impasse.loc[0] - playerIntendedLocation.x) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1 &&
//        abs(e.collider->impasse.loc[1] - playerIntendedLocation.y) < (ENGINE_LOGIC_CHECKING_DISTANCE / 4) + 1 &&
//        abs(e.collider->impasse.loc[2] - playerIntendedLocation.z) < (ENGINE_LOGIC_CHECKING_DISTANCE / 1.5) + 1) {   //close enough to be worth checking
//        float yTop = e.collider->impasse.loc[1] + e.collider->impasse.sz[1] / 2;
//        float yBot = e.collider->impasse.loc[1] - e.collider->impasse.sz[1] / 2;
//        if (playerIntendedLocation.y < yTop && playerIntendedLocation.y > yBot) {  // inbetween the y
//          float xPosOverlapLT = e.collider->impasse.loc[0] + e.collider->impasse.sz[0] / 2;
//          float xPosOverlapGT = e.collider->impasse.loc[0] - e.collider->impasse.sz[0] / 2;
//          if (playerIntendedLocation.x < xPosOverlapLT && playerIntendedLocation.x > xPosOverlapGT) {  // inbetween the x & y
//            float yPosOverlapLT = e.collider->impasse.loc[2] + e.collider->impasse.sz[2] / 2;
//            float yPosOverlapGT = e.collider->impasse.loc[2] - e.collider->impasse.sz[2] / 2;
//            if (playerIntendedLocation.z < yPosOverlapLT && playerIntendedLocation.z > yPosOverlapGT) {  // in between the x & y & z
//
//              if (!movedir.onGround) {
//                /* just x and z */
//                //playerIntendedLocation.x = camera.Position.x;
//                //playerIntendedLocation.z = camera.Position.z;
//
//                /* all 3 */
//                playerIntendedLocation = camera.Position;
//                movedir.falling = true;
//              }
//              else {
//                movedir.positionChanged = false;
//              }
//
//              if (playerIntendedLocation.y > yTop) {
//                movedir.falling = false;
//                movedir.onGround = true;
//                movedir.lastOnGroundHeight = camera.Position.y;
//                playlandingsound();
//              }
//            }
//          }
//        }
//      }
//    }
//  }
//
//  if (movedir.positionChanged) {
//    camera.Position = playerIntendedLocation;
//    lighting.movePointLight(0, playerIntendedLocation);
//    playerEntity->moveTo(glm::vec3(playerIntendedLocation.x, playerIntendedLocation.y - .2f, playerIntendedLocation.z));
//
//    if (movedir.onGround) {
//      static const float TimeBetweenFootsteps = 0.6f;
//      static const float TimeBetweenFootstepsRunning = 0.4f;
//      static float accumulatedTime = 0.f;
//      accumulatedTime += deltaTime;
//      if (movedir.isBoosted() && accumulatedTime > TimeBetweenFootstepsRunning) {
//        playfootstepsound();
//        accumulatedTime = 0.f;
//      }
//      else if (accumulatedTime > TimeBetweenFootsteps) {
//        playfootstepsound();
//        accumulatedTime = 0.f;
//      }
//    }
//  }
//}