#include "Player.h"

#include "../Constraints.h"
#include "../sound/Sound.h"
#include "../lighting/Lighting.h"

Player::Player() {

  baseSpeed = 10.0f;  // base stats
  baseJump = 4.0f;
  legPower = 10.0f;

  characterHeight = 2.5f;

  weaponSelect = 0;  // no weapon

  attackSpeed = 1.8f;

  lastAttackTimeStamp = 0.0f;

  isAttacking = false;

}

Player::Player(float base_speed, float base_jump, float leg_power) {

  baseSpeed = base_speed;
  baseJump = base_jump;
  legPower = leg_power;

  weaponSelect = 1;

  attackSpeed = 1.8f;

  lastAttackTimeStamp = 0.0f;

  isAttacking = false;

}

Player::~Player() {}

void Player::attack(float gametime) {
  switch (getSelectedItem()) {
  case 0:
    playpunchsound();
    break;
  case 1:
    playswordswingsound();
    isAttacking = true;
    break;
  case 2:
    playbowsound();
    break;
  }
  setTimeSinceLastAttack(gametime);
}

void Player::increaseLegPower(float add) {
  if (legPower < 100.00f) {
    legPower += add;
    if (legPower > 100.00f) {
      legPower = 100.00f;
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
  lastAttackTimeStamp = incr;
}

void Player::setPosition(glm::vec3 pos) {
}

int Player::getSelectedItem() {
  return weaponSelect;
}

float Player::getRunSpeed() {
                       //stat_divisor is from Constraints
  // runspeed = BaseSpeed + (LegPower / 20.0f))
  return ((legPower / stat_divisor) + baseSpeed);
}

float Player::getRisingSpeed() {
  return ((legPower / stat_divisor) + baseJump);
}

float Player::getJumpHeight() {
  return (legPower / stat_divisor) + 0.8f;   // at least .8f above the starting position
}

float Player::getAttackSpeed() {
  return attackSpeed;
}

float Player::getLastAttackTime() {
  return lastAttackTimeStamp;
}

glm::vec3 Player::getPosition() {
  return position;
}
