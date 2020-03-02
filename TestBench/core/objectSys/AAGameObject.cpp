#include "AAGameObject.h"
#include "AAOGLGraphics.h"
#include "../cameraSys/AACamera.h"
#include <glad\glad.h>
#include <iostream>
#include <ColliderBox.h>

#define assLoad AAOGLGraphics::getInstance()->loadGameObjectWithAssimp

static int uniqueIDs = 0;

//glm::mat4* AAGameObject::getModelMatrix() noexcept
//{
//  return &mModelMatrix;
//}

const glm::mat4& AAGameObject::getModelMatrix() const noexcept
{
  // TODO: insert return statement here
  return mModelMatrix;
}

const int AAGameObject::getShaderId() const noexcept
{
  return mShaderID;
}
const int AAGameObject::getCameraId() const noexcept
{
  return mCameraID;
}
const int AAGameObject::getObjectId() const noexcept
{
  return mObjectID;
}

//AAGameObject::AAGameObject(std::vector<MeshDrawInfo> meshes) : mMeshes(meshes)
//{
//}

AAGameObject::AAGameObject(const char* path, int camId, int shadId)
{
  std::vector<MeshDrawInfo> meshes = assLoad(path, false);  // no triangulate

  mMeshes = meshes;
  mCameraID = camId;
  mShaderID = shadId;
  mObjectID = uniqueIDs++;
}

void AAGameObject::setCamera(int id) noexcept
{
  mCameraID = id;
}

void AAGameObject::setShader(int id) noexcept
{
  mShaderID = id;
}

void AAGameObject::draw(const AAOGLShader& modelShader) const
{
  modelShader.setMat4("model", mModelMatrix);

  glEnable(GL_DEPTH_TEST);

  for (auto m : mMeshes)
  {
    for (unsigned int i = 0; i < m.textures.size(); ++i)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      std::string texType = m.textures[i].type;  // single instance ver should be diffuse tex only

      //shader.use();
      if (texType == "color")
      {
        modelShader.setVec3("material.color", m.textures[i].color);
      }
      else
      {
        glUniform1i(glGetUniformLocation(modelShader.getID(), ("material." + texType).c_str()), i);
      }

      glBindTexture(GL_TEXTURE_2D, m.textures[i].id);
    }

    glBindVertexArray(m.vao);
    const GLsizei count = (GLsizei)m.elements.size();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
  }

  glActiveTexture(GL_TEXTURE0);
}

void AAGameObject::scale(glm::vec3 amt)
{
  mScale = amt;
  updateModelMatrix();
}

void AAGameObject::rotate(float radianAngle, glm::vec3 which)
{
  if (which.x == 0.f && which.y == 0.f && which.z == 0.f)
  {
    std::cout << "rotation axis setting can't be all 0's\n";
    return;
  }

  mRotateAngle = radianAngle;
  mRotateAxis = which;
  updateModelMatrix();
}

void AAGameObject::translate(glm::vec3 amt)
{
  mTranslate = amt;
  updateModelMatrix();
}

void AAGameObject::advanceScale(glm::vec3 amt)
{
  scale(mScale + amt);
}

void AAGameObject::advanceRotation(float angleAmtRadians)
{
  rotate(mRotateAngle + angleAmtRadians, mRotateAxis);
}

void AAGameObject::advanceTranslate(glm::vec3 amt)
{
  translate(mTranslate + amt);
}

void AAGameObject::changeRotateAxis(glm::vec3 axis)
{
  if (axis.x == 0.f && axis.y == 0.f && axis.z == 0.f)
  {
    std::cout << "rotation axis setting can't be all 0's\n";
    return;
  }

  mRotateAxis = axis;
}

const glm::vec3& AAGameObject::getLocation() const noexcept
{
  return mTranslate;
}

void AAGameObject::updateModelMatrix()
{
  mModelMatrix = glm::mat4(1);

  // Apply via order : Scale-rotate-translate (internet recommends) : fail, does all kinds of weird sh*t
  //mModelMatrix = glm::scale(mModelMatrix, mScale);
  //mModelMatrix = glm::rotate(mModelMatrix, mRotateAngle, mRotate);
  //mModelMatrix = glm::translate(mModelMatrix, mTranslate);

  // Order that seems to work logically and does what is expected: Translate, Scale, Rotate
  mModelMatrix = glm::translate(mModelMatrix, mTranslate);
  mModelMatrix = glm::scale(mModelMatrix, mScale);
  mModelMatrix = glm::rotate(mModelMatrix, mRotateAngle, mRotateAxis);
}

MeshDrawInfo::MeshDrawInfo(unsigned int a,/* unsigned int b, unsigned int e,*/ std::vector<TextureInfo> t, std::vector<unsigned int> el)
  : vao(a),/* vbo(b), ebo(e),*/ textures(t), elements(el)
{
}
