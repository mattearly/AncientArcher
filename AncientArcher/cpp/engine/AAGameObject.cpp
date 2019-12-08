#include "AAGameObject.h"
#include "AAViewport.h"
#include <glad\glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <glm\gtx\transform.hpp>

glm::mat4* AAGameObject::getModelMatrix()
{
  return &mModelMatrix;
}

AAGameObject::AAGameObject(std::vector<MeshDrawInfo> meshes, Shading shading)
  : mMeshes(meshes), mModelShaderType(shading) {}

void AAGameObject::draw()
{
  switch (mModelShaderType)
  {
  case NONE:
    break;
  case CELL:
    AAViewport::getInstance()->mCellShader->use();
    AAViewport::getInstance()->mCellShader->setMat4("view", AAViewport::getInstance()->getViewMatrix());
    AAViewport::getInstance()->mCellShader->setMat4("model", mModelMatrix);
    break;
  case IMGTEX:
    AAViewport::getInstance()->mTexShader->use();
    AAViewport::getInstance()->mTexShader->setMat4("view", AAViewport::getInstance()->getViewMatrix());
    AAViewport::getInstance()->mTexShader->setMat4("model", mModelMatrix);
    break;
  default:
    break;
  }


  glEnable(GL_DEPTH_TEST);

  unsigned int diffuseNumber = 1;
  unsigned int specNumber = 1;
  unsigned int normNumber = 1;
  unsigned int heightNumber = 1;

  for (auto m : mMeshes)
  {

    for (unsigned int i = 0; i < m.textures.size(); ++i)
    {

      glActiveTexture(GL_TEXTURE0 + i);
      std::string texType = m.textures[i].type;

      std::string texNumber;

      if (texType == "diffuse")
      {
        texNumber = std::to_string(diffuseNumber++);
      }
      else if (texType == "specular")
      {
        texNumber = std::to_string(specNumber++);
      }
      else if (texType == "normal")
      {
        texNumber = std::to_string(normNumber++);
      }
      else if (texType == "height")
      {
        texNumber = std::to_string(heightNumber++);
      }
      else if (texType == "color")
      {
        std::cout << "setting cell shader color...\n";
        AAViewport::getInstance()->mCellShader->setVec3("color", m.textures[i].color);
        break;
      }
      else
      {
        std::cout << "error setting tex on mesh render type\n";
      }

      glUniform1i(glGetUniformLocation(AAViewport::getInstance()->mTexShader->ID, ("material." + texType + texNumber).c_str()), i);

      glBindTexture(GL_TEXTURE_2D, m.textures[i].id);

    }


    glBindVertexArray(m.vao);
    glDrawElements(GL_TRIANGLES, (unsigned int)m.elements.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  glActiveTexture(GL_TEXTURE0);

  //std::cout << "drawingmodel\n";
}

void AAGameObject::translate(glm::vec3 amt)
{
  mModelMatrix = glm::translate(mModelMatrix, amt);
}

void AAGameObject::rotate(float angle, glm::vec3 amt)
{
  mModelMatrix *= glm::rotate(angle, amt);
}

MeshDrawInfo::MeshDrawInfo(unsigned int a, unsigned int b, unsigned int e, std::vector<TextureInfo> t, std::vector<unsigned int> el)
  : vao(a), vbo(b), ebo(e), textures(t), elements(el)
{
}
