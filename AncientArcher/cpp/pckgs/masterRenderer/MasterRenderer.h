#pragma once
#include "../monospaceFont/TextRenderer.h"
#include "../primatives/PrimativeRenderer.h"
#include "../skybox/SkyboxRenderer.h"
#include "../firstPersonPlayer/Player.h"
class MasterRenderer {
public:
  MasterRenderer();
  ~MasterRenderer();

  void enableGLBlend();
  void disableGLBlend();

  void enableGLDepthTest();
  void disableGLDepthTest();

  void update(PrimativeRenderer* primRen, TextRenderer* textRen, SkyboxRenderer* skyRen, float deltaTime);

private:

};

