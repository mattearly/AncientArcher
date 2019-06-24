#include "TopDownPlayer.h"
extern Camera g_camera;
extern Controls g_controls;
void TopDownPlayer::initPlayerEntity()
{
  TextureLoader texLoader;
  unsigned int texture = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/topDownPlayer/chronos-javelin.png");
    _entity = std::make_shared<Entity>(glm::vec3(0), glm::ve);
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

void TopDownPlayer::update(float deltaTime)
{
  g_controls.tdpKeyboardIn(_moveStatus.get())

  if (_moveStatus.up)
  {
    moveUp(1.f);
  }
  if (_moveStatus.down)
  {
    moveUp(-1.f);
  }
  if (_moveStatus.left)
  {
    moveRight(-1.f);
  }
  if (_moveStatus.right)
  {
    moveRight(1.f);
  }


}
