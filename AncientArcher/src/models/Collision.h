#pragma once
class Player;
struct Impasse;
class Collision
{
public:
  Collision();
  ~Collision();
  void process(Player *player, float dtime);
private:
  void checkCollision(Impasse a, Impasse b);
};

