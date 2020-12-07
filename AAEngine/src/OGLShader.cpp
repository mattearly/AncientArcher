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
#include "../include/OGLShader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#ifdef _DEBUG
#include <iostream>
#endif
namespace AA
{
OGLShader::OGLShader(const char* vert, const char* frag, bool isFilePath)
{
	std::string vertexCode;
	std::string fragmentCode;

	const char* vertexShaderSource;
	const char* fragmentShaderSource;

	if (isFilePath) {

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
	
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vert);
			fShaderFile.open(frag);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
#ifdef _DEBUG
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
#endif

		}
		vertexShaderSource = vertexCode.c_str();
		fragmentShaderSource = fragmentCode.c_str();
	}
	else
	{
		vertexShaderSource = vert;
		fragmentShaderSource = frag;
	}

	/* compile vertex (points) shader */
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	/* determine if vertex shader was successful in compiling */
	int v_success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &v_success);
	if (!v_success)
	{
#ifdef _DEBUG
		char v_infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, v_infoLog);
		std::cout << "error in vertex shader, compilation failed: " << v_infoLog << std::endl;
		char a;
		std::cin >> a;
#endif
		exit(-1);
	}

	/* compile fragment shader */
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	/* determine if fragment shader was successful in compiling */
	int f_success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &f_success);
	if (!f_success)
	{
#ifdef _DEBUG
		char f_infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, f_infoLog);
		std::cout << "error in fragment shader, compilation failed: " << f_infoLog << std::endl;
		char a;
		std::cin >> a;
#endif
		exit(-1);
	}

	/* make the shader program */
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	/* check that the ID was successful */
	int p_success;
	glGetProgramiv(ID, GL_LINK_STATUS, &p_success);
	if (!p_success)
	{
#ifdef _DEBUG
		char p_infoLog[512];
		glGetProgramInfoLog(ID, 512, nullptr, p_infoLog);
		std::cout << "error in ID: " << p_infoLog << std::endl;
		char a;
		std::cin >> a;
#endif
		exit(-1);
	}

	/* we don't need them anymore */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#ifdef _DEBUG
	std::cout << "debug: shader ID = " << ID << std::endl;
#endif
}

OGLShader::OGLShader(const char* vertex_file, const char* frag, const char* geometry_file)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertex_file);
		fShaderFile.open(frag);
		gShaderFile.open(geometry_file);

		std::stringstream vShaderStream, fShaderStream, gShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
#ifdef _DEBUG
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
#endif
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();
	const char* geometryShaderSource = geometryCode.c_str();

	/* compile vertex (points) shader */
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	/* determine if vertex shader was successful in compiling */
	int v_success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &v_success);
	if (!v_success)
	{
#ifdef _DEBUG
		char v_infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, v_infoLog);
		std::cout << "error in vertex shader, compilation failed: " << v_infoLog << std::endl;
		char a;
		std::cin >> a;
#endif
		exit(-1);
	}

	/* compile fragment shader */
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	/* determine if fragment shader was successful in compiling */
	int f_success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &f_success);
	if (!f_success)
	{
#ifdef _DEBUG
		char f_infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, f_infoLog);
		std::cout << "error in fragment shader, compilation failed: " << f_infoLog << std::endl;
		char a;
		std::cin >> a;
#endif
		exit(-1);
	}

	/* compile geometry shader */
	int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
	glCompileShader(geometryShader);

	/* determine if geometry shader was successful in compiling */
	int g_success;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &g_success);
	if (!g_success)
	{
#ifdef _DEBUG
		char g_infoLog[512];
		glGetShaderInfoLog(geometryShader, 512, nullptr, g_infoLog);
		std::cout << "error in geometry shader, compilation failed: " << g_infoLog << std::endl;
		char a;
		std::cin >> a;
#endif
		exit(-1);
	}

	/* make the shader program */
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glAttachShader(ID, geometryShader);
	glLinkProgram(ID);

	/* check that the ID was successful */
	int p_success;
	glGetProgramiv(ID, GL_LINK_STATUS, &p_success);
	if (!p_success)
	{
#ifdef _DEBUG
		char p_infoLog[512];
		glGetProgramInfoLog(ID, 512, nullptr, p_infoLog);
		std::cout << "error in ID: " << p_infoLog << std::endl;
		char a;
		std::cin >> a;
#endif
		exit(-1);
	}

	/* we don't need them anymore */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

void OGLShader::use() const noexcept
{
	glUseProgram(ID);
}

void OGLShader::stop() const noexcept
{
	glUseProgram(0);
}

void OGLShader::setBool(const std::string& name, bool value) const
{
	glUniform1i(getAndCheckShaderUniform(name), (int)value);
}

void OGLShader::setInt(const std::string& name, int value) const
{
	glUniform1i(getAndCheckShaderUniform(name), value);
}

void OGLShader::setUint(const std::string& name, unsigned int value) const
{
	glUniform1ui(getAndCheckShaderUniform(name), value);
}
void OGLShader::setFloat(const std::string& name, float value) const
{
	glUniform1f(getAndCheckShaderUniform(name), value);
}

void OGLShader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(getAndCheckShaderUniform(name), 1, &value[0]);
}

void OGLShader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(getAndCheckShaderUniform(name), x, y);
}

void OGLShader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(getAndCheckShaderUniform(name), 1, &value[0]);
}

void OGLShader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(getAndCheckShaderUniform(name), x, y, z);
}

void OGLShader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(getAndCheckShaderUniform(name), 1, &value[0]);
}

void OGLShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(getAndCheckShaderUniform(name), x, y, z, w);
}

void OGLShader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(getAndCheckShaderUniform(name), 1, GL_FALSE, &mat[0][0]);
}

void OGLShader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(getAndCheckShaderUniform(name), 1, GL_FALSE, &mat[0][0]);
}

void OGLShader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(getAndCheckShaderUniform(name), 1, GL_FALSE, &mat[0][0]);
}

const int OGLShader::getID() const
{
	return ID;
}

int OGLShader::getAndCheckShaderUniform(const std::string& name) const
{
	const int shader_var_id = glGetUniformLocation(ID, name.c_str());

	if (shader_var_id < 0)
	{
#ifdef _DEBUG
		std::cout <<
			"!!!SETTINGS SHADER UNIFORM VARIABLE ERROR!!!\n"
			" name: " << name << '\n' <<
			" id: " << ID << '\n' <<
			" error: " << shader_var_id << '\n' <<
			"-----------------------------------\n";
#endif
	}

	return shader_var_id;
}
}  // end namespace AA