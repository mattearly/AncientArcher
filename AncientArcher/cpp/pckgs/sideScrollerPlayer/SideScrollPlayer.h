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

  void spawnSword();
  void despawnSword();

  void render();

  Collider* getPlayerCollider();

  Collider* getPlayerSwordCollider();

  Entity* getEntity();

  void updateAttackTimer(float deltaTime);

  bool isAttacking();

  float getAttackDamage();

  void takeHit(float damage);

private:
  
  std::unique_ptr<PrimativeRenderer> playerModel;

  unsigned int weaponTexID = 0;
  
  bool _isAttacking = false;

  float _timeBetweenAttacks = 0.7f;

  float _timeSinceLastAttack = 0.f;

  float _attackDamage = 1.f;

  float _maxHP = 2.9999f;

  float _currentHP = _maxHP;
};