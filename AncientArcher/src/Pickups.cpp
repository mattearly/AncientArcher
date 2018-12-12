#include "Pickups.h"

Pickups::Pickups() {

  speedBoostAvail = true;
  // seed to a random location
  speedBoostLoc = glm::vec2(mearly::NTKR(1.0f, 119.0f), mearly::NTKR(1.0f, 119.0f)); 
  
  attackBoostAvail = true;
  // seed to a random location
  attackBoostLoc = glm::vec2(mearly::NTKR(1.0f, 119.0f), mearly::NTKR(1.0f, 119.0f));

}

Pickups::~Pickups() {}
