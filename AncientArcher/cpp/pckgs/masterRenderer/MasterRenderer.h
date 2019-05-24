#pragma once
#include "../primatives/PrimativeRenderer.h"
#include "../skybox/SkyboxRenderer.h"
#include "../firstPersonPlayer/Player.h"
#include "../sideScrollerPlayer/SideScrollPlayer.h"
#include "../spawner/Spawner.h"
#include "../GUI/HealthBar.h"
#include <vector>

class MasterRenderer {
public:
  MasterRenderer();
  ~MasterRenderer();

  void enableGLBlend();
  void disableGLBlend();

  void enableGLDepthTest();
  void disableGLDepthTest();

  void update(PrimativeRenderer* primRen, SideScrollPlayer* playerRen, 
    Spawner* enemyRen, HealthBar* healthRen, SkyboxRenderer* skyRen, 
    float deltaTime);

private:

};

