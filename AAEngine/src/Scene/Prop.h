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
namespace AA
{
class Prop : public UniqueInstance
{
public:

  Prop(const char* path, int camId, bool lit);
  //Prop(const char* path, int camId, bool lit, std::vector<InstanceDetails> details);
  const int GetCamId() const noexcept;
  void SetCamera(int id) noexcept;

  // getters
  //const glm::vec3& GetLocation() const;
  //const glm::vec3& GetLocation(int which) const;
  //const glm::vec3& GetRotation() const;
  //const glm::vec3& GetRotation(int which) const;
  //int GetModelMatrix(const int& which, glm::mat4& out_mat4) const;
  //glm::mat4 GetModelMatrix(const int& which);
  //const int GetShaderId() const noexcept;

  //const std::size_t GetInstanceCount() const noexcept;
  //const ColliderSphere* GetColliderSphere(uint32_t which = 0) const;
  //bool IsSingleInstance() const;

  // setters

  //void SetShader(int id) noexcept;
  //void SetColliderSphere(const glm::vec3& center, const float& radius, uint32_t which = 0, bool overwrite = true) noexcept;

  //void SetScale(glm::vec3 amt, int which);
  //void SetScale(glm::vec3 amt);
  //void SetRotation(glm::vec3 new_rot, int which);
  //void SetRotation(glm::vec3 new_rot);
  //void SetTranslation(glm::vec3 amt, int which);
  //void SetTranslation(glm::vec3 amt);

  //void AddToScale(glm::vec3 amt);
  //void AddToScale(glm::vec3 amt, int which);
  //void AddToRotation(glm::vec3 radianAmt);
  //void AddToRotation(glm::vec3 radianAmt, int which);
  //void AddToTranslation(glm::vec3 amt);
  //void AddToTranslation(glm::vec3 amt, int which);

  //void AddInstance(const InstanceDetails& instance_details);
  //bool RemoveInstance(int which);

  void draw();
  bool mIsLit = false;

  std::vector<MeshDrawInfo> mMeshes;

  glm::vec3 translation = glm::vec3(0);

  //glm::quat rotation = glm::quat();
  //glm::vec3 rotationAngles = glm::vec3(0);
  float eulerRotationX = 0.f;
  float eulerRotationY = 0.f;  // direction
  float eulerRotationZ = 0.f;

  glm::vec3 scale = glm::vec3(1);

  glm::mat4 finalModelMatrix = glm::mat4(1);

  void updateFinalModelMatrix();

private:

  int mCameraID = -1;

  //std::vector<InstanceDetails> mInstanceDetails;  // for instancing multiple objects, sized to the number of this object in our world

  // helpers
  //void calculateNewModelMatrix(int which);
};
}  // end namespace AA
