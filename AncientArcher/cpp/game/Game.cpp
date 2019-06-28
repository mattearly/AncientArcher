#include "Game.h"
#include <CollisionHandler.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <TextureLoader.h>
#include <Global.h>
#include <mearly.h>

//--- GLOBALS ---//
Display g_display("SPACESHOOTER", Display::MouseControlType::TOPDOWNPLAYER);

// world position 0,0,0
// yaw -90.f : forward facing down the -z axis  (should be 0.f is down the -z axis, why the f is it -90? #confusedprogramming)
// pitch -89.f : downward
// field of view 80.f : pretty wide, slight fisheye
Camera g_camera(glm::vec3(0.f, 0.0f, 0.f), -90.f, -89.0f, 80.f);
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
  for (int x = -10; x < 10; x++)
  {
    for (int z = -30; z < 10; z++)
    {
      Entity e(
        ENTITYTYPE::PLANE,
        glm::vec3(x + 1.f, -4.f, z + 1.f),
        glm::vec3(1.f, 0.25f, 1.f),
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
