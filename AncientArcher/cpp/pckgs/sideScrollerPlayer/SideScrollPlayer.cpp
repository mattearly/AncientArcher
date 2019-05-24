#include "SideScrollPlayer.h"
#include <Controls.h>
#include <Entity.h>
#include <TextureLoader.h>
#include <glad/glad.h>
#include <Sound.h>

extern Controls controls;  // originally declared in Display.cpp

SideScrollPlayer::SideScrollPlayer()
{

  TextureLoader texLoader;
  unsigned int playertexID = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/sideScrollerPlayer/plzno.png");
  weaponTexID = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/sideScrollerPlayer/weapon.png");

  playerModel = std::make_unique<PrimativeRenderer>();
  Entity* e = new Entity(
    ENTITYTYPE::CUBE,
    glm::vec3(0.f, 2.5f, 0.f),
    glm::vec3(2.3f, 3.5f, 0.03f),
    playertexID,
    true
  );
  playerModel->addToPrimativeEntities(*e);
  delete e;

  Lighting lighting;
  lighting.updateConstantLightAmbient(glm::vec3(0.6f, 0.6f, 0.6f));
  lighting.updateConstantLightDirection(glm::vec3(0.6f, 0.6f, 0.6f));
  lighting.setConstantLight(playerModel->getShader());
}

/**
 * Take in keyboard/mouse processing.
 */
void SideScrollPlayer::processControls()
{
  // pass the first entity to the controls
  //controls.sideScrollPlayerKeyboardInput(playerModel->getEntityPtr(), playerModel->size());
  controls.sideScrollPlayerKeyboardInput(this);
}

void SideScrollPlayer::attack()
{
  if (!_isAttacking && playerModel->size() == 1)
  {
    _isAttacking = true;

    Entity* e = new Entity(
      ENTITYTYPE::CUBE,
      *playerModel->getEntityPtr()->gameItem.location + glm::vec3(2.15f, 2.8f, 0.f),
      glm::vec3(2.0f, 1.5f, 0.03f),
      weaponTexID,
      true
    );

    playerModel->addToPrimativeEntities(*e);

    _timeSinceLastAttack = 0.f;
  }
}

/**
 * Render the player and effects.
 */
void SideScrollPlayer::render()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_BLEND);

  playerModel->render();

  glDisable(GL_BLEND);

}

/**
 * Returns the first Entity collider
 */
Collider* SideScrollPlayer::getCollider()
{
  return playerModel->getEntityPtr()->collider;
}

Collider* SideScrollPlayer::getSwordCollider()
{
  if (_isAttacking)
  { 
   return (playerModel->getEntityPtr() + 1)->collider;
  }
  else
  {
    return NULL;
  }
}

/**
 * Returns the first Entity
 */
Entity* SideScrollPlayer::getEntity()
{
  return playerModel->getEntityPtr();
}

void SideScrollPlayer::attackTimer(float deltaTime)
{
  if (_isAttacking)
  {
    _timeSinceLastAttack += deltaTime;
    if (_timeSinceLastAttack > _timeBetweenAttacks)
    {
      _isAttacking = false;
      stopAttacking();
    }
  }
}

bool SideScrollPlayer::isAttacking()
{
  return _isAttacking;
}

float SideScrollPlayer::getAttackDamage()
{
  return _attackDamage;
}

void SideScrollPlayer::takeHit(float damage)
{
  _currentHP -= damage;
  if (_currentHP < 0.f)
  {
    // dead
    playsuccesssound();
  }
}

/**
 * This despawns the sword ( entity 2 ) 
 */
void SideScrollPlayer::stopAttacking()
{
  if (playerModel->size() > 1) {
    playerModel->entityPopBack();
  }
}

