#pragma once
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
#include <vector>
#include <memory>
#include <string>
#include "../cameraSys/AACamera.h"
#include "shaderSys/AAOGLShader.h"
#include "InstanceDetails.h"
#include "MeshDrawInfo.h"

class AAGameObject
{
public:

  AAGameObject(const char* path, int camId, int shadId);
  AAGameObject(const char* path, int camId, int shadId, std::vector<InstanceDetails> details);

  // getters
  const glm::vec3& getLocation() const;
  const glm::vec3& getLocation(int which) const;
  int getModelMatrix(const int& which, glm::mat4& out_mat4) const;
  const int getShaderId() const noexcept;
  const int getCameraId() const noexcept;
  const int getObjectId() const noexcept;
  const std::size_t getInstanceCount() const noexcept;

  // setters
  void setCamera(int id) noexcept;
  void setShader(int id) noexcept;

  // modifiers
  void scale(glm::vec3 amt, int which);
  void scale(glm::vec3 amt);
  void rotate(float angle, glm::vec3 axis, int which);
  void rotate(float angle, glm::vec3 axis);
  void translateTo(glm::vec3 amt, int which);
  void translateTo(glm::vec3 amt);
  void advanceScale(glm::vec3 amt);
  void advanceScale(glm::vec3 amt, int which);
  void advanceRotation(float angleAmtRadians);
  void advanceRotation(float angleAmtRadians, int which);
  void advanceTranslate(glm::vec3 amt);
  void advanceTranslate(glm::vec3 amt, int which);
  void changeRotateAxis(glm::vec3 axis);
  void changeRotateAxis(glm::vec3 axis, int which);

  bool isSingleInstance() const;

  friend class AAWorld;

private:

  // only AAWorld can call draw
  void draw(const AAOGLShader& modelShader);

  int mCameraID = -1;
  int mShaderID = -1;
  int mObjectID = -1;

  std::vector<MeshDrawInfo> mMeshes;
  std::vector<InstanceDetails> mInstanceDetails;  // for instancing multiple objects, sized to the number of this object in our world

  // helpers
  void updateModelMatrix(int which);

};
