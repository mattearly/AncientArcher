#pragma once
#include "../pckgs/masterRenderer/MasterRenderer.h"
#include "../pckgs/primatives/PrimativeRenderer.h"
#include "../pckgs/spawner/Spawner.h"
#include "../pckgs/firstPersonPlayer/FirstPersonPlayer.h"
#include "../pckgs/skybox/SkyboxRenderer.h"

class Game {
public:

  FirstPersonPlayer* player;

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
