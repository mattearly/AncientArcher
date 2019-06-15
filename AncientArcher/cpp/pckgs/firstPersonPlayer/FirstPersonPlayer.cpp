#include "FirstPersonPlayer.h"
#include <Controls.h>
#include <Camera.h>
#include <Sound.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <TextureLoader.h>
#include <Global.h>

extern Controls g_controls;    // from display.cpp (engine)
extern Camera g_camera;        // from game.cpp    (game)

/**
 *  Default constructor.
 */
FirstPersonPlayer::FirstPersonPlayer()
{
  init();
}

/**
 *  Constructor with custom leg_power value
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
      if (risingTime > .85f)
      {
        risingTime = 0.f;
        moves.falling = true;
      }
      else
      {
        model->getFirstEntity()->kinematics->vel.x = moves.forward ? moves.currentVelocity : 0.f;
        model->getFirstEntity()->kinematics->vel.y = getRisingSpeed();

        model.get()->getFirstEntity()->moveBy(model->getFirstEntity()->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.back, moves.jumped, moves.falling, moves.left, moves.right));

        if (moves.back || moves.forward)
        {
          // locks moving foward and backwards to the x and z axii, if y is added in this is become a flyer 
          glm::vec3 moveFront = glm::vec3((float)g_camera.getFront()->x, 0.0f, (float)g_camera.getFront()->z); //get looking direction from the cam;
          if (moves.forward)     model.get()->getFirstEntity()->moveBy(moveFront * moves.currentVelocity);
          else if (moves.back)   model.get()->getFirstEntity()->moveBy(-moveFront * moves.currentVelocity);
        }
      }
      // end ghetto 1 second jumper
    }
  }
  else // moves.onGround
  {
    // --- FIGURE OUT SPEED --- // 
    moves.currentVelocity = getRunSpeed();                         // get player's statebased movement
    if (moves.forward && (moves.left || moves.right))
    {
      moves.currentVelocity = getRunSpeed() / 2;                   // LEFT+FORWARD OR RIGHT+FORWARD = HALF SPEED
    }
    if (moves.boost && moves.forward)
    {
      moves.currentVelocity *= 2.0;                                // BOOSTING?
    }
    moves.currentVelocity *= deltaTime;                            // Final Result calculated with deltaTime.


    // --- SEND DIRECTION --- //
    // --- FOWARD / BACKWARDS --- //
    if (moves.back || moves.forward) {
      // locks moving foward and backwards to the x and z axii, if y is added in this is become a flyer 
      glm::vec3 moveFront = glm::vec3((float)g_camera.getFront()->x, 0.0f, (float)g_camera.getFront()->z); //get looking direction from the cam;
      if (moves.forward)     model.get()->getFirstEntity()->moveBy(moveFront * moves.currentVelocity);
      else if (moves.back)   model.get()->getFirstEntity()->moveBy(-moveFront * moves.currentVelocity);
    }
    // --- LEFT / RIGHT STRAFING --- // 
    if (moves.right) model.get()->getFirstEntity()->moveBy(*g_camera.getRight() * moves.currentVelocity);
    else if (moves.left) model.get()->getFirstEntity()->moveBy(-(*g_camera.getRight()) * moves.currentVelocity);
  }
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
    // std::cout << "finalCollisionCheck entities empty! returning...\n";
    needsToFall = true;
    return;
  }

  //std::cout << "playerloc: "
  // << model.get()->getFirstEntity()->gameItem.loc.x << ","
  // << model.get()->getFirstEntity()->gameItem.loc.y << ","
  // << model.get()->getFirstEntity()->gameItem.loc.z << "\n";

  //int inLogicCheckingRangeCount = 0;

  // DEMO REVISION 2
  for (auto const& e : *entities)
  {
    // check distance
    float distance = glm::distance(e.collider->impasse.loc, model.get()->getFirstEntity()->collider->impasse.loc);

    if (distance < ENGINE_LOGIC_CHECKING_DISTANCE)
    {
      //inLogicCheckingRangeCount++;  // for debug purposes
      // std::cout << "entityloc: " << e.gameItem.loc.x << "," << e.gameItem.loc.y << "," << e.gameItem.loc.z
        //<< " e prevLoc: : " << e.gameItem.prevLoc.x << "," << e.gameItem.prevLoc.y << "," << e.gameItem.prevLoc.z << "\n";
      // std::cout << "going through entitites: " << inLogicCheckingRangeCount << "\n";

      bool didCollide = cHandler.get()->AABB_vs_AABB_3D(e.collider->impasse, model.get()->getFirstEntity()->collider->impasse);
      if (didCollide) {
        // std::cout << "A WILD COLLISION!\n";
        if (!moves.onGround && moves.falling)
        {
          moves.onGround = true;
          moves.falling = false;
          // std::cout << "Ran into something while falling, setting moves.onGround to TRUE, moves.falling to FALSE\n";
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
          // std::cout << "Ran into something while rising, setting move.falling to TRUE\n";
        }
        else // ------ON GROUND LOGIC
        {
          needsToFall = true;

          if (moves.forward) { moves.forward = false; }
          if (moves.back) { moves.back = false; }
          if (moves.left) { moves.left = false; }
          if (moves.right) { moves.right = false; }
          // std::cout << "Ran into something while on the ground, setting directional movement to FALSE\n";
          // set x and z loc to prev loc
          model.get()->getFirstEntity()->moveTo(glm::vec3(
            model.get()->getFirstEntity()->gameItem.prevLoc.x,
            model.get()->getFirstEntity()->gameItem.loc.y,
            model.get()->getFirstEntity()->gameItem.prevLoc.z)
          );
        }
      }
      else
      {
        glm::vec3 yCheckBelow = glm::vec3(
          model.get()->getFirstEntity()->collider->impasse.loc.x,
          model.get()->getFirstEntity()->collider->impasse.loc.y - model.get()->getFirstEntity()->collider->impasse.size.y / 2 - 0.1f,
          model.get()->getFirstEntity()->collider->impasse.loc.z
        );

        if (needsToFall)
          needsToFall = cHandler.get()->point_vs_AABB_3D(yCheckBelow, e.collider->impasse);
      }
    }

  } // exit foreach entity loop

  //std::cout << "Entities within logic checking range: " << inLogicCheckingRangeCount << "\n";


  if (needsToFall) {
    moves.onGround = false;
    moves.falling = true;
  }

}
/**
 *  Move the cam based on offset to near the player model.
 */
