#pragma once
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
#include <vector>
#include <memory>
#include <string>
#include "../cameraSys/AACamera.h"
#include <ColliderBox.h>
#include "shaderSys/AAOGLShader.h"

struct TextureInfo
{
  unsigned int id = 0;
  std::string type;
  std::string path;
  glm::vec3 color = glm::vec3(1);
};

struct MeshDrawInfo
{
  MeshDrawInfo(unsigned int a,/* unsigned int b, unsigned int e,*/ std::vector<TextureInfo> t, std::vector<unsigned int> el);
  unsigned int vao = 0;
  //unsigned int vbo = 0;
  //unsigned int ebo = 0;
  std::vector<TextureInfo> textures;
  std::vector<unsigned int> elements;
};

struct InstanceDetails
{
  void updateModelMatrix();
  glm::mat4 ModelMatrix = glm::mat4(1);

  InstanceDetails();
  InstanceDetails(glm::vec3 scale, glm::vec3 rotAx, glm::vec3 transl, float rotAng);
  // updateModelMatrix uses these
  glm::vec3 Scale = glm::vec3(1);
  glm::vec3 RotationAxis = glm::vec3(1);
  glm::vec3 Translate = glm::vec3(0);
  float RotationAngle = 0.f;
};

class AAGameObject
{
public:

  AAGameObject(const char* path, int camId, int shadId);
  AAGameObject(const char* path, int camId, int shadId, std::vector<InstanceDetails> details);

  // getters

  const glm::vec3& getLocation() const;
  const glm::vec3& getLocation(int which) const;
  const glm::mat4& getModelMatrix() const;
  const glm::mat4& getModelMatrix(int which) const;
  const int getShaderId() const noexcept;
  const int getCameraId() const noexcept;
  const int getObjectId() const noexcept;
  int getInstanceCount() const noexcept;

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
