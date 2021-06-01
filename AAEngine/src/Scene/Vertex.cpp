
#include "Vertex.h"
namespace AA {
Vertex::Vertex(vec3 pos, vec3 norm) {
  Position = pos;
  Normal = norm;
}
Vertex::Vertex(vec3 pos, vec3 norm, vec2 uv) {
  Position = pos;
  Normal = norm;
  TexCoords = uv;
}
} // end namespace AA
