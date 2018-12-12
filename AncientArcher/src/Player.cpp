#include "Player.h"
#include <Constraints.h>

Player::Player() {

  BaseSpeed = 10.0f;  // base stats
  BaseJump = 4.0f;
  LegPower = 10.0f;

  weaponSelect = 0;  // no weapon

}

Player::Player(float base_speed, float base_jump, float leg_power) {

  BaseSpeed = base_speed;
  BaseJump = base_jump;
  LegPower = leg_power;
  
  weaponSelect = 1;

}


Player::~Player() {}

void Player::increaseLegPower(float add) {
  if (LegPower < 100.00f) {
    LegPower += add;
    if (LegPower > 100.00f) {
      LegPower = 100.00f;
    }
  }
}

void Player::selectWeapon(int weapnum) {
  if (weapnum >= 0 && weapnum <= 2) {  //range of valid weapons
    weaponSelect = weapnum;
  }
}

int Player::getSelectedWeapon() {
  return weaponSelect;
}

float Player::getRunSpeed() {
                       //stat_divisor is from Constraints
  // runspeed = BaseSpeed + (LegPower / 20.0f))
  return ((LegPower / stat_divisor) + BaseSpeed);
}

float Player::getRisingSpeed() {
  return ((LegPower / stat_divisor) + BaseJump);
}

float Player::getJumpHeight() {
  return (LegPower / stat_divisor) + 0.8f;   // at least .8f above the starting position
}
