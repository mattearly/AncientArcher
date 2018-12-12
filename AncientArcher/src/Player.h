#pragma once

namespace mainPlayer {

}

class Player {
public:
  Player();
  Player(float base_speed, float base_jump, float leg_power);
  ~Player();

  // MUTATORS
  void increaseLegPower(float add);

  void selectWeapon(int weapnum);

  void increaseTimeSinceLastAttack(float incr);


  // ACCESSORS
  int getSelectedWeapon();

  float getRunSpeed();

  float getRisingSpeed();

  float getJumpHeight();

  float getAttackSpeed();

  float getTimeSinceLastAttack();

private:
  
  float BaseSpeed;
  float BaseJump;
  float LegPower;
  
  int weaponSelect;

  float attackSpeed;  // greater is slower, time in between attacks needed

  float timeSinceLastAttack;  // in deltatime

};

