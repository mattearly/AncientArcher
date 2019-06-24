#include "TopDownPlayer.h"

void TopDownPlayer::render()
{
  _ent
}

void TopDownPlayer::initPlayerEntity()
{
  TextureLoader texLoader;
  //unsigned int texture = texLoader.load2DTexture()
  //_entity = std::make_shared();
}

TopDownPlayer::TopDownPlayer()
{
  initPlayerEntity();
}

void TopDownPlayer::syncCam()
{
  g_camera.setPosition(_entity->gameItem.loc + _camOffset);
}

void TopDownPlayer::moveRight(float amount)
{
  _position.x += amount;

}

void TopDownPlayer::moveUp(float amount)
{
  _position.z += amount;

}
