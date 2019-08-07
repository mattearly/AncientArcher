#include "RandomLevel.h"
#include <TextureLoader.h>
#include <Entity.h>
#include <glm/glm.hpp>
void RandomLevel::populateLayeredBlockGround(World& world)
{
  TextureLoader tLoader;
  unsigned int texIDGrass = tLoader.load2DTexture("../AncientArcher/resource/world/grass.png");
  unsigned int texIDCrumblingRocks = tLoader.load2DTexture("../AncientArcher/resource/world/crumbling_rocks.png");
  unsigned int texIDDirt = tLoader.load2DTexture("../AncientArcher/resource/world/dirt.png");
  unsigned int texIDLightBricks = tLoader.load2DTexture("../AncientArcher/resource/world/light_bricks.png");
  unsigned int texIDMosaicBricks = tLoader.load2DTexture("../AncientArcher/resource/world/mosaic_bricks.png");
  unsigned int texIDDarkStone = tLoader.load2DTexture("../AncientArcher/resource/world/darkstone.png");
  unsigned int texIDPackedRocks = tLoader.load2DTexture("../AncientArcher/resource/world/packed_rocks.png");
  unsigned int texIDLava = tLoader.load2DTexture("../AncientArcher/resource/world/lava.png");

  for (int i = -20; i < 20; i++)  // Base Ground Layers
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
        world.addToStationaryEntities(e);
      }
    }
  }
}

void RandomLevel::populateBoundries(World& world)
{
  //floor
  TextureLoader tLoader;
  unsigned int texIDMazeMetal = tLoader.load2DTexture("../AncientArcher/resource/14-maze_metal.png");

  for (int i = -21; i < 21; i++)  // width
  {
    for (int j = -21; j < 21; j++)  // width
    {
      for (int k = -5; k < 22; k++)  // height
      {
        if (((i == -21 || i == 20) || (j == -21 || j == 20)) || k == 21)
        {
          Entity e(
            ENTITYTYPE::CUBE,
            glm::vec3(i, -3.f - 1.0f * k, j),
            glm::vec3(1, 1, 1),
            texIDMazeMetal,
            true,
            false
          );
          world.addToStationaryEntities(e);
        }
      }
    }
  }
}
