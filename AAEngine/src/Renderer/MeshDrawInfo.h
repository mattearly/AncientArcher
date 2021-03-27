#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>
#include <unordered_map>
namespace AA
{
struct MeshDrawInfo   // NodeDrawInfo a better name?
{
	MeshDrawInfo(uint32_t a, uint32_t elsize, std::unordered_map<uint32_t, 
		std::string> t_ids, glm::mat4 trans);
	uint32_t vao = 0;
	std::unordered_map<uint32_t, std::string> textureDrawIds; // list of texture ids(uint32_t) and type(string) in mem
	float shininess;
	uint32_t numElements;
	glm::mat4 transformation;
};
}  // end namespace AA
