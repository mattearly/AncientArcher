#pragma once

static float world_width = 120.0f;
static float world_height = 40.0f;
static float ground_level = 0.0f;
static float stat_divisor = 20.0f;  //this controls how much effect the player stats have - higher = less effect (hence divisor)

class Constraints {
public:
  Constraints();
  ~Constraints();
};

