#include <AAEngine.h>
#include "Game.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Game::Game() {

  // ----------- GRAPHICS CARD INFORMATION --------- //

  int __textures_allowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";

  int __totalTexturesAllowed = 0;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
  std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";


  // Player and Camera
  player = new Player(100.0f);  // should be first to instantiate camera I think


  // ---------- PRELOAD ENVIRORNMENT DETAILS ---------- //
  primativeRenderer = new PrimativeRenderer();

  TextureLoader texLoader;

  unsigned int lightBricksTexID = texLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/light_bricks.png");

  // -------- LOAD WORLD --------- //
  Entity* e;
  for (int i = 0; i < 15; i++)
  {
    for (int j = 0; j < 15; j++) 
    {
      e = new Entity(
        ENTITYTYPE::BOX,
        glm::vec3(i, mearly::NTKR(-5.25f, -5.19f), j),
        glm::vec3(1.0, 5.4f, 1.0),
        lightBricksTexID,
        true
      );
      primativeRenderer->addToPrimativeEntities(*e);
    }
  }

  skyboxRenderer = new SkyboxRenderer();


  // ------------ SET FLOOR ------------ //
  //float planeCollisionThickness = 0.18f;
  //float currentGroundLevel = -0.02f;
  //int groundTextureId = 2;
  //for (int i = 0; i < 4; i++) {
  //  Entity floor1(
  //    ENTITYTYPE::PLANE,
  //    glm::vec3(i * ENGINE_LOGIC_CHECKING_DISTANCE, currentGroundLevel, ENGINE_LOGIC_CHECKING_DISTANCE / 2.0f),
  //    glm::vec3(ENGINE_LOGIC_CHECKING_DISTANCE, planeCollisionThickness, ENGINE_LOGIC_CHECKING_DISTANCE),
  //    groundTextureId,
  //    true
  //  );
  //  entities.push_back(floor1);

  //  Entity floor2(
  //    ENTITYTYPE::PLANE,
  //    glm::vec3(i * ENGINE_LOGIC_CHECKING_DISTANCE, currentGroundLevel, ENGINE_LOGIC_CHECKING_DISTANCE * 1.5f),
  //    glm::vec3(ENGINE_LOGIC_CHECKING_DISTANCE, planeCollisionThickness, ENGINE_LOGIC_CHECKING_DISTANCE),
  //    groundTextureId,
  //    true
  //  );
  //  entities.push_back(floor2);

  //  Entity floor3(
  //    ENTITYTYPE::PLANE,
  //    glm::vec3(i * ENGINE_LOGIC_CHECKING_DISTANCE, currentGroundLevel, ENGINE_LOGIC_CHECKING_DISTANCE * 2.5f),
  //    glm::vec3(ENGINE_LOGIC_CHECKING_DISTANCE, planeCollisionThickness, ENGINE_LOGIC_CHECKING_DISTANCE),
  //    groundTextureId,
  //    true
  //  );
  //  entities.push_back(floor3);

  //  Entity floor4(
  //    ENTITYTYPE::PLANE,
  //    glm::vec3(i * ENGINE_LOGIC_CHECKING_DISTANCE, currentGroundLevel, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5f),
  //    glm::vec3(ENGINE_LOGIC_CHECKING_DISTANCE, planeCollisionThickness, ENGINE_LOGIC_CHECKING_DISTANCE),
  //    groundTextureId,
  //    true
  //  );
  //  entities.push_back(floor4);
  //}

  //// ------------ PLACE RANDOMIZED BOXES ------------ //
  //bool allowRandomBoxesToOverlap = false;
  //int countTotalCollisions = 0;
  //for (int i = 0; i < 3000; i++) {

  //  Entity* e;

  //  if (i % 3 == 0) {
  //    e = new Entity(
  //      ENTITYTYPE::BOX,
  //      glm::vec3(mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5),
  //        mearly::NTKR(7.01f, 15.0f), mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5)),
  //      glm::vec3(mearly::NTKR(2.5f, 4.5f), mearly::NTKR(0.3f, 6.5f), mearly::NTKR(2.5f, 4.5f)),
  //      mearly::NTKR(0, 31),
  //      true
  //    );
  //  }
  //  else if (i % 3 == 1) {
  //    e = new Entity(
  //      ENTITYTYPE::BOX,
  //      glm::vec3(mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5), mearly::NTKR(2.01f, 8.0f), mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5)),
  //      glm::vec3(mearly::NTKR(0.5f, 8.0f), mearly::NTKR(0.5f, 5.0f), mearly::NTKR(0.5f, 8.0f)),
  //      mearly::NTKR(0, 31),
  //      true
  //    );
  //  }
  //  else {
  //    e = new Entity(
  //      ENTITYTYPE::BOX,
  //      glm::vec3(mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5), 1.08f, mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5)),
  //      glm::vec3(2.f, 2.f, 2.f),
  //      mearly::NTKR(0, 31),
  //      true
  //    );
  //  }

  //  for (auto const& f : entities) {
  //    if (mearly::AABB_vs_AABB_3D(e->collider->impasse, f.collider->impasse)) {
  //      allowRandomBoxesToOverlap = true;
  //      countTotalCollisions++;
  //      break;
  //    }
  //  }

  //  if (!allowRandomBoxesToOverlap) {
  //    entities.push_back(*e);
  //  }
  //  else {
  //    i--;
  //    allowRandomBoxesToOverlap = false; //reset for next run;
  //  }

  //}
  //std::cout << "total collisions: " << countTotalCollisions << "\ntotal entities: " << entities.size() << "\n";

}
