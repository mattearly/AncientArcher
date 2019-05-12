#pragma once
#include "Entity.h"

#include<glm/glm.hpp>

class Entity;

class Player {
public:

  Player();
  Player(float leg_power);

  void update(float deltaTime);

    // accessors
  float getRunSpeed() const;
  float getRisingSpeed() const;
  float getJumpHeight() const;

    // mutators
  void increaseLegPower(float add);

  //Entity *getEntity() const;

private:

  //Entity *playerEntity;

  const float BASE_PLAYER_SPEED = 3.0f;
  const float BASE_PLAYER_JUMP_HEIGHT = 0.8f;

    // player stats
  float legPower;
  const float LEGPOWER_CAP = 100.0f;

  void processCommands(float deltaTime);

  static constexpr const float STAT_DIVISOR = 40.0f; 

};

