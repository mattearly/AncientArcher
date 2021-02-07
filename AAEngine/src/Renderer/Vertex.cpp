
#include "../../include/Renderer/Vertex.h"
namespace AA
{
Vertex::Vertex(glm::vec3 pos, glm::vec2 texcoords, glm::vec3 norms) noexcept
	: Position(pos), TexCoords(texcoords), Normal(norms) {}

//Vertex::Vertex(glm::vec4 pos, glm::vec4 norms, glm::vec4 colors, glm::vec2 texcoords) noexcept
//	: Position(pos), Normal(norms), Color(colors), TexCoords(texcoords) {}
} // end namespace AA