void FirstPersonPlayer::syncFrontVectorVisual()
{
  (model->getFirstEntity() + 1)->moveTo(g_camera.Position + *g_camera.getFront() * _frontCheckerVecScaler);
}
/**
 *  Move the cam based on offset to near the player model.
 */
void FirstPersonPlayer::syncCam()
{
  g_camera.setPosition(model->getFirstEntity()->gameItem.loc + _camOffset);
}

/**
 *  Moves the player raidus light to the gameItem location.
 *  @param[in] shader  Shader to send the lighting information to.
 */
void FirstPersonPlayer::syncPlayerLight(Shader* shader)
{
  if (status.radiusLightOn)
  {
    movePlayerLight(glm::vec3(model.get()->getFirstEntity()->gameItem.loc), shader);
  }
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
void FirstPersonPlayer::removePointLight(Shader* shader)
{
  light->removePointLight(shader);
}
/**
 * Send out a vector in front of the player and erases entity that was hit.
 * @param[inout] entities  list to check against and modify (erase).
 */
void FirstPersonPlayer::destroyEntityInFrontOfPlayer(std::vector<Entity>* entities)
{
  glm::vec3 startPosition = *g_camera.getPosition();
  glm::vec3 tipOfHitScanVec = *g_camera.getFront() * _frontCheckerVecScaler;
  //std::cout << "front Point Vec @ " << tipOfHitScanVec.x << "," << tipOfHitScanVec.y << "," << tipOfHitScanVec.z << "\n";

  auto i = std::begin(*entities);

  while (i != std::end(*entities))
  {
    if (cHandler.get()->vector_vs_AABB_3D(startPosition, tipOfHitScanVec, i->collider->impasse))
    {
      *entities->erase(i);
      playSift02SoundEffect();
      moves.interacting = false;
      return;
    }
    else
    {
      ++i;
    }
  }

  moves.interacting = false;

}

/**
 * Checks if a specific entity collides with a hitscan
 * @param[in] entity  The entity to check the player's interacting front vector against.
 * @return  True if hits, False if doesn't hit.
 */
bool FirstPersonPlayer::checkFrontVectorVsEntity(const Entity* entity)
{
  glm::vec3 startPosition = *g_camera.getPosition();

  glm::vec3 tipOfHitScanVec = *g_camera.getFront() * 2.f;

  bool returner = false;

  if (cHandler.get()->vector_vs_AABB_3D(startPosition, tipOfHitScanVec, entity->collider->impasse))
  {
    returner = true;
    moves.interacting = false;
  }

  return returner;

}

void FirstPersonPlayer::usePlanter(PrimativeRenderer* prims)
{
  planter->plantDemoTree(
    (model->getFirstEntity() + 1)->gameItem.loc,
    prims
  );

  /*prims->getEntites()->push_back(
    *planter->plantDemoTree((model->getFirstEntity() + 1)->gameItem.loc, prims->getEntites())
  );*/

  moves.usingTool = false;
}

/**
 * Toggle the player raidus light on or off.
 * @param shader  shader that has the light details.
 */
void FirstPersonPlayer::toggleRadiusLight(Shader* shader)
{

  if (status.radiusLightOn)
  {
    removePointLight(shader);
  }
  else
  {
    addPointLight((model->getFirstEntity()->gameItem.loc), shader);  // add light at location of the player model
  }

  status.radiusLightOn = !status.radiusLightOn;  // toggle state

  moves.useItem01 = false;

}

/**
 * Common init ran from the constructors.
 */
void FirstPersonPlayer::init()
{
  model = std::make_unique<PrimativeRenderer>();

  TextureLoader tLoader;
  unsigned int texID = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/firstPersonPlayer/foot.png");
  unsigned int texIDRed = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/firstPersonPlayer/red_shimmer.png");

  Entity e(
    ENTITYTYPE::CUBE,
    glm::vec3(0, 1, 0),
    glm::vec3(.115f, 1.0f, .115f),
    texID,
    true,
    true
  );
  model->addToPrimativeEntities(e);

  Entity e2(
    ENTITYTYPE::CUBE,
    glm::vec3(0, 1, 0),
    glm::vec3(0.06f, 0.06f, 0.06f),
    texIDRed,
    true,
    false
  );
  model->addToPrimativeEntities(e2);

  light = std::make_unique<Lighting>();
  light->setConstantLight(model->getShader());

  cHandler = std::make_unique<CollisionHandler>();

  planter = std::make_unique<Planter>();

  syncFrontVectorVisual();

  legPower = 10.f;
  jumpTimer = 0.0f;

}
