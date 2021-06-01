#include "AnimProp.h"
#include "Camera.h"
#include "../Renderer/OpenGL/OGLGraphics.h"
#include "../Renderer/AnimModelLoader.h"
#include <iostream>

namespace AA {


AnimProp::AnimProp(const char* path, int camId, SHADERTYPE shadertype) {
  ModelLoader::LoadAnimGameObjectFromFile(mMeshes, m_BoneInfoMap, path);
  mCameraID = camId;
  mShaderType = shadertype; 
  m_CurrentTime = 0.0f;

  m_FinalBoneMatrices.reserve(100);

  for (int i = 0; i < 100; i++)  // 100 is max bones in the shader
  {
    m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
  }

  m_DeltaTime = 0.0f;
}

void AnimProp::Draw() {
  if (mCameraID == -1)
    throw("camera not set for this AnimProp");
  if (mModelMatrixChanged) {
    updateFinalModelMatrix();
    mModelMatrixChanged = false;
  }
  OGLGraphics::Render(mMeshes, finalModelMatrix, mShaderType);
}

void AnimProp::UpdateAnimation(float deltaTime) {
  m_DeltaTime = deltaTime;
  m_CurrentTime += m_TicksPerSecond * deltaTime;
  m_CurrentTime = fmod(m_CurrentTime, m_Duration);
  CalculateBoneTransform(&m_RootNode, glm::mat4(1.0f));
}

void AnimProp::ReadMissingBones(const aiAnimation* mAnimation) {}

void AnimProp::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src) {}

Bone* AnimProp::FindBone(const string& name) {
  auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
    [&](const Bone& Bone)
  {
    return Bone.GetBoneName() == name;
  }
  );
  if (iter == m_Bones.end()) return nullptr;
  else return &(*iter);
}

void AnimProp::CalculateBoneTransform(const AssimpNodeData* node, mat4 parentTransform) {
  string nodeName = node->name;
  mat4 nodeTransform = node->transformation;

  Bone* Bone = FindBone(nodeName);

  if (Bone) {
    Bone->Update(m_CurrentTime);
    nodeTransform = Bone->GetLocalTransform();
  }

  glm::mat4 globalTransformation = parentTransform * nodeTransform;

  if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end()) {
    int index = m_BoneInfoMap[nodeName].id;
    mat4 offset = m_BoneInfoMap[nodeName].offset;
    m_FinalBoneMatrices[index] = globalTransformation * offset;
  }

  for (int i = 0; i < node->childrenCount; i++)
    CalculateBoneTransform(&node->children[i], globalTransformation);
}

void AnimProp::updateFinalModelMatrix() {
  // reset to identity matrix
  finalModelMatrix = glm::mat4(1);

  // apply translation
  finalModelMatrix = glm::translate(finalModelMatrix, translation);

  // fix angles between 0 & 360
  if (eulerRotationX < 0.f)
    eulerRotationX += 360.f;
  if (eulerRotationX > 360.f)
    eulerRotationX -= 360.f;

  if (eulerRotationY < 0.f)
    eulerRotationY += 360.f;
  if (eulerRotationY > 360.f)
    eulerRotationY -= 360.f;

  if (eulerRotationZ < 0.f)
    eulerRotationZ += 360.f;
  if (eulerRotationZ > 360.f)
    eulerRotationZ -= 360.f;


  finalModelMatrix = glm::rotate(finalModelMatrix, glm::radians(eulerRotationX), glm::vec3(1, 0, 0));
  finalModelMatrix = glm::rotate(finalModelMatrix, glm::radians(eulerRotationY), glm::vec3(0, 1, 0));
  finalModelMatrix = glm::rotate(finalModelMatrix, glm::radians(eulerRotationZ), glm::vec3(0, 0, 1));

  // apply current scale
  finalModelMatrix = glm::scale(finalModelMatrix, scale);
}

}  // end namespace AA