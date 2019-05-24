#pragma once
#include "../primatives/PrimativeRenderer.h"
#include <Collider.h>
#include <Entity.h>
#include <memory>
class SideScrollPlayer
{
public:
  SideScrollPlayer();

  void processControls();

  void attack();

  void render();

  Collider* getCollider();

  Collider* getSwordCollider();

  Entity* getEntity();

private:
  
  std::unique_ptr<PrimativeRenderer> playerModel;

  unsigned int weaponTexID = 0;
  
  bool _isAttacking = false;

  float _timeBetweenAttacks = 0.9f;

  float _timeSinceLastAttack = 0.f;

};