#include "AAGameObject.h"
#include "AAViewport.h"
#include <glad\glad.h>
#include <iostream>

glm::mat4* AAGameObject::getModelMatrix()
{
  return &mModelMatrix;
}

AAGameObject::AAGameObject(std::vector<MeshDrawInfo> meshes)
  : mMeshes(meshes) {}

void AAGameObject::draw(const Shader& shader)
{
  shader.use();
  shader.setMat4("view", AAViewport::getInstance()->getViewMatrix());
  shader.setMat4("model", mModelMatrix);

  glEnable(GL_DEPTH_TEST);

  for (auto m : mMeshes)
  {
    for (unsigned int i = 0; i < m.textures.size(); ++i)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      std::string texType = m.textures[i].type;  // single instance ver should be diffuse tex only

      //shader.use();
      glUniform1i(glGetUniformLocation(shader.ID, ("material." + texType).c_str()), i);

      glBindTexture(GL_TEXTURE_2D, m.textures[i].id);
    }

    glBindVertexArray(m.vao);
    glDrawElements(GL_TRIANGLES, (unsigned int)m.elements.size(), GL_UNSIGNED_INT, nullptr);
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

MeshDrawInfo::MeshDrawInfo(unsigned int a, unsigned int b, unsigned int e, std::vector<TextureInfo> t, std::vector<unsigned int> el)
  : vao(a), vbo(b), ebo(e), textures(t), elements(el)
{
}
