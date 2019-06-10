#pragma once
#include "../pckgs/masterRenderer/MasterRenderer.h"
#include "../pckgs/primatives/PrimativeRenderer.h"
#include "../pckgs/firstPersonPlayer/FirstPersonPlayer.h"
#include "../pckgs/skybox/SkyboxRenderer.h"

class Game {
public:

  FirstPersonPlayer* player;
  
  PrimativeRenderer* prims;
  
  SkyboxRenderer* sky;

  MasterRenderer masterRenderer;

  Game();

  void mainLoop();

private:

  float lastFrame = 0.0f;

  float deltaTime = 0.0f;

  float elapsedTime = 0.0f;

	  bool timeTrigger = true;

};
