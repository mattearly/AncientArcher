/*
Skybox
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
// derived from https://learnopengl.com/Advanced-OpenGL/Cubemaps
#include "../include/Skybox.h"
#include "../include/vendor/glad/glad.h"
#include <glm/glm.hpp>
#include <iostream>
#include "../include/OGLGraphics.h"
#include "../include/TexLoader.h"

namespace AA
{
/**
 * Custom skymap constructor. Loads up the files in the path and skymap shader.
 *
 * @param incomingSkymapFiles  A six png image cube map texture. The order must be: "right", "left", "up", "down", "front", "back"
 */
Skybox::Skybox(std::vector<std::string> incomingSkymapFiles)
{
	skyboxShader = std::make_unique<OGLShader>(
		"../assets/shaders/skycube.vert",
		"../assets/shaders/skycube.frag"
		);
	loadSkybox();
	cubemapTexture = TexLoader::getInstance()->loadCubeTexture(incomingSkymapFiles);
	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);
	//loadProjectionMatrix();  //does on first render
}

/**
 * Renders the skybox behind all other visable objects.
 */
void Skybox::render(const Camera& cam)
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	//if (AAViewport::getInstance()->hasViewportChanged())
	//{
	loadProjectionMatrix(cam);
	//}

	loadViewMatrix(cam);        // todo: only update when camera moved

	glBindVertexArray(mSkyboxVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	skyboxShader->use();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void Skybox::setProjectionMatrix(const Camera& cam)
{
	loadProjectionMatrix(cam);
}

/**
 * Loads up a cube that is pushed to the graphics card. skyboxVAO and skyboxVBO are populated with IDs.
 */
void Skybox::loadSkybox()
{
	const float SIZE = 1.f;

	float skyboxVertices[] = {
		// positions
		-SIZE,  SIZE, -SIZE,
		-SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE, -SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,

		-SIZE, -SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE,
		-SIZE, -SIZE,  SIZE,

		-SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE, -SIZE,
		 SIZE,  SIZE,  SIZE,
		 SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE,  SIZE,
		-SIZE,  SIZE, -SIZE,

		-SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE, -SIZE,
		 SIZE, -SIZE, -SIZE,
		-SIZE, -SIZE,  SIZE,
		 SIZE, -SIZE,  SIZE
	};

	//todo: use element buffers
	glGenVertexArrays(1, &mSkyboxVAO);

	unsigned int skyboxVBO = 0;

	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(mSkyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

/**
 * Sets the view matrix value on the skyboxShader from the getViewMatrix() function in AAViewport.
 * skyboxShader is in use after this call completes.
 */
void Skybox::loadViewMatrix(const Camera& cam)
{
	const glm::mat4 viewMatrix = glm::mat4(glm::mat3(cam.getViewMatrix()));
	skyboxShader->use();
	skyboxShader->setMat4("view", viewMatrix);
}

/**
 * Sets the projection matrix value on the skyboxShader from the getProjectionMatrix() function in AAViewport.
 * skyboxShader is in use after this call completes.
 */
void Skybox::loadProjectionMatrix(const Camera& cam)
{
	glm::mat4 projectionMatrix = cam.getProjectionMatrix();
	skyboxShader->use();
	skyboxShader->setMat4("projection", projectionMatrix);
}
}  // end namespace AA