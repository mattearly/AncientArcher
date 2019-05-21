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
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_BLEND);

  playerModel->render();

  glDisable(GL_BLEND);

}
