#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "globals.h"
#include "Controls.h"
#include "mearly.h"
#include <iostream>

Display display;
Shader shader("../AncientArcher/resource/shader_vertex.glsl", "../AncientArcher/resource/shader_fragment.glsl");
Lighting lighting;
TextureBank texBank;
std::vector<Entity> entities;

Game::Game() {


	// ----------- GRAPHICS CARD INFORMATION --------- //
	int __textures_allowed = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
	std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";

	int __totalTexturesAllowed = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
	std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";


	// ---------- PRELOAD ENVIRORNMENT DETAILS ---------- //
	lighting.setConstantLight();
	player = new Player(100.0f);
	texBank.loadTexture("../AncientArcher/resource/00-pixelized_grass.png");
	texBank.loadTexture("../AncientArcher/resource/01.png");
	texBank.loadTexture("../AncientArcher/resource/02-pixelized_wood.png");
	texBank.loadTexture("../AncientArcher/resource/03-footsteps.png");
	texBank.loadTexture("../AncientArcher/resource/04-pixelized_portal.png");
	texBank.loadTexture("../AncientArcher/resource/05-pixelized_darkstone.png");
	texBank.loadTexture("../AncientArcher/resource/06-pixelized_water_droplet.png");
	texBank.loadTexture("../AncientArcher/resource/07-pixelized_gravel.png");
	texBank.loadTexture("../AncientArcher/resource/08-pixelized_water.png");
	texBank.loadTexture("../AncientArcher/resource/09-bricks_light.png");
	texBank.loadTexture("../AncientArcher/resource/10-pixelized_mud.png");
	texBank.loadTexture("../AncientArcher/resource/11-pixelized_darkwood.png");
	texBank.loadTexture("../AncientArcher/resource/12-pickup_speedboost.png");
	texBank.loadTexture("../AncientArcher/resource/13-pixelized_snow.png");
	texBank.loadTexture("../AncientArcher/resource/14-maze_metal.png");
	texBank.loadTexture("../AncientArcher/resource/15-pickup_attackboost.png");
	texBank.loadTexture("../AncientArcher/resource/16-cracked_rock.png");
	texBank.loadTexture("../AncientArcher/resource/17-stone_texture.png");
	texBank.loadTexture("../AncientArcher/resource/18-black_schist_rock.png");
	texBank.loadTexture("../AncientArcher/resource/19-red_brick.png");
	texBank.loadTexture("../AncientArcher/resource/20-blue_paint.png");
	texBank.loadTexture("../AncientArcher/resource/21-cracked_mud.png");
	texBank.loadTexture("../AncientArcher/resource/22-fern_leaves.png");
	texBank.loadTexture("../AncientArcher/resource/23-cracked_mud_light.png");
	texBank.loadTexture("../AncientArcher/resource/24-hardwood_floor.png");
	texBank.loadTexture("../AncientArcher/resource/25-leather_dark.png");
	texBank.loadTexture("../AncientArcher/resource/26-rock_with_holes.png");
	texBank.loadTexture("../AncientArcher/resource/27-skull_mud.png");
	texBank.loadTexture("../AncientArcher/resource/28-plastic_grip.png");
	texBank.loadTexture("../AncientArcher/resource/29-colorful_window.png");
	texBank.loadTexture("../AncientArcher/resource/30-water_refractions.png");
	texBank.loadTexture("../AncientArcher/resource/31-water_top.png");


	// ------------ SET FLOOR ------------ //
	float planeCollisionThickness = 0.18f;
	float currentGroundLevel = -0.02f;
	int groundTextureId = 2;
	for (int i = 0; i < 4; i++) {
		Entity floor1(
			ENTITYTYPE::PLANE,                                                                          /* type */
			glm::vec3(i * ENGINE_LOGIC_CHECKING_DISTANCE, currentGroundLevel, ENGINE_LOGIC_CHECKING_DISTANCE / 2.0f), /* location */
			glm::vec3(ENGINE_LOGIC_CHECKING_DISTANCE, planeCollisionThickness, ENGINE_LOGIC_CHECKING_DISTANCE),       /* size */
			groundTextureId,                                                                            /* texture id */
			true               /* give collision */
		);
		entities.push_back(floor1);

		Entity floor2(
			ENTITYTYPE::PLANE,
			glm::vec3(i * ENGINE_LOGIC_CHECKING_DISTANCE, currentGroundLevel, ENGINE_LOGIC_CHECKING_DISTANCE * 1.5f),
			glm::vec3(ENGINE_LOGIC_CHECKING_DISTANCE, planeCollisionThickness, ENGINE_LOGIC_CHECKING_DISTANCE),
			groundTextureId,
			true
		);
		entities.push_back(floor2);

		Entity floor3(
			// TYPE
			ENTITYTYPE::PLANE,
			// LOCATION 
			glm::vec3(i * ENGINE_LOGIC_CHECKING_DISTANCE, currentGroundLevel, ENGINE_LOGIC_CHECKING_DISTANCE * 2.5f),
			// SIZE
			glm::vec3(ENGINE_LOGIC_CHECKING_DISTANCE, planeCollisionThickness, ENGINE_LOGIC_CHECKING_DISTANCE),
			// TEXTURE ID
			groundTextureId,
			// COLLISION?
			true
		);
		entities.push_back(floor3);

		Entity floor4(
			// TYPE
			ENTITYTYPE::PLANE,
			// LOCATION 
			glm::vec3(i * ENGINE_LOGIC_CHECKING_DISTANCE, currentGroundLevel, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5f),
			// SIZE
			glm::vec3(ENGINE_LOGIC_CHECKING_DISTANCE, planeCollisionThickness, ENGINE_LOGIC_CHECKING_DISTANCE),
			// TEXTURE ID
			groundTextureId,
			// COLLISION?
			true
		);
		entities.push_back(floor4);
	}

	// ------------ PLACE RANDOMIZED BOXES ------------ //
	bool allowRandomBoxesToOverlap = false; 
	int countTotalCollisions = 0;
	for (int i = 0; i < 2000; i++) {

		Entity* e;

		if (i % 3 == 0) {
			e = new Entity(
				ENTITYTYPE::SQUARE,
				glm::vec3(mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5),
					mearly::NTKR(7.01f, 15.0f), mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5)),
				glm::vec3(mearly::NTKR(2.5f, 4.5f), mearly::NTKR(0.3f, 6.5f), mearly::NTKR(2.5f, 4.5f)),
				mearly::NTKR(0, 31),
				true
			);
		}
		else if (i % 3 == 1) {
			e = new Entity(
				ENTITYTYPE::SQUARE,
				glm::vec3(mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5), mearly::NTKR(2.01f, 8.0f), mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5)),
				glm::vec3(mearly::NTKR(0.5f, 8.0f), mearly::NTKR(0.5f, 5.0f), mearly::NTKR(0.5f, 8.0f)),
				mearly::NTKR(0, 31),
				true
			);
		}
		else {
			e = new Entity(
				ENTITYTYPE::SQUARE,
				glm::vec3(mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5), 1.08f, mearly::NTKR(3.f, ENGINE_LOGIC_CHECKING_DISTANCE * 3.5)),
				glm::vec3(2.f, 2.f, 2.f),
				mearly::NTKR(0, 31),
				true
			);
		}

		for (auto const& f : entities) {
			if (mearly::AABB_vs_AABB_3D(e->collider->impasse, f.collider->impasse)) {
				allowRandomBoxesToOverlap = true;
				countTotalCollisions++;
				break;
			}
		}

		if (!allowRandomBoxesToOverlap) {
			entities.push_back(*e);
		}
		else {
			i--;
			allowRandomBoxesToOverlap = false; //reset for next run;
		}

	}
	std::cout << "total collisions: " << countTotalCollisions << "\ntotal entities: " << entities.size() << "\n";


}
