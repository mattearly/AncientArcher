#include "FirstPersonPlayer.h"
#include <Controls.h>
#include <Camera.h>
#include <glm/glm.hpp>
#include <TextureLoader.h>
#include <Global.h>

extern Controls g_controls;    // from display.cpp (engine)

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

/**
 * Construct using a previously allocated cam and light.
 */
FirstPersonPlayer::FirstPersonPlayer(std::shared_ptr<Camera> cam, std::shared_ptr<Shader> shader)
{
  _playerCamera   = cam;
  _playerShader   = shader;

  init();
}

/**
 * Main update function. Checks keyboard interaction and player move statuses to move accordingly.
 * Does not check collision.
 * @param[in] deltaTime  for calculation movement distances.
 */
void FirstPersonPlayer::update(float deltaTime)
{
  // KEYBOARD
  g_controls.keyboardInput();

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
