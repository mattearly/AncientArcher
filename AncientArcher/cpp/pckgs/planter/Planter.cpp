#include "Planter.h"

void Planter::update(float deltaTime)
{
  //for (auto& plant : allPlants)
  //{
  //  plant->growBy(glm::vec3(1.0f, 1.0f, 1.0f) * deltaTime);


    //if (plant == null)
    //{
    //  allPlants->erase(plant);
    //}
  //}
  // grow plants or whatever
}
/**
 *  Returns a new beginning to grow tree entity.
 */
Entity* Planter::plantDemoTree(glm::vec3 loc)
{
  if (texIDPlant == 0) 
  {
    texIDPlant = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/planter/bark.png");
  }

  Entity* newPlant = new Entity(
    ENTITYTYPE::CUBE,
    loc,
    glm::vec3(1.0f),
    texIDPlant,
    true,
    false
  );

  allPlants.resize(allPlants.size() + 1);

  allPlants.back() = newPlant;  
  
  return newPlant;
}
