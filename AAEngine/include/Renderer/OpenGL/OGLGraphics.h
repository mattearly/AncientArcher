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
#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "OGLShader.h"
#include "../MeshDrawInfo.h"
#include "../../Scene/InstanceDetails.h"
#include "../Vertex.h"
namespace AA
{
class OGLGraphics
{
	friend class GameObject;
	friend class Display;
	friend class SceneLoader;
	friend class Skybox;
private:
	static void Render(const std::vector<MeshDrawInfo>& meshes, const std::vector<InstanceDetails>& details, const OGLShader& modelShader);
	static void SetViewportSize(int x, int y, int w, int h);
	static void SetViewportClearColor(glm::vec3 color) noexcept;
	static void ClearScreen() noexcept;
	//static uint32_t UploadMesh(std::vector<Vertex> verts, std::vector<uint32_t> elems);
	static uint32_t UploadMesh(const std::vector<Vertex>& verts, const std::vector<uint32_t>& elems);
	static void DeleteMesh(const uint32_t&  VAO);
	static uint32_t Upload2DTex(const unsigned char* tex_data, int width, int height);
	static uint32_t UploadCubeMapTex(std::vector<unsigned char*> tex_data, int width, int height);
	static void DeleteTex(const uint32_t& id);
};
}  // end namespace AA