/*
OGLGraphics
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.
* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#include "OGLGraphics.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stb_image.h>
#include <glad/glad.h>
#include <memory>
#include <string>
#include <cstddef>
#include "Conversions.h"
#include "GameObject.h"
#include "Vertex.h"

namespace AA
{

OGLGraphics* OGLGraphics::getInstance()
{
  static std::unique_ptr<OGLGraphics> graphics = std::make_unique<OGLGraphics>();
  return graphics.get();
}

/** Render the meshes with the shader. Assumes Camera View Matrix is already set.
 *  @param[in] meshes to draw.
 *  @param[in] details about instances of the mesh to also render.
 *  @param[in] shader to use for mesh rendering pipeline.
 */
void OGLGraphics::Render(const std::vector<MeshDrawInfo>& meshes, const std::vector<InstanceDetails>& details, const OGLShader& modelShader)
{
  // turn on depth test in case something else turned it off
  glEnable(GL_DEPTH_TEST);

  // go through all meshes in the this
  for (auto m : meshes)
  {
    // go through all textures in this mesh
    for (unsigned int i = 0; i < m.textures.size(); ++i)
    {
      // activate each texture
      glActiveTexture(GL_TEXTURE0 + i);
      // get the texture type
      const std::string texType = m.textures[i].type;

      //might not need shader.use() here
      //modelShader.use();

      // tell opengl to bind the texture to a model shader uniform var
      glUniform1i(glGetUniformLocation(modelShader.getID(), ("material." + texType).c_str()), i);
      glBindTexture(GL_TEXTURE_2D, m.textures[i].id);
    }

    //modelShader.setFloat("material.Shininess", m.shininess);
    //modelShader.setVec4("material.Specular", m.specular);

    // bind verts
    glBindVertexArray(m.vao);
    const GLsizei count = (GLsizei)m.elements.size();

    // draw all the instances with their differing model matrices
    for (const auto& instance : details)
    {
      //glm::mat4 drawModel = instance.ModelMatrix * m.transformation;
      //modelShader.setMat4("model", drawModel);
      modelShader.setMat4("model", instance.ModelMatrix);
      glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
  }

  // unbind vert array
  glBindVertexArray(0);
  // reset to first texture
  glActiveTexture(GL_TEXTURE0);
}

}  // end namespace AA