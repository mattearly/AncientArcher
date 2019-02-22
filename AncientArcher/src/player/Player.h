#pragma once
#include<glm/glm.hpp>

struct Movement {

  bool positionChanged = false;

  bool forward = false, back = false;
  bool left = false, right = false;
  bool boost = false;

  bool jumped = false;
  bool canJumpAgain = true;
  bool onGround = true;
  bool falling = false;

  float timeSinceLastStep = 0.0f;

  float lastOnGroundHeight = 0.0f;
  float currentGroundHeight = 0.0f;

  float intendedLocation[3] = { 0.0f, 0.0f, 0.0f };

  bool canJump() { return canJumpAgain && onGround && !falling; };
  bool canBoost() { return forward && !back; };

};

class Player {
public:

  Player();
  Player(float base_speed, float base_jump, float leg_power);

  ~Player();

  void update(float deltaTime);

  // MUTATORS
  void increaseLegPower(float add);
  void selectWeapon(int weapnum);

  /* accessors */
  int getSelectedItem() const;
  float getRunSpeed() const;
  float getRisingSpeed() const;
  float getJumpHeight() const;
  float getAttackSpeed() const;

private:

  float baseSpeed;
  float baseJump;
  float legPower;

  float characterHeight;

  int weaponSelect;

  float attackSpeed;  // greater is slower, time in between attacks needed, starts at 1800 (1.8s between attacks)

  bool isAttacking;

  void processCommands(float deltaTime);
  
  static constexpr const float stat_divisor = 40.0f;

};

