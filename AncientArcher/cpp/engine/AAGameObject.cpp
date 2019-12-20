#include "AAGameObject.h"
#include "AAViewport.h"
#include <glad\glad.h>
#include <mearly/Shader.h>
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

      shader.use();
      glUniform1i(glGetUniformLocation(shader.ID, ("material." + texType).c_str()), i);


      glBindTexture(GL_TEXTURE_2D, m.textures[i].id);

    }

    glBindVertexArray(m.vao);
    glDrawElements(GL_TRIANGLES, (unsigned int)m.elements.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  glActiveTexture(GL_TEXTURE0);

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
