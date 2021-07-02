#pragma once
#include "../Base/UniqueInstance.h"
#include "../../include/AncientArcher/ShaderType.h"
#include "../Renderer/MeshDrawInfo.h"
namespace AA {
class Plane : public UniqueInstance {
public:
  Plane();
  void Draw();
  SHADERTYPE shadertype;
  MeshDrawInfo *meshdrawinfo;
private:
};
}  // end namespace AA
