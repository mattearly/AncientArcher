#include "Game.h"
#include <CollisionHandler.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <TextureLoader.h>
#include <Global.h>
#include <mearly.h>

// world position 0,0,0
// yaw -90.f : forward facing down the -z axis  (should be 0.f is down the -z axis, why the f is it -90? #confusedprogramming)
// pitch -89.f : downward
// field of view 80.f : pretty wide, slight fisheye
//Camera g_camera(glm::vec3(0.f, 5.0f, 0.f), -90.f, -89.0f, 80.f);
extern Display g_display;

Game::Game()
{
  int __textures_allowed = 0, __totalTexturesAllowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
  std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";
  std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";

  world = new World();

  TextureLoader tLoader;
  unsigned int texIDGrass = tLoader.load2DTexture(         "../AncientArcher/resource/world/grass.png");
  unsigned int texIDCrumblingRocks = tLoader.load2DTexture("../AncientArcher/resource/world/crumbling_rocks.png");
  unsigned int texIDDirt = tLoader.load2DTexture(          "../AncientArcher/resource/world/dirt.png");
  unsigned int texIDLightBricks = tLoader.load2DTexture(   "../AncientArcher/resource/world/light_bricks.png");
  unsigned int texIDMosaicBricks = tLoader.load2DTexture(  "../AncientArcher/resource/world/mosaic_bricks.png");
  unsigned int texIDDarkStone = tLoader.load2DTexture(     "../AncientArcher/resource/world/darkstone.png");
  unsigned int texIDPackedRocks = tLoader.load2DTexture(   "../AncientArcher/resource/world/packed_rocks.png");
  unsigned int texIDLava = tLoader.load2DTexture(          "../AncientArcher/resource/world/lava.png");

  // ---- Base Ground Layers ----
  for (int i = -20; i < 20; i++)
  {
    for (int j = -20; j < 20; j++)
    {
      for (int k = 0; k < 21; k++)
      {
        Entity e(
          ENTITYTYPE::CUBE,
          glm::vec3(i, -3.f - 1.0f * k, j),
          glm::vec3(1, 1, 1),
          (k < 1) ? texIDGrass : 
          (k < 4) ? texIDDirt : 
          (k < 8) ? texIDCrumblingRocks : 
          (k < 13) ? texIDPackedRocks : 
          (k < 19) ? texIDDarkStone : 
          texIDLava,
          true,
          false
        );
        world->addToPrimativeEntities(e);
      }
    }
  }

  player = new FirstPersonPlayer(world->getSharedCamera(), world->getSharedShader());

  // ---- LOAD SKYBOX ---- 
  //std::vector<std::string> skyboxFiles =
  //{
  //  "../AncientArcher/cpp/pckgs/skybox/stars/right.png",
  //  "../AncientArcher/cpp/pckgs/skybox/stars/left.png",
  //  "../AncientArcher/cpp/pckgs/skybox/stars/top.png",
  //  "../AncientArcher/cpp/pckgs/skybox/stars/bottom.png",
  //  "../AncientArcher/cpp/pckgs/skybox/stars/front.png",
  //  "../AncientArcher/cpp/pckgs/skybox/stars/back.png"
  //};
  //sky = new SkyboxRenderer(skyboxFiles);

  sky = new SkyboxRenderer(world->getSharedCamera());

}
