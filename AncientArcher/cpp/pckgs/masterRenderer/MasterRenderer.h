//#pragma once
//#include "../primatives/PrimativeRenderer.h"
//#include "../skybox/SkyboxRenderer.h"
//#include "../sideScrollerPlayer/SideScrollPlayer.h"
//#include "../spawner/Spawner.h"
//#include "../GUI/HealthBar.h"
//#include <vector>
//#include <glm/glm.hpp>
//#include "../firstPersonPlayer/FirstPersonPlayer.h"
//#include "../topDownPlayer/TopDownPlayer.h"
//
//
//class MasterRenderer {
//public:
//  MasterRenderer();
//  ~MasterRenderer();
//
//  void enableGLBlend();
//  void disableGLBlend();
//
//  void enableGLDepthTest();
//  void disableGLDepthTest();
//
//  void update(float deltaTime, FirstPersonPlayer *fpplayer, PrimativeRenderer *prims, SkyboxRenderer *sky);
//  void update(float deltaTime, TopDownPlayer* tdp, PrimativeRenderer* prims, SkyboxRenderer* sky);
//  void update(float deltaTime, PrimativeRenderer* prims, SkyboxRenderer* sky);
//
//  void update(PrimativeRenderer* primRen, SideScrollPlayer* playerRen,
//    Spawner* enemyRen, HealthBar* healthRen, SkyboxRenderer* skyRen,
//    float deltaTime);
//
//private:
//  ///< color of the background if a skybox is not being rendered.
//  glm::vec3 _backgroundColor;
//
//};
//
