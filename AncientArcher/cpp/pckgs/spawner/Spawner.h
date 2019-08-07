//#pragma once
////#include "../primatives/PrimativeRenderer.h"
//#include <Collider.h>
//#include <Entity.h>
//#include <memory>
//class Spawner
//{
//public:
//
//  void checkAndSpawn(float deltaTime);
//
//  void setTimeBetweenSpawns(float time);
//
//  void setPopulationCap(unsigned int max);
//
//  void update(float deltaTime);
//
//  void render();
//
//  void takeHit(float damage);
//
//  void despawn();
//
//  float getDamage();
//
//  Collider* getCollider();
//  Entity* getEntity();
//  unsigned int getAliveCount();
//  void init();
//
//private:
//
//  std::unique_ptr<PrimativeRenderer> thing;
//
//  unsigned int _popCap = 0;
//
//  unsigned int _numberAlive = 0;
//
//  float _timeBetweenSpawns = 0.f;
//
//  unsigned int texID = 0;
//
//  float _defaultHP = 2.1f;
//
//  float _currentHP = _defaultHP;
//
//  float _defaultAttackDamage = 0.0f;
//
//  void spawnAt(glm::vec3 loc);
//};