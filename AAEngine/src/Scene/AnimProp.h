#pragma once
#include <map>
#include <vector>
#include "BoneInfo.h"
#include "Bone.h"
#include "../Renderer/MeshDrawInfo.h"
#include "../Scene/AnimVertex.h"
#include "../Base/UniqueInstance.h"
#include "../../include/AncientArcher/ShaderType.h"
#include "../../include/AncientArcher/Types.h"
struct aiAnimation;
struct aiNode;
namespace AA {

struct AssimpNodeData {
  mat4 transformation;
  string name;
  int childrenCount;
  std::vector<AssimpNodeData> children;
};

class AnimProp : public UniqueInstance {

public:
  AnimProp() = delete;
  AnimProp(const char* path, int camId, SHADERTYPE shadertype);
  void UpdateAnimation(float dt);
  void Draw();

  SHADERTYPE mShaderType;
  std::vector<MeshDrawInfo> mMeshes;
  glm::vec3 translation = glm::vec3(0);
  float eulerRotationX = 0.f;
  float eulerRotationY = 0.f;  // direction
  float eulerRotationZ = 0.f;
  glm::vec3 scale = glm::vec3(1);
  glm::mat4 finalModelMatrix = glm::mat4(1);
  std::vector<mat4> m_FinalBoneMatrices;
  bool mModelMatrixChanged = false;

  int mCameraID = -1;
  float m_Duration;
  int m_TicksPerSecond;
  std::vector<Bone> m_Bones;
  AssimpNodeData m_RootNode;
  std::map<string, BoneInfo> m_BoneInfoMap; 
  float m_CurrentTime;
  float m_DeltaTime;

  void ReadMissingBones(const aiAnimation* mAnimation);
  void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
  Bone* FindBone(const string& name);
  void CalculateBoneTransform(const AssimpNodeData* node, mat4 parentTransform);
  void updateFinalModelMatrix();


private:

};

}  // end namespace AA