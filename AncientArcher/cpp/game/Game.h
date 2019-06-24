#pragma once
#include "../pckgs/masterRenderer/MasterRenderer.h"
#include "../pckgs/primatives/PrimativeRenderer.h"
#include "../pckgs/spawner/Spawner.h"
#include "../pckgs/skybox/SkyboxRenderer.h"
#include "../pckgs/topDownPlayer/TopDownPlayer.h"

class Game {
public:

  TopDownPlayer* player;

  PrimativeRenderer* prims;

  Spawner* spawner;

  SkyboxRenderer* sky;

  MasterRenderer masterRenderer;

  Game();

  void mainLoop();

private:

  float lastFrame = 0.0f;

  float deltaTime = 0.0f;

};
