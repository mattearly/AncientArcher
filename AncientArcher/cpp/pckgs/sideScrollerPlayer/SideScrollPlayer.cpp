#include "SideScrollPlayer.h"
#include <Controls.h>
#include <Entity.h>
#include <TextureLoader.h>
#include <glad/glad.h>
#include <Sound.h>

extern Controls controls;  // originally declared in Display.cpp

extern Camera camera;

void SideScrollPlayer::processMovement(float deltaTime)
{
  static const float SPEED = 7.f;
  static std::size_t numEntities = 0;
  static unsigned int j = 0;
  numEntities = playerModel->getEntites()->size();
  if (moves.forward || moves.backward)
  {
	if (moves.forward) {
		_direction = 0;
	}
	if (moves.backward) {
		_direction = 1;
	}

	for (j = 0; j < numEntities; ++j) {
		(playerModel->getEntityPtr() + j)->moveBy((playerModel->getEntityPtr() + j)->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.backward, _direction));
	}
	camera.Position.x += playerModel->getEntityPtr()->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.backward, _direction).x;
  }
  else
  {
	  for (j = 0; j < numEntities; ++j) {
		  if ((playerModel->getEntityPtr() + j)->kinematics->vel.x != 0.0f) {
			if ((playerModel->getEntityPtr() + j)->kinematics->vel.x > 0.0f) {
				(playerModel->getEntityPtr() + j)->moveBy((playerModel->getEntityPtr() + j)->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.backward, _direction));
				camera.Position.x += playerModel->getEntityPtr()->kinematics->getCalculatedPosition(deltaTime, moves.forward, moves.backward, _direction).x;
			}
			else {
				(playerModel->getEntityPtr() + j)->kinematics->vel.x = 0.0f;
			}
		  }
	  }
  }
}

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
    true,
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

void SideScrollPlayer::spawnSword()
{
  //if (!_isAttacking && playerModel->size() == 1)
  //{
  //  _isAttacking = true;

  //  Entity* e = new Entity(
  //    ENTITYTYPE::CUBE,
  //    *playerModel->getEntityPtr()->gameItem.loc + glm::vec3(2.15f, 2.8f, 0.f),
  //    glm::vec3(2.0f, 1.5f, 0.03f),
  //    weaponTexID,
  //    true,
	 // true
  //  );

  //  playerModel->addToPrimativeEntities(*e);

  //  _timeSinceLastAttack = 0.f;
  //}
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
Collider* SideScrollPlayer::getPlayerCollider()
{
  return playerModel->getEntityPtr()->collider;
}

Collider* SideScrollPlayer::getPlayerSwordCollider()
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

void SideScrollPlayer::updateAttackTimer(float deltaTime)
{
  if (_isAttacking)
  {
    _timeSinceLastAttack += deltaTime;
    if (_timeSinceLastAttack > _timeBetweenAttacks)
    {
      _isAttacking = false;
      despawnSword();
    }
  }
}

bool SideScrollPlayer::isAttacking()
{
  return _isAttacking;
}

float SideScrollPlayer::getAttackDamage()
{
  despawnSword();
  return _attackDamage;
}

void SideScrollPlayer::takeHit(float damage)
{
  _currentHP -= damage;
}

/**
 * This despawns the sword ( entity 2 )
 */
void SideScrollPlayer::despawnSword()
{
  if (playerModel->size() > 1) {
    playerModel->entityPopBack();
  }
}

