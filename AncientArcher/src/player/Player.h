#pragma once
#include<glm/glm.hpp>

class Player {
public:

  Player();
  Player(float base_speed, float base_jump, float leg_power);

  ~Player();

  float characterHeight;

  // ACTIONS  void attack(float gametime);

  // MUTATORS
  void increaseLegPower(float add);

  void increaseAttackSpeed(float sub);

  void selectWeapon(int weapnum);

  //void setTimeSinceLastAttack(float incr);

  //void setPosition(glm::vec3 pos);

  // ACCESSORS
  int getSelectedItem();

  float getRunSpeed();

  float getRisingSpeed();

  float getJumpHeight();

  float getAttackSpeed();

  //float getLastAttackTime();

  //glm::vec3 getPosition();

private:
  
  float baseSpeed;
  float baseJump;
  float legPower;

  glm::vec3 position;  // x y z

  int weaponSelect;

  float attackSpeed;  // greater is slower, time in between attacks needed, starts at 1800 (1.8s between attacks)

  //float lastAttackTimeStamp;  // gametime stamp

  bool isAttacking;

};

