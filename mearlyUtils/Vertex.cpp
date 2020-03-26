///////////////////VERTEX CONSTRUCTOR///////////////////////////
#include "Vertex.h"

namespace mearly {
Vertex::Vertex(glm::vec4 pos, glm::vec4 norms, glm::vec2 texcoords) noexcept
  : Position(pos), Normal(norms), TexCoords(texcoords) {}

Vertex::Vertex(glm::vec4 pos, glm::vec4 norms, glm::vec4 colors, glm::vec2 texcoords) noexcept
  : Position(pos), Normal(norms), Color(colors), TexCoords(texcoords) {}

} // end namespace mearly