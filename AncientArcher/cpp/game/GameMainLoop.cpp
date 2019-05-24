#include <AAEngine.h>
#include "Game.h"
#include "../pckgs/firstPersonPlayer/Movement.h"
#include "../pckgs/masterRenderer/MasterRenderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
extern Movement movedir;
extern Camera camera;
void Game::mainLoop() {

  toggleAmbientWindyNight();

  static float currentFrame;

  while (!glfwWindowShouldClose(display.window)) {
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    sideScrollPlayer->processControls();

    spawnedEnemies->checkAndSpawn(deltaTime);


    // this collision is all a hack, don't take it too seriously hahahaha
    static float collisionCheckTime = 0.f;
    collisionCheckTime += deltaTime;
    if (spawnedEnemies->getAliveCount() > 0 && sideScrollPlayer->getSwordCollider() && collisionCheckTime > 0.2f)
    {
      if ( mearly::AABB_vs_AABB_3D( sideScrollPlayer->getSwordCollider()->impasse, 
                                    spawnedEnemies->getCollider()->impasse ) )
      {
        //sideScrollPlayer->getEntity()->moveBy(glm::vec3(-2.5f, 0.f, 0.f));
        //camera.Position.x -= 2.5f;   // hack to keep the cam in place with the player
        //spawnedEnemies->getEntity()->moveBy(glm::vec3(2.5f, 0.f, 0.f));
        spawnedEnemies->despawn();
        playswordswingsound();
      }
      collisionCheckTime = 0.f;
    }

    sideScrollPlayer->attackTimer(deltaTime);

    masterRenderer.update(primativeRenderer, sideScrollPlayer, spawnedEnemies, textRenderer, skyboxRenderer, deltaTime);

    glfwPollEvents();
  }
}