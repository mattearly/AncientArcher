#include "Game.h"
#include <CollisionHandler.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <TextureLoader.h>
#include <Global.h>
#include <mearly.h>

//--- GLOBALS ---//
Display g_display(" MMO ", Display::MouseControlType::FPP);
//              Starting Position           YAW    PITCH  FOV
//Camera g_camera(glm::vec3(0.f, 1.0f, 0.f), 0, 0, 45.f);
Camera g_camera(glm::vec3(0.f, 1.0f, 0.f), -89.991f, 0, 75.f);
Lighting g_lighting;
//--- END GLOBALS ---//

Game::Game()
{
  int __textures_allowed = 0, __totalTexturesAllowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
  std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";
  std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";

  player = new FirstPersonPlayer();
  prims = new PrimativeRenderer();

  player->addPointLight(glm::vec3(0, 0, 0), prims->getShader());

  g_lighting.updateConstantLightAmbient(glm::vec3(.09, 0.07, 0.07));
  g_lighting.updateConstantLightDirection(glm::vec3(0.35, -0.75, 0.15));
  g_lighting.updateConstantLightDiffuse(glm::vec3(.80, .70, .74));
  //g_lighting.updateConstantLightSpecular(glm::vec3(.2, .2, .2));

  g_lighting.setConstantLight(prims->getShader());

  spawner = new Spawner();
  spawner->setPopulationCap(1);
  spawner->setTimeBetweenSpawns(2.f);

  TextureLoader tLoader;
  unsigned int texIDGrass = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/grass.png");
  unsigned int texIDCrumblingRocks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/crumbling_rocks.png");
  unsigned int texIDDirt = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/dirt.png");
  unsigned int texIDLightBricks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/light_bricks.png");
  unsigned int texIDMosaicBricks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/mosaic_bricks.png");
  unsigned int texIDDarkStone = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/darkstone.png");
  unsigned int texIDPackedRocks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/packed_rocks.png");
  unsigned int texIDLava = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/lava.png");
  
  //BASE GROUND LAYERS
  for (int i = -20; i < 40; i++)
  {
    for (int j = -20; j < 40; j++)
    {
      for (int k = 0; k < 21; k++)
      {
        Entity e(
          ENTITYTYPE::CUBE,
          glm::vec3(i * 2, -3.f - 1.0f * k, j * 2),
          glm::vec3(2.f, 1.0f, 2.f),
          // Layers - 1:grass + 3:dirt + 4:crumbling + 5:packedrock + 6:darkstone + 2:lava
          (k < 1) ? texIDGrass : (k < 4) ? texIDDirt : (k < 8) ? texIDCrumblingRocks : (k < 13) ? texIDPackedRocks : (k<19)? texIDDarkStone : texIDLava,
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
    //base
    for (int j = 5; j < 8; j++)
    {
      Entity e(
        ENTITYTYPE::CUBE,
        glm::vec3(i * 2, -2.f, j * 2),
        glm::vec3(2.f, 1.0f, 2.f),
        texIDLightBricks,
        true,
        false
      );
      prims->addToPrimativeEntities(e);

      //side railings
      if (j == 5 || j == 7)  
      {
        Entity e(
          ENTITYTYPE::CUBE,
          /* loc */ glm::vec3(/*x*/(i * 2.f), /*y*/(-1.25f), /*z*/((j == 5) ? (-.75f + j * 2.f) : (.75f + j * 2.f))),
          glm::vec3(1.5f, 0.5f, .5f),
          texIDLightBricks,
          true,
          false
        );
        prims->addToPrimativeEntities(e);
      }
    }
  }


  //Moving Blocks
  for (int i = 0; i < 4; i++)  //ground
  {
    for (int j = 0; j < 5; j++)
    {
      Entity e(
        ENTITYTYPE::CUBE,
        glm::vec3(47.5f, (1.f * j) - 2.0f, (1.f * i) + 11.f),
        glm::vec3(1.f, 1.f, 1.f),
        texIDMosaicBricks,
        true,
        false
      );
      prims->addToMovingEntities(e);
    }
  }


  // ---- LOAD SKYBOX ---- //
  //std::vector<std::string> skyboxFiles =
  //{
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/right.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/left.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/top.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/bottom.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/front.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/back.png"
  //};

  sky = new SkyboxRenderer();

}
