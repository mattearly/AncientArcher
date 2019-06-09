#pragma once
#include <Collider.h>
#include <Kinematics.h>
#include <Entity.h>
#include <Shader.h>
#include <Lighting.h>
#include <glm/glm.hpp>
#include <vector>
#include <Controls.h>

struct Movement {

  bool positionChanged = false;

  bool forward = false, back = false;
  bool left = false, right = false;
  bool boost = false;

  bool jumped = false;
  bool canJumpAgain = true;
  bool onGround = false;
  bool falling = true;

  float lastOnGroundHeight = 0.0f;
  float currentGroundHeight = 0.0f;

  bool canJump() { return canJumpAgain && onGround && !falling; };
  bool canBoost() { return forward && !back; };
  bool isMoving() { return back || forward || left || right || jumped || !onGround; };
  bool isBoosted() { return forward && boost; };

};

class FirstPersonPlayer {
public:
  Movement movement;

  // accessors
  float getRunSpeed() const;
  float getRisingSpeed() const;
  float getJumpHeight() const;

  FirstPersonPlayer();
  FirstPersonPlayer(float leg_power);

  void render() const;
  void increaseLegPower(float add);
  void processControls();
  void addPointLight(glm::vec3 pos, Shader* shader);
  void movePlayerLight(glm::vec3 newpos, Shader* shader);

private:

  const float BASE_PLAYER_SPEED = 3.0f;
  const float BASE_PLAYER_JUMP_SPEED = 1.5f;
  const float BASE_PLAYER_JUMP_HEIGHT = 0.8f;
  const float BASE_PLAYER_WEIGHT = 160.0f;

  Entity* entity;
  Lighting* light;

  float jumpTimer;

  // player stats
  float legPower;
  const float LEGPOWER_CAP = 100.0f;

  // control the divisor of the actual in-game stat, lower provides bigger results in the getter fuctions
  const float STAT_DIVISOR = 40.0f;

};

