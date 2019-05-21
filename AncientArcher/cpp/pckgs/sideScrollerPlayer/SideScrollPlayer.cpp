#include "SideScrollPlayer.h"
#include <Controls.h>
#include <Entity.h>
#include <TextureLoader.h>

extern Controls controls;  // originally declared in Display.cpp

SideScrollPlayer::SideScrollPlayer()
{

  TextureLoader texLoader;
  unsigned int playertexID = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/sideScrollPlayer/plzno.png");

  playerModel = std::make_unique<PrimativeRenderer>();
  Entity* e = new Entity(
      ENTITYTYPE::CUBE,
      glm::vec3(0.f, 2.2f, 0.f),
      glm::vec3(1.3f, 2.33f, 1.3f),
      playertexID,
      true
    );

  playerModel->addToPrimativeEntities(*e);

  delete e;

  Lighting lighting;
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

/**
 * Render the player and effects.
 */
void SideScrollPlayer::render()
{
  playerModel->render();
}
