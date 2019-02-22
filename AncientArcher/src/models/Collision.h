#pragma once
#include <glm\glm.hpp>
class Player;
struct Impasse;
class Collision
{
public:
  void processPointCollision(Player *player, const float dtime, const glm::vec3 original_loc, const glm::vec3 intended_loc);
private:
};

