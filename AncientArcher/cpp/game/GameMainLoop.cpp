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

    sideScrollPlayer->processMovement(deltaTime);

    if (spawnedEnemies->getAliveCount() > 0)
    {
      spawnedEnemies->getEntity()->moveBy(glm::vec3(-2.0f * deltaTime, 0.f, 0.f));

      // Collision Player & Enemies
        // uses SideScrollPlayer, Spawner, & Camera
      static float collisionTimeOut = 0.0000f;
      if (collisionTimeOut > 0.0001f) {
        collisionTimeOut -= deltaTime;
      }
      if (collisionTimeOut < 0.0001f)
      {
        if (sideScrollPlayer->getPlayerSwordCollider() &&
          mearly::AABB_vs_AABB_3D(sideScrollPlayer->getPlayerSwordCollider()->impasse,
            spawnedEnemies->getCollider()->impasse))
        {
          spawnedEnemies->takeHit(sideScrollPlayer->getAttackDamage());
          spawnedEnemies->getEntity()->moveBy(glm::vec3(3.5, 0.f, 0.f));
          playswordswingsound();
          collisionTimeOut = 0.6f;
        }
        else if (mearly::AABB_vs_AABB_3D(sideScrollPlayer->getPlayerCollider()->impasse,
          spawnedEnemies->getCollider()->impasse))
        {
          sideScrollPlayer->takeHit(spawnedEnemies->getDamage());
          sideScrollPlayer->getEntity()->moveBy(glm::vec3(-3.5f, 0.f, 0.f));  //knock player back
          camera.Position.x -= 3.5f;   // knock back camera
          playpunchsound();
          collisionTimeOut = 0.1f;
        }
        else
        {
          collisionTimeOut = 0.f;
        }
      }
    }

    sideScrollPlayer->updateAttackTimer(deltaTime);

    spawnedEnemies->checkAndSpawn(deltaTime);

    masterRenderer.update(primativeRenderer, sideScrollPlayer, spawnedEnemies, healthBar, skyboxRenderer, deltaTime);

    glfwPollEvents();
  }
}