/* 
   built based on a singleton pattern to check all collision in our world
   based off the data found at:
   https://stackoverflow.com/questions/86582/singleton-how-should-it-be-used
*/

#pragma once
#include <glm/glm.hpp>
#include <vector>
class Collision {
private:
  struct impasse {
    float location[3];
    float size[3];
  };
  std::vector<impasse> blockings;

  Collision() {};
  Collision(Collision const& copy) {};             //not implemented
  Collision& operator=(Collision const& copy) {};  //not implemented

public:
  static Collision& getInstance();
  void addCollider(glm::vec3 loc, glm::vec3 sz);
  void removeCollider(int vec_pos);
};
