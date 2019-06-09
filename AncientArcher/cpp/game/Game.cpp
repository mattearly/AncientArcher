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
  g_lighting.updateConstantLightDiffuse(glm::vec3(.6, .6, .6));
  g_lighting.updateConstantLightSpecular(glm::vec3(1, 1, 1));

  g_lighting.setConstantLight(prims->getShader());

  TextureLoader tLoader;
  unsigned int texIDGrass = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/grass.png");
  unsigned int texIDDirt = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/dirt.png");
  unsigned int texIDLightBricks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/light_bricks.png");
  unsigned int texIDMosaicBricks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/mosaic_bricks.png");


  // grass + dirt layer
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      for (int k = 0; k < 2; k++)
      {
        Entity e(
          ENTITYTYPE::CUBE,
          glm::vec3(i*2, -3.f -.5f*k , j*2),
          glm::vec3(2.f, .5f, 2.f),
          k < 1 ? texIDGrass : texIDDirt,
          true,
          false
        );
        prims->addToPrimativeEntities(e);
      }
    }
  }

  // bridge
  for (int i = 10; i < 24; i++)
  {
    for (int j = 5; j < 8; j++)
    {
        Entity e(
          ENTITYTYPE::CUBE,
          glm::vec3(i * 2, -3.f, j * 2),
          glm::vec3(2.f, 2.0f, 2.f),
          texIDLightBricks,
          true,
          false
        );
        prims->addToPrimativeEntities(e);
        if (j == 5 || j == 7)  //side railings
        {
          Entity e(
            ENTITYTYPE::SPHERE,
            glm::vec3(i * 2, -2.f, j * 2),
            glm::vec3(2.0f, 1.0f, .5f),
            texIDLightBricks,
            true,
            false
          );
          prims->addToPrimativeEntities(e);
        }
    }
  }

  // other land of maze
  for (int i = 24; i < 44; i++)  //ground
  {
    for (int j = 0; j < 10; j++)
    {
      for (int k = 0; k < 2; k++)
      {
        Entity e(
          ENTITYTYPE::CUBE,
          glm::vec3(i * 2, -4.f - .5f *k, j * 2),
          glm::vec3(2.f, .5f, 2.f),
          k < 1 ? texIDGrass : texIDDirt,
          true,
          false
        );
        prims->addToPrimativeEntities(e);
      }

    }
  }

  Entity e(
	  ENTITYTYPE::CUBE,
	  glm::vec3(9.f, -2.25f, 9.f),
	  glm::vec3(1.f, 1.f, 1.f),
	  texIDMosaicBricks,
	  true,
	  false
  );
  prims->addToMovingEntities(e);

  //for (int i = 6; i < 38; i++)  // walls
  //{
  //  for (int j = 5; j < 8; j++)
  //  {
  //    Entity e(
  //      ENTITYTYPE::CUBE,
  //      glm::vec3(i * 2, -2.f - .5f, j * 2),
  //      glm::vec3(2.f, .5f, 2.f),
  //      k < 1 ? texIDGrass : texIDDirt,
  //      true,
  //      false
  //    );
  //    prims->addToPrimativeEntities(e);
  //  }
  //}

}
