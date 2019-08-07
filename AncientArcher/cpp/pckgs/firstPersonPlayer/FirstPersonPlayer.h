#pragma once
#include <Camera.h>
#include <Shader.h>
#include <Entity.h>
#include <CollisionHandler.h>
#include <keys.h>
#include <vector>
#include <memory>
class FirstPersonPlayer {
public:
  FirstPersonPlayer(std::shared_ptr<Camera>& cam, std::shared_ptr<Shader>& shader, std::shared_ptr<keys>& key);

  struct Moves {

    bool forward = false, back = false;
    bool left = false, right = false;
    bool boost = false;

    bool jumped = false;
    bool canJumpAgain = true;
    bool onGround = false;
    bool falling = true;

    bool interacting = false;
    bool usingTool = false;

    bool useItem01 = false;
    bool canUseItem01 = true;

    float currentVelocity = 0.f;

    bool canJump() { return canJumpAgain && onGround && !falling; };
    bool canBoost() { return forward && !back; };
    bool isMoving() { return back || forward || left || right || jumped || !onGround; };
    bool isBoosted() { return forward && boost; };

  } moves;

  struct Status {
    bool radiusLightOn = false;
  } status;

  void update();
  void finalCollisionCheck(const std::vector<Entity>* entities);
  void syncCam();

  // accessors
  float getRunSpeed() const;
  float getRisingSpeed() const;
  float getJumpHeight() const;

  // increase player stats
  void increaseLegPower(float add);

private:

  std::unique_ptr<CollisionHandler> _playerCollisionHandler;

  std::shared_ptr<Camera>   _playerCamera;
  std::shared_ptr<Shader>   _playerShader;
  std::shared_ptr<keys> _keypress;



  float jumpTimer;

  glm::vec3 _camOffset = glm::vec3(0, 0.618f, 0);  // offset from the center of the player
  float _frontCheckerVecScaler = 1.6667f;

  // player stats
  const float BASE_PLAYER_SPEED = 3.0f;
  const float BASE_PLAYER_JUMP_SPEED = 1.5f;
  const float BASE_PLAYER_JUMP_HEIGHT = 0.8f;
  const float BASE_PLAYER_WEIGHT = 160.0f;
  float legPower;
  const float LEGPOWER_CAP = 100.0f;
  // control the divisor of the actual in-game stat, lower provides bigger results in the getter fuctions
  const float STAT_DIVISOR = 10.0f;

  void updateFromKeypresses();

  void init();

};