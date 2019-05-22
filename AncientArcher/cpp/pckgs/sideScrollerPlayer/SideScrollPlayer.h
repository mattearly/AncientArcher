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
  void render();

  Collider* getCollider();
  Entity* getEntity();

private:
  
  std::unique_ptr<PrimativeRenderer> playerModel;

};