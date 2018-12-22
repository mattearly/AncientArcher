#pragma once

class Player {
public:
  Player();
  Player(float base_speed, float base_jump, float leg_power);
  ~Player();

  // MUTATORS
  void increaseLegPower(float add);

  void increaseAttackSpeed(float sub);

  void selectWeapon(int weapnum);

  void setTimeSinceLastAttack(float incr);

  bool swinging;

  // ACCESSORS
  int getSelectedWeapon();

  float getRunSpeed();

  float getRisingSpeed();

  float getJumpHeight();

  float getAttackSpeed();

  float getLastAttackTime();

private:
  
  float BaseSpeed;
  float BaseJump;
  float LegPower;
  
  int weaponSelect;

  float attackSpeed;  // greater is slower, time in between attacks needed, starts at 1800 (1.8s between attacks)

  float lastAttackTime;  // gametime stamp

};

