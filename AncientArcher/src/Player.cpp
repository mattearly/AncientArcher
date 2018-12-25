#include "Player.h"
#include "Constraints.h"
#include <iostream>

Player::Player() {

  BaseSpeed = 10.0f;  // base stats
  BaseJump = 4.0f;
  LegPower = 10.0f;

  characterHeight = 2.5f;

  weaponSelect = 0;  // no weapon

  attackSpeed = 1.8f;

  lastAttackTime = 0.0f;

  swinging = false;

}

Player::Player(float base_speed, float base_jump, float leg_power) {

  BaseSpeed = base_speed;
  BaseJump = base_jump;
  LegPower = leg_power;

  weaponSelect = 1;

  attackSpeed = 1.8f;

  lastAttackTime = 0.0f;

  swinging = false;

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

void Player::increaseAttackSpeed(float sub) {
  if (attackSpeed > 500.0f) {
    attackSpeed -= sub;
    if (attackSpeed < 500.0f) {
      attackSpeed = 500.0f;  //max attack speed is .5s between attacks
    }
  }
}

void Player::selectWeapon(int weapnum) {
  if (weapnum >= 0 && weapnum <= 2) {  //range of valid weapons
    weaponSelect = weapnum;
  }
}

void Player::setTimeSinceLastAttack(float incr) {
  lastAttackTime = incr;
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

float Player::getAttackSpeed() {
  return attackSpeed;
}

float Player::getLastAttackTime() {
  return lastAttackTime;
}
