#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <unordered_map>
namespace AA
{
struct MeshDrawInfo   // NodeDrawInfo a better name?
{
	MeshDrawInfo(uint32_t a, uint32_t elsize, std::unordered_map<uint32_t, std::string> t_ids, glm::mat4 trans);
	uint32_t vao = 0;
	//uint32_t vbo = 0;
	//uint32_t ebo = 0;
	std::unordered_map<uint32_t, std::string> textureDrawIds; // list of texture ids(uint32_t) and type(string) in mem
	//float shininess;
	//glm::vec4 specular;
	uint32_t numElements;
	//glm::mat4 transformation;
};
}  // end namespace AA
