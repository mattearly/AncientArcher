#pragma once

enum WEAPONSELECT { MELEE, RANGED };

namespace mainPlayer {
  static constexpr float BaseSpeed = 10.0f;
  static float LegPower = 10.0f;
  static WEAPONSELECT weaponSelect = RANGED;
}

class Player {
public:
  Player();
  ~Player();
};

