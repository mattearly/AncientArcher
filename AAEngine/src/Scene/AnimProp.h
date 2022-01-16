#pragma once
#include "Prop.h"
#include "Animation.h"
#include "Animator.h"
#include "../Renderer/MeshInfo.h"
#include "../Base/UniqueInstance.h"
#include "Camera.h"
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include <PxRigidDynamic.h>
#include <vector>
#include <memory>
#include <string>
#include <map>
namespace AA {


class Animator;
class Animation;
struct BoneInfo;

class AnimProp : public Prop {
public:
  AnimProp(const std::string& path);

  virtual void RemoveCache() override;
  virtual void Load(const std::string& path) override;
  virtual void Draw(const std::shared_ptr<Camera>& cam) override;

  void UpdateAnim(float dt);

  std::map<std::string, BoneInfo> m_BoneInfoMap;
  int m_BoneCounter;

  glm::mat4 mGlobalInverseTransform;

  Animator* mAnimator;
  Animation* mAnimation;

  friend class AnimModelLoader;
  friend class AncientArcher;


};

}