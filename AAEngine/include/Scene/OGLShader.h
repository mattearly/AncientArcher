/*
OGLShader
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
#include <string>
#include <glm\glm.hpp>
namespace AA
{
enum class SHADERTYPE {
	DIFF, LITDIFF
};
class OGLShader
{
public:

	OGLShader(const SHADERTYPE& type);
	OGLShader(const char* vert, const char* frag, bool isFilePath = true);
	//OGLShader(const char* vertex_file, const char* fragment_file, const char* geometry_file);

	void use() const noexcept;
	void stop() const noexcept;

	//uniform setters
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setUint(const std::string& name, unsigned int value) const;
	void setFloat(const std::string& name, float value) const;

	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;

	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;

	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;

	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

	const int getID() const;

	int getAndCheckShaderUniform(const std::string& name) const;

private:
	OGLShader() = delete;

	unsigned int ID;

	void LoadShader(const char* vert, const char* frag);
};
}  // end namespace AA