#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>  
#include <iostream>
#include <vector>
#include <map>
#include "../Utility/Conversions.h"
#include "Bone.h"
#include "../Renderer/Vertex.h"
#include "AnimProp.h"

namespace AA {

struct BoneInfo {
  /*id is index in finalBoneMatrices*/
  int id;

  /*offset matrix transforms vertex from model space to bone space*/
  glm::mat4 offset;
};

class Bone;
struct AssimpNodeData {
  glm::mat4 transformation;
  std::string name;
  int childrenCount;
  std::vector<AssimpNodeData> children;
};
class AnimProp;

class Animation {
public:
  Animation();

  Animation(const std::string& animationPath, AnimProp* model);

  ~Animation();

  Bone* FindBone(const std::string& name);

  float GetTicksPerSecond();

  float GetDuration();

  const AssimpNodeData& GetRootNode();

  const std::map<std::string, BoneInfo>& GetBoneIDMap();

private:
  void ReadMissingBones(const aiAnimation* animation, AnimProp& model);

  void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

  float m_Duration;
  float m_TicksPerSecond;
  std::vector<Bone> m_Bones;
  AssimpNodeData m_RootNode;
  std::map<std::string, BoneInfo> m_BoneInfoMap;
};

}
