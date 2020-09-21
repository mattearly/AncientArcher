/*
Lights
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
#include <sstream>
#include <iostream>
#include "OGLShader.h"

namespace AA
{
const int MAXPOINTLIGHTS = 50;
const int MAXSPOTLIGHTS = 25;
static int NUM_POINT_LIGHTS = 0;
static int NUM_SPOT_LIGHTS = 0;

struct DirectionalLight
{
	glm::vec4 Direction, Ambient, Diffuse, Specular;
};

struct PointLight
{
	glm::vec4 Position;
	float Constant, Linear, Quadratic;
	glm::vec4 Ambient, Diffuse, Specular;
};

struct SpotLight
{
	glm::vec4 Position, Direction;
	float CutOff, OuterCutOff;
	float Constant, Linear, Quadratic;
	glm::vec4 Ambient, Diffuse, Specular;
};

// single directional lights
static void setDirectionalLight(const DirectionalLight& light, const OGLShader& shader)
{
	shader.use();

	std::string direction, ambient, diffuse, specular;
	ambient = diffuse = specular = direction = "directionalLight.";

	direction += "Direction";
	ambient += "Ambient";
	diffuse += "Diffuse";
	specular += "Specular";

	shader.setVec4(direction, light.Direction);
	shader.setVec4(ambient, light.Ambient);
	shader.setVec4(diffuse, light.Diffuse);
	//shader.setVec4(specular,  light.Specular);
}

// multi lights
static void setPointLight(const PointLight& light, const int which, const OGLShader& shader)
{
	if (which >= MAXPOINTLIGHTS)
	{
		std::cout << "couldn't set PointLight light, too many\n";
		return;
	}

	if (which >= NUM_POINT_LIGHTS)
	{
		std::cout << "couldn't set PointLight light, not enough in use\n";
		return;
	}

	shader.use();

	shader.setInt("NUM_POINT_LIGHTS", NUM_POINT_LIGHTS);

	std::string position, constant, linear, quadratic, ambient, diffuse, specular;
	constant = linear = quadratic = ambient = diffuse = specular = position = "pointLight[";

	std::stringstream ss;
	ss << which;

	position += ss.str();
	constant += ss.str();
	linear += ss.str();
	quadratic += ss.str();
	ambient += ss.str();
	diffuse += ss.str();
	specular += ss.str();

	position += "].";
	constant += "].";
	linear += "].";
	quadratic += "].";
	ambient += "].";
	diffuse += "].";
	specular += "].";

	position += "Position";
	constant += "Constant";
	linear += "Linear";
	quadratic += "Quadratic";
	ambient += "Ambient";
	diffuse += "Diffuse";
	specular += "Specular";

	shader.setVec4(position, light.Position);
	shader.setFloat(constant, light.Constant);
	shader.setFloat(linear, light.Linear);
	shader.setFloat(quadratic, light.Quadratic);
	shader.setVec4(ambient, light.Ambient);
	shader.setVec4(diffuse, light.Diffuse);
	shader.setVec4(specular, light.Specular);
}

static void setSpotLight(const SpotLight& light, const int which, const OGLShader& shader)
{
	if (which >= MAXSPOTLIGHTS)
	{
		std::cout << "couldn't set SpotLight light, too many\n";
		return;
	}

	if (which >= NUM_SPOT_LIGHTS)
	{
		std::cout << "couldn't set SpotLight light, not enough in use\n";
		return;
	}

	shader.use();

	shader.setInt("NUM_SPOT_LIGHTS", NUM_SPOT_LIGHTS);

	std::string pos, ambient, constant, cutoff, ocutoff, diffuse, direction, linear, quadrat, specular;

	ambient = constant = cutoff = ocutoff = diffuse = direction = linear = quadrat = specular = pos = "spotLight[";

	std::stringstream ss;
	ss << which;

	pos += ss.str();
	constant += ss.str();
	cutoff += ss.str();
	ocutoff += ss.str();
	direction += ss.str();
	linear += ss.str();
	quadrat += ss.str();
	ambient += ss.str();
	diffuse += ss.str();
	specular += ss.str();

	pos += "].";
	constant += "].";
	cutoff += "].";
	ocutoff += "].";
	direction += "].";
	linear += "].";
	quadrat += "].";
	ambient += "].";
	diffuse += "].";
	specular += "].";

	pos += "Position";
	constant += "Constant";
	cutoff += "CutOff";
	ocutoff += "OuterCutOff";
	direction += "Direction";
	linear += "Linear";
	quadrat += "Quadratic";
	ambient += "Ambient";
	diffuse += "Diffuse";
	specular += "Specular";

	shader.setVec4(pos, light.Position);
	shader.setFloat(cutoff, light.CutOff);
	shader.setFloat(ocutoff, light.OuterCutOff);
	shader.setVec4(direction, light.Direction);
	shader.setFloat(linear, light.Linear);
	shader.setFloat(quadrat, light.Quadratic);
	shader.setVec4(ambient, light.Ambient);
	shader.setVec4(diffuse, light.Diffuse);
	shader.setVec4(specular, light.Specular);
}
}  // end namespace AA