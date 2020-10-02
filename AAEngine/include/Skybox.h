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
#pragma once
#include "Camera.h"
#include "OGLShader.h"
#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace AA
{
class Skybox
{
public:

	//Skybox(std::shared_ptr<Camera>& viewport);
	//Skybox(std::vector<std::string> incomingSkymapFiles);

	Skybox(std::vector<std::string> incomingSkymapFiles);

	void render(const Camera& cam);

	//void render(const AAOGLShader& shader);

	// to be manually called as needed
	void setProjectionMatrix(const Camera& cam);

private:

	void loadSkybox();
	void loadProjectionMatrix(const Camera& cam);
	void loadViewMatrix(const Camera& cam);

	unsigned int mSkyboxVAO = 0;
	unsigned int cubemapTexture = 0;
	std::unique_ptr<OGLShader> skyboxShader;
	//std::shared_ptr<Camera>& mCamera;
};
}  // end namespace AA
