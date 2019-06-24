#include <glm/glm.hpp>
#include <Entity.h>
#include <Camera.h>
#include <memory>

class TopDownPlayer
{
  int _health = 10;
  glm::vec3 _camOffset = glm::vec3(0, 40.f, 0);
  PrimativeRenderer _player;
  void initPlayerEntity();
  struct MoveStatus { bool up = 0, down = 0, left = 0, right = 0; };
  std::unique_ptr<MoveStatus> _moveStatus = std::make_unique<MoveStatus>();
public:
  void syncCam();
  void moveRight(float amount);
  void moveUp(float amount);
  void update(float deltaTime);
  //void finalCollisionCheck(const std::vector<Entity>* entities);
  //void syncFrontVectorVisual();
  void syncCam();
  //void syncPlayerLight(Lighting* light, Shader* shader);
  //void movePlayerLight(Lighting* light, glm::vec3 newpos, Shader* shader);
  //void render() const;
};