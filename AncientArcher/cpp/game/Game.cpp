#include "Game.h"
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <Controls.h>
#include <TextureLoader.h>

extern Display g_display;
extern Controls g_controls;

Game::Game()
{
  int __textures_allowed = 0, __totalTexturesAllowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
  std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";
  std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";

  keypress = std::make_shared<keys>();
  mousepos = std::make_shared<mouse>();
  scrolling = std::make_shared<scroll>();

  g_controls.setKeyboard(keypress);
  g_controls.setMouse(mousepos);
  g_controls.setScroller(scrolling);

  world = new World();

  TextureLoader tLoader;
  unsigned int texIDGrass = tLoader.load2DTexture("../AncientArcher/resource/world/grass.png");
  unsigned int texIDCrumblingRocks = tLoader.load2DTexture("../AncientArcher/resource/world/crumbling_rocks.png");
  unsigned int texIDDirt = tLoader.load2DTexture("../AncientArcher/resource/world/dirt.png");
  unsigned int texIDLightBricks = tLoader.load2DTexture("../AncientArcher/resource/world/light_bricks.png");
  unsigned int texIDMosaicBricks = tLoader.load2DTexture("../AncientArcher/resource/world/mosaic_bricks.png");
  unsigned int texIDDarkStone = tLoader.load2DTexture("../AncientArcher/resource/world/darkstone.png");
  unsigned int texIDPackedRocks = tLoader.load2DTexture("../AncientArcher/resource/world/packed_rocks.png");
  unsigned int texIDLava = tLoader.load2DTexture("../AncientArcher/resource/world/lava.png");

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
        world->addToStationaryEntities(e);
      }
    }
  }

  //player = new FirstPersonPlayer(world->getSharedCamera(), world->getSharedShader());

  // ---- LOAD SKYBOX ---- 
  std::vector<std::string> skyboxFiles =
  {
    "../AncientArcher/cpp/pckgs/skybox/stars/right.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/left.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/top.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/bottom.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/front.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/back.png"
  };
  sky = new Skybox(world->getSharedCamera(), skyboxFiles);

  //sky = new SkyboxRenderer(world->getSharedCamera());

  world->getLight()->addPointLight(*world->getCamera()->getPosition(), world->getShader());   //debug point light
}

void Game::moveCamHelper(float dt)
{
  // this is a debug cam mover with no colliding

  static float flySpeed = 1.f;
  static float realVelocity = 0.f;
  static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
  static glm::vec3 moveFront = glm::vec3(*world->getCamera()->getFront());

  flySpeed += scrolling->yOffset;
  if (flySpeed > 10.f)
  {
    flySpeed = 10.f;
  }
  if (flySpeed < 1.f)
  {
    flySpeed = 1.f;
  }

  realVelocity = dt * flySpeed;

  if (keypress->w)
  {
    directionPlacement += moveFront * realVelocity;
  }

  if (keypress->s)
  {
    directionPlacement -= moveFront * realVelocity;
  }

  if (keypress->a)
  {
    directionPlacement -= *world->getCamera()->getRight() * realVelocity;
  }

  if (keypress->d)
  {
    directionPlacement += *world->getCamera()->getRight() * realVelocity;
  }

  world->getCamera()->increasePosition(directionPlacement);

  world->getLight()->movePointLight(0, *world->getCamera()->getPosition(), world->getShader());  // debug point light stays at cam

  directionPlacement = glm::vec3(0.f, 0.f, 0.f);
  moveFront = glm::vec3(*world->getCamera()->getFront());
}