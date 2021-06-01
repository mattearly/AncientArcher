#include "Bone.h"
namespace AA {

/*reads keyframes from aiNodeAnim*/
Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
  : m_Name(name), m_ID(ID), m_LocalTransform(1.0f) {
  m_NumPositions = channel->mNumPositionKeys;

  for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex) {
    aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
    f64 timeStamp = channel->mPositionKeys[positionIndex].mTime;
    KeyPosition data{};
    data.position = vec3(aiPosition.x, aiPosition.y, aiPosition.z);
    data.timeStamp = timeStamp;
    m_Positions.push_back(data);
  }

  m_NumRotations = channel->mNumRotationKeys;
  for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex) {
    aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
    f64 timeStamp = channel->mRotationKeys[rotationIndex].mTime;
    KeyRotation data{};
    data.orientation = quat(aiOrientation.x, aiOrientation.y, aiOrientation.z, aiOrientation.w);
    data.timeStamp = timeStamp;
    m_Rotations.push_back(data);
  }

  m_NumScalings = channel->mNumScalingKeys;
  for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex) {
    aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
    f64 timeStamp = channel->mScalingKeys[keyIndex].mTime;
    KeyScale data{};
    data.scale = vec3(scale.x, scale.y, scale.z);
    data.timeStamp = timeStamp;
    m_Scales.push_back(data);
  }
}

/* Interpolates b/w positions,rotations & scaling keys based on the curren time of the
mAnimation and prepares the local transformation matrix by combining all keys tranformations */
void Bone::Update(float animationTime) {
  glm::mat4 translation = InterpolatePosition(animationTime);
  glm::mat4 rotation = InterpolateRotation(animationTime);
  glm::mat4 scale = InterpolateScaling(animationTime);
  m_LocalTransform = translation * rotation * scale;
}

glm::mat4 Bone::GetLocalTransform() { return m_LocalTransform; }
std::string Bone::GetBoneName() const { return m_Name; }
int Bone::GetBoneID() { return m_ID; }

/* Gets the current index on mKeyPositions to interpolate to based on the current
mAnimation time */
int Bone::GetPositionIndex(float animationTime) {
  for (int index = 0; index < m_NumPositions - 1; ++index) {
    if (animationTime < m_Positions[static_cast<std::vector<AA::KeyPosition, std::allocator<AA::KeyPosition>>::size_type>(index) + 1].timeStamp)
      return index;
  }
  assert(0);
}

/* Gets the current index on mKeyRotations to interpolate to based on the current
mAnimation time */
int Bone::GetRotationIndex(float animationTime) {
  for (int index = 0; index < m_NumRotations - 1; ++index) {
    if (animationTime < m_Rotations[static_cast<std::vector<AA::KeyRotation, std::allocator<AA::KeyRotation>>::size_type>(index) + 1].timeStamp)
      return index;
  }
  assert(0);
}

/* Gets the current index on mKeyScalings to interpolate to based on the current
mAnimation time */
int Bone::GetScaleIndex(float animationTime) {
  for (int index = 0; index < m_NumScalings - 1; ++index) {
    if (animationTime < m_Scales[static_cast<std::vector<AA::KeyScale, std::allocator<AA::KeyScale>>::size_type>(index) + 1].timeStamp)
      return index;
  }
  assert(0);
}

/* Gets normalized value for Lerp & Slerp*/
f64 Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
  float scaleFactor = 0.0f;
  float midWayLength = animationTime - lastTimeStamp;
  float framesDiff = nextTimeStamp - lastTimeStamp;
  scaleFactor = midWayLength / framesDiff;
  return scaleFactor;
}

/* figures out which position keys to interpolate b/w and performs the interpolation
and returns the translation matrix */
glm::mat4 Bone::InterpolatePosition(float animationTime) {
  if (1 == m_NumPositions)
    return glm::translate(glm::mat4(1.0f), m_Positions[0].position);

  int p0Index = GetPositionIndex(animationTime);
  int p1Index = p0Index + 1;
  f64 scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,
    m_Positions[p1Index].timeStamp, animationTime);
  glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position,
    m_Positions[p1Index].position
    , scaleFactor);
  return glm::translate(glm::mat4(1.0f), finalPosition);
}

/* figures out which rotations keys to interpolate b/w and performs the interpolation
and returns the rotation matrix */
glm::mat4 Bone::InterpolateRotation(float animationTime) {
  if (1 == m_NumRotations) {
    auto rotation = glm::normalize(m_Rotations[0].orientation);
    return glm::toMat4(rotation);
  }

  int p0Index = GetRotationIndex(animationTime);
  int p1Index = p0Index + 1;
  float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, animationTime);
  glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, scaleFactor);
  finalRotation = glm::normalize(finalRotation);
  return glm::toMat4(finalRotation);
}

/* figures out which scaling keys to interpolate b/w and performs the interpolation
and returns the scale matrix */
glm::mat4 Bone::InterpolateScaling(float animationTime) {
  if (1 == m_NumScalings)
    return glm::scale(glm::mat4(1.0f), m_Scales[0].scale);

  int p0Index = GetScaleIndex(animationTime);
  int p1Index = p0Index + 1;
  float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,
    m_Scales[p1Index].timeStamp, animationTime);
  glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale,
    m_Scales[p1Index].scale, scaleFactor);
  return glm::scale(glm::mat4(1.0f), finalScale);
}

} // end namespace AA
