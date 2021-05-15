#pragma once
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include <vector>
#include <memory>
#include <string>
#include "Camera.h"
#include "../Renderer/OpenGL/OGLShader.h"
#include "InstanceDetails.h"
#include "../Renderer/MeshDrawInfo.h"
#include "ColliderSphere.h"
#include "../../include/AncientArcher/ShaderType.h"
namespace AA {
class Prop : public UniqueInstance {
public:

  Prop(const char* path, int camId, SHADERTYPE shadertype);

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
  glm::mat4 finalModelMatrix = glm::mat4(1);

  bool mModelMatrixChanged = false;

protected:

  void updateFinalModelMatrix();
  int mCameraID = -1;

};
}  // end namespace AA
