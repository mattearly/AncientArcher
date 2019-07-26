#pragma once
#include <glm/glm.hpp>
#include <Entity.h>
#include <Camera.h>
#include <memory>

struct MoveStatus { bool up = 0, down = 0, left = 0, right = 0; };

class TopDownPlayer
{
public:
  TopDownPlayer();
  ~TopDownPlayer();
  void moveRight(float amount);
  void moveUp(float amount);
  void update();
  void updateMovement(float deltaTime);
  void render(Shader* shader);
private:
  void init();
  void syncCam();
  int _health = 10;
  glm::vec3 _camOffset = glm::vec3(0, 40.f, 0);
  MoveStatus _moveStatus;
  std::unique_ptr<Entity> _playerShip;
  unsigned int planeVAO = 0;
};