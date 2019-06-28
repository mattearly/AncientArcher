#include "TopDownPlayer.h"
#include <iostream>
extern Camera g_camera;
extern Controls g_controls;

void TopDownPlayer::syncCam()
{
  //g_camera.setPosition(_entity->gameItem.loc + _camOffset);
}

void TopDownPlayer::moveRight(float amount)
{
  g_camera.Position.x += amount;
  std::cout << "pos( x:" << g_camera.getPosition()->x << " z:" << g_camera.getPosition()->z << std::endl;
}

void TopDownPlayer::moveUp(float amount)
{
  g_camera.Position.z += amount;
}

void TopDownPlayer::update()
{
  g_controls.tdpKeyboardIn(_moveStatus);
}

void TopDownPlayer::updateMovement(float deltaTime)
{
  static const float MVSPEED = 1.55666667f;
  if (_moveStatus.up)
  {
    moveUp(-MVSPEED * deltaTime);
  }
  if (_moveStatus.down)
  {
    moveUp(MVSPEED * deltaTime);
  }
  if (_moveStatus.left)
  {
    moveRight(-MVSPEED * deltaTime);
  }
  if (_moveStatus.right)
  {
    moveRight(MVSPEED * deltaTime);
  }

  //syncCam();
}
