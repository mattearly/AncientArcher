#pragma once
#include "../primatives/PrimativeRenderer.h"
#include <memory>
class SideScrollPlayer
{
public:
  SideScrollPlayer();

  void processControls();
  void render();
private:
  std::unique_ptr<PrimativeRenderer> playerModel;
};