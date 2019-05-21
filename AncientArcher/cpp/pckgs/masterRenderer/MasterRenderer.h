#pragma once
#include "../monospaceFont/TextRenderer.h"
#include "../primatives/PrimativeRenderer.h"
#include "../skybox/SkyboxRenderer.h"
#include "../firstPersonPlayer/Player.h"
#include <vector>
#include "../sideScrollerPlayer/SideScrollPlayer.h"
class MasterRenderer {
public:
  MasterRenderer();
  ~MasterRenderer();

  void enableGLBlend();
  void disableGLBlend();

  void enableGLDepthTest();
  void disableGLDepthTest();

  void update(PrimativeRenderer* primRen, SideScrollPlayer* playerRen, TextRenderer* textRen, SkyboxRenderer* skyRen, float deltaTime);

private:

};

