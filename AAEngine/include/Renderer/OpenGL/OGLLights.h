#pragma once
#include <sstream>
#include "OGLShader.h"
#include "../Lights.h"
namespace AA
{
/// static helper functions

// single directional light
static void setDirectionalLight(const DirectionalLight& light, const OGLShader& shader) {
	shader.use();

	std::string direction, ambient, diffuse, specular;
	ambient = diffuse = specular = direction = "directionalLight.";

	direction += "Direction";
	ambient += "Ambient";
	diffuse += "Diffuse";
	specular += "Specular";

	shader.setVec3(direction, light.Direction);
	shader.setVec3(ambient, light.Ambient);
	shader.setVec3(diffuse, light.Diffuse);
	//shader.setVec3(specular,  light.Specular);
}

// multi lights
static void setPointLight(const PointLight& light, const int which, const OGLShader& shader) {
	if (which >= MAXPOINTLIGHTS)
	{
#ifdef _DEBUG
		std::cout << "couldn't set PointLight light, too many\n";
#endif
		return;
	}

	if (which >= NUM_POINT_LIGHTS)
	{
#ifdef _DEBUG
		std::cout << "couldn't set PointLight light, not enough in use\n";
#endif
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

	shader.setVec3(position, light.Position);
	shader.setFloat(constant, light.Constant);
	shader.setFloat(linear, light.Linear);
	shader.setFloat(quadratic, light.Quadratic);
	shader.setVec3(ambient, light.Ambient);
	shader.setVec3(diffuse, light.Diffuse);
	shader.setVec3(specular, light.Specular);
}

static void setSpotLight(const SpotLight& light, const int which, const OGLShader& shader) {
	if (which >= MAXSPOTLIGHTS)
	{
#ifdef _DEBUG
		std::cout << "couldn't set SpotLight light, too many\n";
#endif
		return;
	}

	if (which >= NUM_SPOT_LIGHTS)
	{
#ifdef _DEBUG
		std::cout << "couldn't set SpotLight light, not enough in use\n";
#endif
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

	shader.setVec3(pos, light.Position);
	shader.setFloat(cutoff, light.CutOff);
	shader.setFloat(ocutoff, light.OuterCutOff);
	shader.setVec3(direction, light.Direction);
	shader.setFloat(linear, light.Linear);
	shader.setFloat(quadrat, light.Quadratic);
	shader.setVec3(ambient, light.Ambient);
	shader.setVec3(diffuse, light.Diffuse);
	shader.setVec3(specular, light.Specular);
}

} // end namespace AA