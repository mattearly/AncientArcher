#include "Pickups.h"
#include "../util/mearly.h"
#include "../Constraints.h"
#include "../models/PrimativeManager.h"
#include "../globals.h"

Pickups::Pickups() {
  speedBoostAvail = false;
  respawn();
}

Pickups::~Pickups() {}

void Pickups::respawn() {
  speedBoostAvail = true;
  // seed to a random location
  speedBoostLoc = glm::vec2(mearly::NTRK(1.0f, world_width-2.0f), mearly::NTRK(1.0f, world_width-2.0f));

  attackBoostAvail = true;
  // seed to a random location
  attackBoostLoc = glm::vec2(mearly::NTRK(1.0f, world_width-2.0f), mearly::NTRK(1.0f, world_width-2.0f));
}

void Pickups::draw(PrimativeManager *primativeManager) {
  
  if (speedBoostAvail) {
    texBank.activate(15);
    primativeManager->drawCube(glm::vec3(speedBoostLoc.x, 1.0f, speedBoostLoc.y));
  }

  if (attackBoostAvail) {
    texBank.activate(4);
    primativeManager->drawCube(glm::vec3(attackBoostLoc.x, 1.0f, attackBoostLoc.y));
  }

  if (!speedBoostAvail && !attackBoostAvail) {
    respawn();
  }
}

