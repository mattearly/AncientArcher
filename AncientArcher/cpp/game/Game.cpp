#include "Game.h"
#include <CollisionHandler.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <TextureLoader.h>
#include <Global.h>
#include <mearly.h>
#define DEBUG FALSE;

//--- GLOBALS ---//
Display g_display(" MMO ", Display::MouseControlType::FPP);
//              Starting Position           YAW    PITCH  FOV
//Camera g_camera(glm::vec3(0.f, 1.0f, 0.f), 0, 0, 45.f);
Camera g_camera(glm::vec3(0.f, 1.0f, 0.f), 0, 0, 75.f);
Lighting g_lighting;
//--- END GLOBALS ---//

Game::Game()
{
#ifdef DEBUG
  int __textures_allowed = 0, __totalTexturesAllowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
  std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";
  std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";
#endif

  player = new FirstPersonPlayer(100.0f);
  prims = new PrimativeRenderer();
  sky = new SkyboxRenderer();

  g_lighting.updateConstantLightAmbient(glm::vec3(.25, 0.25, 0.25));
  //g_lighting.updateConstantLightDirection(glm::vec3(-1, -.93, -1));
  g_lighting.updateConstantLightDiffuse(glm::vec3(.7, .7, .7));
  g_lighting.updateConstantLightSpecular(glm::vec3(1, 1, 1));

  g_lighting.setConstantLight(prims->getShader());

  TextureLoader tLoader;
  unsigned int texID = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/grass.png");
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      Entity e(
        ENTITYTYPE::CUBE,
        glm::vec3(i, -5.0f, j),
        glm::vec3(1.f, 8.f, 1.f),
        texID,
        true,
        false
      );
      prims->addToPrimativeEntities(e);
    }
  }

}
