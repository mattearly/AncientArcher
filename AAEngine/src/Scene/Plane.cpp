#include "Plane.h"
#include "../Renderer/LoadPlane.h"
#include "../Renderer/OpenGL/OGLGraphics.h"
#include "../Core.h"
#include "../Renderer/MeshDrawInfo.h"
#include <glm/glm.hpp>
namespace AA {
Plane::Plane() {
  meshdrawinfo = new MeshDrawInfo(load_plane(), 12, std::unordered_map<u32, string>(), 0, glm::mat4(1));
  shadertype = SHADERTYPE::DIFF;
}

void Plane::Draw() {
  OGLGraphics::SetCullFace(true);
  OGLGraphics::Render(*meshdrawinfo, shadertype);
  OGLGraphics::SetCullFace(false);
}

}  // end namespace AA
