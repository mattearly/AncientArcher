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
  for (int i = 0; i < 500; i++) {
    e = new Entity(
      ENTITYTYPE::BOX,
      glm::vec3(i + 3, mearly::NTKR(-0.50f, 0.20f), i + 3),
      glm::vec3(2.0f, 1.0f, 2.0f),
      lightBricksTexID,
      true
    );
    primativeRenderer->addToPrimativeEntities(*e);
  }
  delete e;

  // ---- LOAD SKYBOX ---- //s
  std::vector<std::string> skyboxFiles =
  {
  "../AncientArcher/cpp/pckgs/skybox/dark/skybox_right.png",
  "../AncientArcher/cpp/pckgs/skybox/dark/skybox_left.png",
  "../AncientArcher/cpp/pckgs/skybox/dark/skybox_top.png",
  "../AncientArcher/cpp/pckgs/skybox/dark/skybox_bottom.png",
  "../AncientArcher/cpp/pckgs/skybox/dark/skybox_front.png",
  "../AncientArcher/cpp/pckgs/skybox/dark/skybox_back.png"
  };

  skyboxRenderer = new SkyboxRenderer(skyboxFiles);

}
