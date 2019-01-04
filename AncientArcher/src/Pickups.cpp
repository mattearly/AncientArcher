#include "Pickups.h"
#include "util/mearly.h"
#include "Constraints.h"
#include "models/PrimativeManager.h"
#include "util/TextureBank.h"

extern TextureBank texBank;

Pickups::Pickups() {

  speedBoostAvail = true;
  // seed to a random location
  speedBoostLoc = glm::vec2(mearly::NTKR(1.0f, world_width-2.0f), mearly::NTKR(1.0f, world_width-2.0f)); 
  
  attackBoostAvail = true;
  // seed to a random location
  attackBoostLoc = glm::vec2(mearly::NTKR(1.0f, world_width-2.0f), mearly::NTKR(1.0f, world_width-2.0f));

}

Pickups::~Pickups() {}

void Pickups::respawn() {
  speedBoostAvail = true;
  // seed to a random location
  speedBoostLoc = glm::vec2(mearly::NTKR(1.0f, world_width-2.0f), mearly::NTKR(1.0f, world_width-2.0f));

  attackBoostAvail = true;
  // seed to a random location
  attackBoostLoc = glm::vec2(mearly::NTKR(1.0f, world_width-2.0f), mearly::NTKR(1.0f, world_width-2.0f));
}

void Pickups::draw(PrimativeManager *primativeManager) {


  if (speedBoostAvail) {
    texBank.activate(13);
    primativeManager->drawCube(glm::vec3( speedBoostLoc.x, 2.0f, speedBoostLoc.y), glm::vec3(2.0f, 2.0f, 2.0f));
  }

  if (attackBoostAvail) {
    texBank.activate(15);
    primativeManager->drawCube(glm::vec3(attackBoostLoc.x, 2.0f, attackBoostLoc.y), glm::vec3(2.0f, 2.0f, 2.0f));
  }

}

