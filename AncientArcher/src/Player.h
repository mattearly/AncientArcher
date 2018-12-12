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


  // ACCESSORS
  int getSelectedWeapon();

  float getRunSpeed();

  float getRisingSpeed();

  float getJumpHeight();


private:
  float BaseSpeed;
  float BaseJump;
  float LegPower;
  int weaponSelect;

};

