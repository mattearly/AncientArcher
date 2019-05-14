#pragma once
struct Movement {

  bool positionChanged = false;

  bool forward = false, back = false;
  bool left = false, right = false;
  bool boost = false;

  bool jumped = false;
  bool canJumpAgain = true;
  bool onGround = true;
  bool falling = false;

  float lastPlayerIntendedLocation = 0.0f;
  float currentPlayerIntendedLocation = 0.0f;

  float lastOnGroundHeight = 0.0f;
  float currentGroundHeight = 0.0f;

  float intendedLocation[3] = { 0.0f, 0.0f, 0.0f };

  bool canJump() { return canJumpAgain && onGround && !falling; };
  bool canBoost() { return forward && !back; };
  bool isMoving() { return back || forward || left || right || jumped || !onGround; };
  bool isBoosted() { return forward && boost; };

};
