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
#include "../include/OGLGraphics.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stb_image.h>
#include "../include/vendor/glad/glad.h"
#include <memory>
#include <string>
#include <cstddef>
#include "../include/GameObject.h"
#include <iostream>
//#include "Vertex.h"

namespace AA
{
OGLGraphics* OGLGraphics::getInstance()
{
	static OGLGraphics* OGLgraphics = new OGLGraphics();
	return OGLgraphics;
}
OGLGraphics::OGLGraphics() {};

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
		uint32_t i = 0;
		for (auto textures : m.textureDrawIds)
		{
			// activate each texture
			glActiveTexture(GL_TEXTURE0 + i);
			// get the texture type
			const std::string texType = textures.second;

			//might not need shader.use() here
			//modelShader.use();

			// tell opengl to bind the texture to a model shader uniform var
			glUniform1i(glGetUniformLocation(modelShader.getID(), ("material." + texType).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, textures.first);
			i++;
		}

		// bind vertex
		glBindVertexArray(m.vao);
		//const GLsizei count = (GLsizei)m.elements.size();

		// draw all the instances with their differing model matrices
		for (const auto& instance : details)
		{
			modelShader.setMat4("model", instance.ModelMatrix);
			glDrawElements(GL_TRIANGLES, m.numElements, GL_UNSIGNED_INT, nullptr);
		}
	}

	// unbind vert array
	glBindVertexArray(0);
	// reset to first texture
	glActiveTexture(GL_TEXTURE0);
}
}  // end namespace AA