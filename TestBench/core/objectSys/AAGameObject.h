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

class AAGameObject
{
public:
  // must manually set ethe cam and shader id if using this one
  //AAGameObject(std::vector<MeshDrawInfo> meshes);

  AAGameObject(const char* path, int camId, int shadId);

  //void draw();

  // getters
  const glm::vec3& getLocation() const noexcept;
  const glm::mat4& getModelMatrix() const noexcept;
  const int getShaderId() const noexcept;
  const int getCameraId() const noexcept;
  const int getObjectId() const noexcept;

  // setters
  void setCamera(int id) noexcept;
  void setShader(int id) noexcept;

  void scale(glm::vec3 amt);
  void rotate(float angle, glm::vec3 axis);
  void translate(glm::vec3 amt);
  void advanceScale(glm::vec3 amt);
  void advanceRotation(float angleAmtRadians);
  void advanceTranslate(glm::vec3 amt);
  void changeRotateAxis(glm::vec3 axis);

  friend class AAWorld;

private:

  // only AAWorld can call draw
  void draw(const AAOGLShader& modelShader) const;

  int mCameraID = -1;
  int mShaderID = -1;
  int mObjectID = -1;

  std::vector<MeshDrawInfo> mMeshes;

  glm::vec3 mScale         = glm::vec3(1);
  glm::vec3 mRotateAxis    = glm::vec3(1);
  float mRotateAngle       = 0.f;
  glm::vec3 mTranslate     = glm::vec3(1);

  void updateModelMatrix();

  glm::mat4 mModelMatrix   = glm::mat4(1);
};
