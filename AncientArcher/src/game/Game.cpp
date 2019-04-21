#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../globals.h"
#include "../player/Controls.h"
#include "../util/mearly.h"
#include <iostream>

Display display;
Shader shader("../AncientArcher/res/shaders/vertex.shader", "../AncientArcher/res/shaders/fragment.shader");
Lighting lighting;
TextureBank texBank;
std::vector<Entity> entities;

Game::Game() {

  int __textures_allowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  std::cout << "FYI:\nMax textures per shader:  " << __textures_allowed << "\n";

  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  std::cout << "Max combined textures:  " << __textures_allowed << "\n";

  player = new Player(100.0f);

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

  float ground_plane_thickness = 0.09f;
  float ground_plane_level = -0.01f;
  int floor_texture_id = 2;

  for (int i = 0; i < 4; i++) {
    Entity floor1(
      // TYPE
      ENTITYTYPE::PLANE,
      // LOCATION 
      glm::vec3(i * logic_checking_distance, ground_plane_level, logic_checking_distance / 2.f),
      // SIZE
      glm::vec3(logic_checking_distance, ground_plane_thickness, logic_checking_distance),
      // TEXTURE ID
      floor_texture_id,
      // COLLISION?
      true
    );
    entities.push_back(floor1);

    Entity floor2(
      // TYPE
      ENTITYTYPE::PLANE,
      // LOCATION 
      glm::vec3(i * logic_checking_distance, ground_plane_level, logic_checking_distance * 1.5f),
      // SIZE
      glm::vec3(logic_checking_distance, ground_plane_thickness, logic_checking_distance),
      // TEXTURE ID
      floor_texture_id,
      // COLLISION?
      true
    );
    entities.push_back(floor2);

    Entity floor3(
      // TYPE
      ENTITYTYPE::PLANE,
      // LOCATION 
      glm::vec3(i * logic_checking_distance, ground_plane_level, logic_checking_distance * 2.5f),
      // SIZE
      glm::vec3(logic_checking_distance, ground_plane_thickness, logic_checking_distance),
      // TEXTURE ID
      floor_texture_id,
      // COLLISION?
      true
    );
    entities.push_back(floor3);

    Entity floor4(
      // TYPE
      ENTITYTYPE::PLANE,
      // LOCATION 
      glm::vec3(i * logic_checking_distance, ground_plane_level, logic_checking_distance * 3.5f),
      // SIZE
      glm::vec3(logic_checking_distance, ground_plane_thickness, logic_checking_distance),
      // TEXTURE ID
      floor_texture_id,
      // COLLISION?
      true
    );
    entities.push_back(floor4);
  }

  bool collide = false;
  int total_collisions = 0;
  // many test blocks
  for (int i = 0; i < 2000; i++) {

    Entity *e;

    if (i % 3 == 0) {
      e = new Entity(
        ENTITYTYPE::SQUARE,
        glm::vec3(mearly::NTKR(3.f, logic_checking_distance * 3.5), mearly::NTKR(7.01f, 15.0f), mearly::NTKR(3.f, logic_checking_distance * 3.5)),
        glm::vec3(mearly::NTKR(2.5f, 4.5f), mearly::NTKR(0.3f, 6.5f), mearly::NTKR(2.5f, 4.5f)),
        mearly::NTKR(0, 31),
        true
      );
    }
    else if (i % 3 == 1) {
      e = new Entity(
        ENTITYTYPE::SQUARE,
        glm::vec3(mearly::NTKR(3.f, logic_checking_distance * 3.5), mearly::NTKR(2.01f, 8.0f), mearly::NTKR(3.f, logic_checking_distance * 3.5)),
        glm::vec3(mearly::NTKR(0.5f, 8.0f), mearly::NTKR(0.5f, 5.0f), mearly::NTKR(0.5f, 8.0f)),
        mearly::NTKR(0, 31),
        true
      );
    }
    else {
      e = new Entity(
        ENTITYTYPE::SQUARE,
        glm::vec3(mearly::NTKR(3.f, logic_checking_distance * 3.5), 1.08f, mearly::NTKR(3.f, logic_checking_distance * 3.5)),
        glm::vec3(2.f, 2.f, 2.f),
        mearly::NTKR(0, 31),
        true
      );
    }

    for (auto const & f : entities) {
      if (mearly::AABB_vs_AABB_3D(e->collider->impasse, f.collider->impasse)) {
        collide = true;
        total_collisions++;
        break;
      }
    }

    if (!collide) {
      entities.push_back(*e);
    }
    else {
      i--;
      collide = false; //reset for next run;
    }

  }
  std::cout << "total collisions: " << total_collisions << "\ntotal entities: " << entities.size() << "\n";
}
