#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../globals.h"
#include "../controls/Controls.h"
#include "../util/mearly.h"
#include <iostream>

Display display;
Shader shader("../AncientArcher/res/shaders/vertex.shader", "../AncientArcher/res/shaders/fragment.shader");
Camera camera;
Controls controls;
Pickups pickups;
Lighting lighting;
TextureBank texBank;
std::vector<Entity> entities;

Game::Game() {

  int __textures_allowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  std::cout << "Your Graphics Card allows " << __textures_allowed << " textures at the same time\n";

  //player = new Player();   //default character
  player = new Player(3.f, 4.f, 100.f);

  /* load 32 textures */
  texBank.loadTexture("../AncientArcher/res/texture/00-pixelized_grass.png");
  texBank.loadTexture("../AncientArcher/res/texture/01.png");
  texBank.loadTexture("../AncientArcher/res/texture/02-pixelized_wood.png");
  texBank.loadTexture("../AncientArcher/res/texture/03-footsteps.png");
  texBank.loadTexture("../AncientArcher/res/texture/04-pixelized_portal.png");
  texBank.loadTexture("../AncientArcher/res/texture/05-pixelized_darkstone.png");
  texBank.loadTexture("../AncientArcher/res/texture/06-pixelized_water_droplet.png");
  texBank.loadTexture("../AncientArcher/res/texture/07-pixelized_gravel.png");
  texBank.loadTexture("../AncientArcher/res/texture/08-pixelized_water.png");
  texBank.loadTexture("../AncientArcher/res/texture/09-bricks_light.png");
  texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png");
  texBank.loadTexture("../AncientArcher/res/texture/11-pixelized_darkwood.png");
  texBank.loadTexture("../AncientArcher/res/texture/12-pickup_speedboost.png");
  texBank.loadTexture("../AncientArcher/res/texture/13-pixelized_snow.png");
  texBank.loadTexture("../AncientArcher/res/texture/14-maze_metal.png");
  texBank.loadTexture("../AncientArcher/res/texture/15-pickup_attackboost.png");
  texBank.loadTexture("../AncientArcher/res/texture/16-cracked_rock.png");
  texBank.loadTexture("../AncientArcher/res/texture/17-stone_texture.png");
  texBank.loadTexture("../AncientArcher/res/texture/18-black_schist_rock.png");
  texBank.loadTexture("../AncientArcher/res/texture/19-red_brick.png");
  texBank.loadTexture("../AncientArcher/res/texture/20-blue_paint.png");
  texBank.loadTexture("../AncientArcher/res/texture/21-cracked_mud.png");
  texBank.loadTexture("../AncientArcher/res/texture/22-fern_leaves.png");
  texBank.loadTexture("../AncientArcher/res/texture/23-cracked_mud_light.png");
  texBank.loadTexture("../AncientArcher/res/texture/24-hardwood_floor.png");
  texBank.loadTexture("../AncientArcher/res/texture/25-leather_dark.png");
  texBank.loadTexture("../AncientArcher/res/texture/26-rock_with_holes.png");
  texBank.loadTexture("../AncientArcher/res/texture/27-skull_mud.png");
  texBank.loadTexture("../AncientArcher/res/texture/28-plastic_grip.png");
  texBank.loadTexture("../AncientArcher/res/texture/29-colorful_window.png");
  texBank.loadTexture("../AncientArcher/res/texture/30-water_refractions.png");
  texBank.loadTexture("../AncientArcher/res/texture/31-water_top.png");


  lighting.setConstantLight();

  lighting.addPointLight(*camera.getPosition());

  renderer.enableGLDepthTest();

  camera.updateProjectionMatrix();

  float width = world_width + 1.f;

  for (int i = 0; i < 3; i++) {
    Entity floor(
      // TYPE
      ENTITYTYPE::PLANE,
      // LOCATION 
      glm::vec3(i * width, 0.0f, width / 2),
      // SIZE
      glm::vec3(width, 0.5f, width),
      // TEXTURE ID
      mearly::NTKR(0, 31),
      // COLLISION?
      false
    );
    entities.push_back(floor);
  }

  for (int i = 0; i < 3; i++) {
    Entity floor(
      // TYPE
      ENTITYTYPE::PLANE,
      // LOCATION 
      glm::vec3(i * width, -0.01f, width * 1.5 - .5),
      // SIZE
      glm::vec3(width, 0.5f, width),
      // TEXTURE ID
      mearly::NTKR(0, 31),
      // COLLISION?
      false
    );
    entities.push_back(floor);
  }

  for (int i = 0; i < 3; i++) {
    Entity floor(
      // TYPE
      ENTITYTYPE::PLANE,
      // LOCATION 
      glm::vec3(i * width, 0.0f, width * 2.5 - .5f),
      // SIZE
      glm::vec3(width, 1.0f, width),
      // TEXTURE ID
      mearly::NTKR(0, 31),
      // COLLISION?
      false
    );
    entities.push_back(floor);
  }

  // many test blocks
  for (int i = 0; i < 500; i++) {

    if (i % 3 == 0) {
      Entity e(
        ENTITYTYPE::SQUARE,
        glm::vec3(mearly::NTRK(3.f, 100.f), mearly::NTRK(0.0f, 4.0f), mearly::NTRK(3.f, 120.f)),
        glm::vec3(2.f, 2.f, 2.f),
        mearly::NTKR(0, 31),
        true
      );
      entities.push_back(e);
    }
    else {
      Entity e(
        ENTITYTYPE::SQUARE,
        glm::vec3(mearly::NTRK(3.f, 100.f), 1.0f, mearly::NTRK(3.f, 120.f)),
        glm::vec3(2.f, 2.f, 2.f),
        mearly::NTKR(0, 31),
        true
      );
      entities.push_back(e);
    }

  }
}

  Game::~Game() {

  }
