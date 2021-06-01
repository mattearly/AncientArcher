#pragma once
#include <vector>
#include <assimp/anim.h>
#include <glm/gtx/quaternion.hpp>
#include "../../include/AncientArcher/Types.h"
namespace AA {

struct KeyPosition {
  vec3 position;
  f64 timeStamp;
};

struct KeyRotation {
  quat orientation;
  f64 timeStamp;
};

struct KeyScale {
  vec3 scale;
  f64 timeStamp;
};

class Bone {

public:

  Bone(const std::string& name, int ID, const aiNodeAnim* channel);

  void Update(float animationTime);

  mat4 GetLocalTransform();

  string GetBoneName() const;
  int GetBoneID();

  int GetPositionIndex(float animationTime);

  int GetRotationIndex(float animationTime);

  int GetScaleIndex(float animationTime);

  f64 GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

private:

  glm::mat4 InterpolatePosition(float animationTime);

  glm::mat4 InterpolateRotation(float animationTime);

  glm::mat4 InterpolateScaling(float animationTime);

private:
  std::vector<KeyPosition> m_Positions;
  std::vector<KeyRotation> m_Rotations;
  std::vector<KeyScale> m_Scales;
  int m_NumPositions;
  int m_NumRotations;
  int m_NumScalings;

  glm::mat4 m_LocalTransform;
  std::string m_Name;
  int m_ID;
};

} // end namespace AA