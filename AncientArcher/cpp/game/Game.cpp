#include "Game.h"
#include <CollisionHandler.h>
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <TextureLoader.h>
#include <Global.h>
#include <mearly.h>

//--- GLOBALS ---//
Display g_display(" MMO ", Display::MouseControlType::FPP);
//              Starting Position           YAW    PITCH  FOV
//Camera g_camera(glm::vec3(0.f, 1.0f, 0.f), 0, 0, 45.f);
Camera g_camera(glm::vec3(0.f, 1.0f, 0.f), -89.991f, 0, 75.f);
Lighting g_lighting;
//--- END GLOBALS ---//

Game::Game()
{
	int __textures_allowed = 0, __totalTexturesAllowed = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
	std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";
	std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";

	player = new FirstPersonPlayer();
	prims = new PrimativeRenderer();

  player->addPointLight(glm::vec3(0, 0, 0), prims->getShader());

	g_lighting.updateConstantLightAmbient(glm::vec3(.09, 0.07, 0.07));
	g_lighting.updateConstantLightDirection(glm::vec3(0.35, -0.75, 0.15));
	g_lighting.updateConstantLightDiffuse(glm::vec3(.80, .70, .74));
	//g_lighting.updateConstantLightSpecular(glm::vec3(.2, .2, .2));

	g_lighting.setConstantLight(prims->getShader());

  spawner = new Spawner();
  spawner->setPopulationCap(1);
  spawner->setTimeBetweenSpawns(2.f);

	TextureLoader tLoader;
	unsigned int texIDGrass = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/grass3.png");
	unsigned int texIDCrumblingRocks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/crumbling_rocks.png");
	unsigned int texIDDirt = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/dirt.png");
	unsigned int texIDLightBricks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/light_bricks.png");
	unsigned int texIDMosaicBricks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/mosaic_bricks.png");


	// grass + dirt layer
	for (int i = -21; i < 40; i++)
	{
		for (int j = -21; j < 40; j++)
		{
			for (int k = 0; k < 15; k++)
			{
				Entity e(
					ENTITYTYPE::CUBE,
					glm::vec3(i * 2, -3.f - .5f * k, j * 2),
					glm::vec3(2.f, .5f, 2.f),
					(k < 1) ? texIDGrass : (k < 5) ? texIDDirt : texIDCrumblingRocks,
					true,
					false
				);
				prims->addToPrimativeEntities(e);
			}
		}
	}

	// bridge
	for (int i = 10; i < 24; i++)
	{
		for (int j = 5; j < 8; j++)
		{
			Entity e(
				ENTITYTYPE::CUBE,
				glm::vec3(i * 2, -3.f, j * 2),
				glm::vec3(2.f, 2.0f, 2.f),
				texIDLightBricks,
				true,
				false
			);

			prims->addToPrimativeEntities(e);
			if (j == 5 || j == 7)  //side railings
			{
				Entity e(
					ENTITYTYPE::CUBE,
					glm::vec3(i * 2.f, -1.75f, j * 2.f),
					glm::vec3(2.0f, 0.5f, .25f),
					texIDLightBricks,
					true,
					false
				);
				prims->addToPrimativeEntities(e);
			}
		}
	}


  // ---- LOAD SKYBOX ---- //
  //std::vector<std::string> skyboxFiles =
  //{
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/right.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/left.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/top.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/bottom.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/front.png",
  //  "../AncientArcher/cpp/pckgs/skybox/sunny/back.png"
  //};

  sky = new SkyboxRenderer();

}
