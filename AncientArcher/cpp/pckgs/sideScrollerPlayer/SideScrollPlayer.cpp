#include "SideScrollPlayer.h"
#include <Controls.h>
#include <Entity.h>
#include <TextureLoader.h>
#include <glad/glad.h>

extern Controls controls;  // originally declared in Display.cpp

SideScrollPlayer::SideScrollPlayer()
{

  TextureLoader texLoader;
  unsigned int playertexID = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/sideScrollerPlayer/plzno.png");
  weaponTexID = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/sideScrollerPlayer/weapon.png");
  
  playerModel = std::make_unique<PrimativeRenderer>();
  Entity* e = new Entity(
      ENTITYTYPE::CUBE,
      glm::vec3(0.f, 2.2f, 0.f),
      glm::vec3(1.5f, 2.5f, 0.03f),
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
  controls.sideScrollPlayerKeyboardInput(playerModel->getEntityPtr());
}

void SideScrollPlayer::attack()
{
  _isAttacking = true;

  Entity* e = new Entity(
    ENTITYTYPE::CUBE,
    *playerModel->getEntityPtr()->gameItem.location + glm::vec3(1.0f, 0.3f, 0.1f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    weaponTexID,
    true
  );

  playerModel->addToPrimativeEntities(*e);

  _timeSinceLastAttack = 0.f;
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

/**
 * Returns the first Entity
 */
Entity* SideScrollPlayer::getEntity()
{
  return playerModel->getEntityPtr();
}

