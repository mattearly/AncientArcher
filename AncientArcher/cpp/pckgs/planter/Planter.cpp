#include "Planter.h"
#include <CollisionHandler.h>
#include <Sound.h>
#include <Entity.h>

/**
 *  Returns a new beginning to grow tree entity. Checks for collision of world is not null.
 *  @param[in] loc    Location to plant the object.
 *  @param[in] prims  A PrimativeRenderer object to check for collision againt and place a new entity onto.
 */
void Planter::plantDemoTree(const glm::vec3 loc, PrimativeRenderer *prims)
{
  if (texIDPlant == 0) 
  {
    texIDPlant = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/planter/bark.png");
  }

  Entity* newPlant = new Entity(
    ENTITYTYPE::CUBE,
    loc,
    glm::vec3(0.9f),
    texIDPlant,
    true,
    false
  );

  if (prims)
  {
    static CollisionHandler ch;
    for (const auto& e : *prims->getEntites())
    {
      if (ch.AABB_vs_AABB_3D(newPlant->collider->impasse, e.collider->impasse))
      {
        delete newPlant;
        return;
      }
    }
  }

  prims->addToPrimativeEntities(*newPlant);
  playequipgearsound();
}
