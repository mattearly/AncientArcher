#pragma once
#include "AAShaderManager.h"
#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>
#include <vector>
#include <memory>
#include <string>
#include <ColliderBox.h>

struct TextureInfo
{
  unsigned int id = 0;
  std::string type;
  std::string path;
  glm::vec3 color = glm::vec3(1);
};

struct MeshDrawInfo
{
  MeshDrawInfo(unsigned int a, unsigned int b, unsigned int e, std::vector<TextureInfo> t, std::vector<unsigned int> el);
  unsigned int vao = 0;
  unsigned int vbo = 0;
  unsigned int ebo = 0;
  std::vector<TextureInfo> textures;
  std::vector<unsigned int> elements;
};

class AAGameObject
{
public:

  glm::mat4* getModelMatrix() noexcept;

  AAGameObject(std::vector<MeshDrawInfo> meshes);
  AAGameObject(std::vector<MeshDrawInfo> meshes, glm::vec3 collider_loc, glm::vec3 collider_sz);

  void draw(const Shader& shader);

  void scale(glm::vec3 amt);
  void rotate(float angle, glm::vec3 axis);
  void translate(glm::vec3 amt);

  void advanceScale(glm::vec3 amt);
  void advanceRotation(float angleAmtRadians);
  void advanceTranslate(glm::vec3 amt);
  void changeRotateAxis(glm::vec3 axis);

  const glm::vec3& getLocation() const noexcept;

  bool mImpedes = false;


private:

  std::vector<MeshDrawInfo> mMeshes;

  glm::vec3 mScale = glm::vec3(1);
  glm::vec3 mRotateAxis = glm::vec3(1);
  float mRotateAngle = 0.f;
  glm::vec3 mTranslate = glm::vec3(1);

  void updateModelMatrix();

  glm::mat4 mModelMatrix = glm::mat4(1);

  std::unique_ptr<mearly::ColliderBox> mColliderBox;

};
