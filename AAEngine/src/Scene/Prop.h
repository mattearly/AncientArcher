#pragma once
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include "Camera.h"
#include "../Renderer/OpenGL/OGLShader.h"
#include "InstanceDetails.h"
#include "../Renderer/MeshDrawInfo.h"
#include "ColliderSphere.h"
#include "../../include/AncientArcher/ShaderType.h"
namespace AA {

struct BoneInfo
{
  mat4 BoneOffset = mat4(0);
  mat4 FinalTransformation = mat4(0);
};

class Prop : public UniqueInstance {
public:

  Prop(const char* path, int camId, SHADERTYPE shadertype, bool animated = false);

  const int GetCamId() const noexcept;
  void SetCamera(int id) noexcept;

  void Draw();

  SHADERTYPE mShaderType;

  std::vector<MeshDrawInfo> mMeshes;

  glm::vec3 translation = glm::vec3(0);
  float eulerRotationX = 0.f;
  float eulerRotationY = 0.f;  // direction
  float eulerRotationZ = 0.f;
  glm::vec3 scale = glm::vec3(1);
  glm::mat4 currentWorldTransform = glm::mat4(1);
  bool mModelMatrixChanged = false;

  bool isAnimated = false;
  std::map<string, u32> mBoneMapping;
  u32 mNumBones = 0;
  std::vector<BoneInfo> mBoneInfo;
  mat4 mGlobalInverseTransform;

protected:

  void updateFinalModelMatrix();
  int mCameraID = -1;

};
}  // end namespace AA
