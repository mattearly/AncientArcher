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
#include "../../include/Scene/OGLShader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#ifdef _DEBUG
#include <iostream>
#endif
namespace AA
{

/// <summary>
/// Constructor to load an internal shader.
/// </summary>
/// <param name="type">which to use</param>
OGLShader::OGLShader(const SHADERTYPE& type)
{
	ID = -1;
	switch (type)
	{
	case SHADERTYPE::DIFF:
	{
		const char* vert_source =
			"#version 330 core\n"
			"layout(location=0)in vec3 inPos;\n"
			"layout(location=1)in vec2 inTexUV;\n"
			"out vec2 pass_TexUV;\n"
			"uniform mat4 projection;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"void main(){\n"
			"  pass_TexUV = inTexUV;\n"
			"  gl_Position = projection * view * model * vec4(inPos,1);\n"
			"}\n"
			;
		const char* frag_source =
			"#version 330 core\n"
			"in vec2 pass_TexUV;\n"
			"out vec4 out_Color;\n"
			"struct Material {\n"
			"  sampler2D Albedo;\n"
			"};\n"
			"uniform Material material;\n"
			"void main(){\n"
			"  out_Color = texture(material.Albedo, pass_TexUV);\n"
			"}\n"
			;
		LoadShader(vert_source, frag_source);
	}
	break;
	case SHADERTYPE::LITDIFF:
	{
		const char* vert_source =
			"#version 330 core\n"
			"layout(location=0)in vec3 inPos;\n"
			"layout(location=1)in vec2 inTexUV;\n"
			"layout(location=2)in vec3 inNorm;\n"
			"out vec3 pass_Pos;\n"
			"out vec2 pass_TexUV;\n"
			"out vec3 pass_Norm;\n"
			"uniform mat4 projection;\n"
			"uniform mat4 model;\n"
			"uniform mat4 view;\n"
			"void main(){	\n"
			"  pass_Pos = (model * vec4(inPos,1.)).xyz;\n"
			"  pass_TexUV = inTexUV;\n"
			"  pass_Norm = inNorm;\n"
			"  gl_Position = projection * view * model * vec4(inPos,1);\n"
			"}\n"
			;
		const char* frag_source =
			"#version 330 core\n"
			"in vec3 pass_Pos;\n"
			"in vec2 pass_TexUV;\n"
			"in vec3 pass_Norm;\n"
			"out vec3 out_Color;\n"
			"struct Material{\n"
			"  sampler2D Albedo;\n"
			"};\n"
			"struct DirectionalLight{\n"
			"  vec3 Direction;\n"
			"  vec3 Ambient;\n"
			"  vec3 Diffuse;\n"
			"  vec3 Specular;\n"
			"};\n"
			"struct PointLight{\n"
			"  vec3 Position;\n"
			"  float Constant,Linear,Quadratic;\n"
			"  vec3 Ambient,Diffuse,Specular;\n"
			"};\n"
			"struct SpotLight{\n"
			"  vec3 Position,Direction;\n"
			"  float CutOff,OuterCutOff;\n"
			"  float Constant,Linear,Quadratic;\n"
			"  vec3 Ambient,Diffuse,Specular;\n"
			"};\n"
			"const int MAXPOINTLIGHTS=50;\n"
			"const int MAXSPOTLIGHTS=25;\n"
			"uniform vec3 viewPos;\n"
			"uniform Material material;\n"
			"uniform DirectionalLight directionalLight;\n"
			"uniform PointLight pointLight[MAXPOINTLIGHTS];\n"
			"uniform SpotLight spotLight[MAXSPOTLIGHTS];\n"
			"uniform int NUM_POINT_LIGHTS;\n"
			"uniform int NUM_SPOT_LIGHTS;\n"
			"vec3 CalcDirectionalLight(vec3 normal,vec3 viewDir);\n"
			"vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir);\n"
			"vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 viewDir);\n"
			"int i = 0;\n"
			"void main(){\n"
			"  vec3 result;\n"
			"  vec3 normal = normalize(pass_Norm);\n"
			"  vec3 viewDir = normalize(viewPos - pass_Pos);\n"
			"  result += CalcDirectionalLight(normal, viewDir);\n"
			"  for (i = 0; i < NUM_POINT_LIGHTS; i++)\n"
			"    result += CalcPointLight(pointLight[i], normal, viewDir);\n"
			"  for (i = 0; i < NUM_SPOT_LIGHTS; i++)\n"
			"    result += CalcSpotLight(spotLight[i], normal, viewDir);\n"
			"  out_Color = result;\n"
			"}\n"
			"vec3 CalcDirectionalLight(vec3 normal,vec3 viewDir){\n"
			"  vec3 lightDir=normalize(-directionalLight.Direction);\n"
			"  float diff=max(dot(normal,lightDir),0.);\n"
			"  vec3 reflectDir=reflect(-lightDir,normal);\n"
			"  // float spec=pow(max(dot(viewDir,reflectDir),0.),material.Shininess);\n"
			"  vec3 ambient=directionalLight.Ambient*texture(material.Albedo,pass_TexUV).rgb;\n"
			"  vec3 diffuse=directionalLight.Diffuse*diff*texture(material.Albedo,pass_TexUV).rgb;\n"
			"  // vec3 specular = directionalLight.Specular * spec * texture(material.Specular, pass_TexUV).rgb;\n"
			"  return(ambient+diffuse/*+ specular*/);\n"
			"}\n"
			"vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir){\n"
			"  vec3 lightDir = normalize(light.Position - pass_Pos);\n"
			"  float diff = max(dot(normal, lightDir), 0.0);\n"
			"  vec3 reflectDir = reflect(-lightDir, normal);\n"
			"  // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);\n"
			"  float dist = length(light.Position - pass_Pos);\n"
			"  float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));\n"
			"  vec3 ambient = light.Ambient * texture(material.Albedo, pass_TexUV).rgb;\n"
			"  vec3 diffuse = light.Diffuse * diff * texture(material.Albedo, pass_TexUV).rgb;\n"
			"  //  vec3 specular = light.Specular * spec * vec3(texture(material.Specular, pass_TexUV);\n"
			"  ambient *= attenuation;\n"
			"  diffuse *= attenuation;\n"
			"  //  specular *= attenuation;\n"
			"  return (ambient + diffuse /*+ specular*/);\n"
			"}\n"
			"vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir){\n"
			"  vec3 lightDir = normalize(light.Position - pass_Pos);\n"
			"  float diff = max(dot(normal, lightDir), 0.0);\n"
			"  vec3 reflectDir = reflect(-lightDir, normal);\n"
			"  // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);\n"
			"  float dist = length(light.Position - pass_Pos);\n"
			"  float attenuation  = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));\n"
			"  float theta = dot(lightDir, normalize(-light.Direction));\n"
			"  float epsilon = light.CutOff - light.OuterCutOff;\n"
			"  float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);\n"
			"  vec3 ambient = light.Ambient * texture(material.Albedo, pass_TexUV).rgb;\n"
			"  vec3 diffuse = light.Diffuse * diff * texture(material.Albedo, pass_TexUV).rgb;\n"
			"  //  vec3 specular = light.Specular * spec * vec3(texture(material.Specular, pass_TexUV);\n"
			"  ambient *= attenuation * intensity;\n"
			"  diffuse *= attenuation * intensity;\n"
			"  //  specular *= attenuation * intensity;\n"
			"  return (ambient + diffuse /*+ specular*/);\n"
			"}\n";
			LoadShader(vert_source, frag_source);
	}
	break;
	default:
		throw("invalid shadertype");
		break;
	};
}


/// <summary>
/// Loads a vertex and fragment pair into a shader program. Must be of the same type(Path or Source).
/// </summary>
/// <param name="vert">Path or Source</param>
/// <param name="frag">Path or Source</param>
/// <param name="isFilePath">Defaults to true. Set to false if you are passing in source code and not a file path</param>
OGLShader::OGLShader(const char* vert, const char* frag, bool isFilePath)
{
	const char* vertexShaderSource;
	const char* fragmentShaderSource;

	if (isFilePath) {

		std::string vertexCode;
		std::string fragmentCode;

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

	LoadShader(vertexShaderSource, fragmentShaderSource);
}

//OGLShader::OGLShader(const char* vertex_file, const char* frag, const char* geometry_file)
//{
//	std::string vertexCode;
//	std::string fragmentCode;
//	std::string geometryCode;
//	std::ifstream vShaderFile;
//	std::ifstream fShaderFile;
//	std::ifstream gShaderFile;
//
//	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//	try
//	{
//		vShaderFile.open(vertex_file);
//		fShaderFile.open(frag);
//		gShaderFile.open(geometry_file);
//
//		std::stringstream vShaderStream, fShaderStream, gShaderStream;
//
//		vShaderStream << vShaderFile.rdbuf();
//		fShaderStream << fShaderFile.rdbuf();
//		gShaderStream << gShaderFile.rdbuf();
//
//		vShaderFile.close();
//		fShaderFile.close();
//		gShaderFile.close();
//
//		vertexCode = vShaderStream.str();
//		fragmentCode = fShaderStream.str();
//		geometryCode = gShaderStream.str();
//	}
//	catch (std::ifstream::failure e)
//	{
//#ifdef _DEBUG
//		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
//#endif
//	}
//
//	const char* vertexShaderSource = vertexCode.c_str();
//	const char* fragmentShaderSource = fragmentCode.c_str();
//	const char* geometryShaderSource = geometryCode.c_str();
//
//	/* compile vertex (points) shader */
//	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//	glCompileShader(vertexShader);
//
//	/* determine if vertex shader was successful in compiling */
//	int v_success;
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &v_success);
//	if (!v_success)
//	{
//#ifdef _DEBUG
//		char v_infoLog[512];
//		glGetShaderInfoLog(vertexShader, 512, nullptr, v_infoLog);
//		std::cout << "error in vertex shader, compilation failed: " << v_infoLog << std::endl;
//		char a;
//		std::cin >> a;
//#endif
//		exit(-1);
//	}
//
//	/* compile fragment shader */
//	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//	glCompileShader(fragmentShader);
//
//	/* determine if fragment shader was successful in compiling */
//	int f_success;
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &f_success);
//	if (!f_success)
//	{
//#ifdef _DEBUG
//		char f_infoLog[512];
//		glGetShaderInfoLog(fragmentShader, 512, nullptr, f_infoLog);
//		std::cout << "error in fragment shader, compilation failed: " << f_infoLog << std::endl;
//		char a;
//		std::cin >> a;
//#endif
//		exit(-1);
//	}
//
//	/* compile geometry shader */
//	int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
//	glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
//	glCompileShader(geometryShader);
//
//	/* determine if geometry shader was successful in compiling */
//	int g_success;
//	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &g_success);
//	if (!g_success)
//	{
//#ifdef _DEBUG
//		char g_infoLog[512];
//		glGetShaderInfoLog(geometryShader, 512, nullptr, g_infoLog);
//		std::cout << "error in geometry shader, compilation failed: " << g_infoLog << std::endl;
//		char a;
//		std::cin >> a;
//#endif
//		exit(-1);
//	}
//
//	/* make the shader program */
//	ID = glCreateProgram();
//	glAttachShader(ID, vertexShader);
//	glAttachShader(ID, fragmentShader);
//	glAttachShader(ID, geometryShader);
//	glLinkProgram(ID);
//
//	/* check that the ID was successful */
//	int p_success;
//	glGetProgramiv(ID, GL_LINK_STATUS, &p_success);
//	if (!p_success)
//	{
//#ifdef _DEBUG
//		char p_infoLog[512];
//		glGetProgramInfoLog(ID, 512, nullptr, p_infoLog);
//		std::cout << "error in ID: " << p_infoLog << std::endl;
//		char a;
//		std::cin >> a;
//#endif
//		exit(-1);
//	}
//
//	/* we don't need them anymore */
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	glDeleteShader(geometryShader);
//}

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
void OGLShader::LoadShader(const char* vert_source, const char* frag_source)
{

	/* compile vertex (points) shader */
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vert_source, nullptr);
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
	glShaderSource(fragmentShader, 1, &frag_source, nullptr);
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

}  // end namespace AA