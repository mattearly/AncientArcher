#include "Pickups.h"

Pickups::Pickups() {

  speedBoostAvail = true;

  // seed to a random location
  boostLoc = glm::vec2(mearly::NTKR(1.0f, 119.0f), mearly::NTKR(1.0f, 119.0f));

}

Pickups::~Pickups() {}
