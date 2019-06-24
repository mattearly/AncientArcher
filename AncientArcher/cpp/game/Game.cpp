#include "Game.h"
#include <CollisionHandler.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <TextureLoader.h>
#include <Global.h>
#include <mearly.h>

//--- GLOBALS ---//
Display g_display(" SPACESHOOTER ", Display::MouseControlType::TOPDOWNPLAYER);
//             (Starting Position        ,  YAW    , PITCH,  FOV)
Camera g_camera(glm::vec3(0.f, 0.0f, 0.f), 0.f, 0.90f, 90.f);
//--- END GLOBALS ---//

Game::Game()
{
  int __textures_allowed = 0, __totalTexturesAllowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
  std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";
  std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";

  prims = new PrimativeRenderer();

  player = new TopDownPlayer();

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
  for (int x = -20; x < 20; x++)
  {
    for (int z = -20; z < 20; z++)
    {
      Entity e(
        ENTITYTYPE::CUBE,
        glm::vec3(x + 1, 0.f, z + 1),
        glm::vec3(1, 1, 1),
        texIDGrass,
        //(k < 1) ? texIDGrass : (k < 4) ? texIDDirt : (k < 8) ? texIDCrumblingRocks : (k < 13) ? texIDPackedRocks : (k < 19) ? texIDDarkStone : texIDLava,
        true,
        false
      );
      prims->addToPrimativeEntities(e);
      //prims->addToMovingEntities(e);
    }
  }

  // ---- LOAD SKYBOX ---- //
  std::vector<std::string> skyboxFiles =
  {
    "../AncientArcher/cpp/pckgs/skybox/stars/right.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/left.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/top.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/bottom.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/front.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/back.png"
  };

  sky = new SkyboxRenderer();

}
