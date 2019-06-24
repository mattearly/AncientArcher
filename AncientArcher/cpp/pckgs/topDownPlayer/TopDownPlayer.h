#include <glm/glm.hpp>
#include <Entity.h>
#include <Camera.h>
#include <memory>
extern Camera g_camera;
class TopDownPlayer
{
  int _health = 10;
  glm::vec3 _position = glm::vec3(0.f, 0.f, 0.f);
  glm::vec3 _camOffset = glm::vec3(0, 40.f, 0);
  std::shared_ptr<Entity> _entity;
  void initPlayerEntity();
public:
  TopDownPlayer();
  void syncCam();
  void moveRight(float amount);
  void moveUp(float amount);
};