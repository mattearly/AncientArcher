#include "Gravity.h"
#include "Entity.h"
void Gravity::apply(Entity &e, float deltaTime)
{
  e.moveby(glm::vec3(0.0f, -1.0f * deltaTime, 0.0f));
}
