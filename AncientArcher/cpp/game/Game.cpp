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
Camera g_camera(glm::vec3(0.f, 1.0f, 0.f), 0, 0, 75.f);
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

	g_lighting.updateConstantLightAmbient(glm::vec3(.15, 0.123, 0.123));
	g_lighting.updateConstantLightDirection(glm::vec3(0.5, -0.75, 0.5));
	g_lighting.updateConstantLightDiffuse(glm::vec3(.38, .38, .38));
	g_lighting.updateConstantLightSpecular(glm::vec3(.2, .2, .2));

	g_lighting.setConstantLight(prims->getShader());

  spawner = new Spawner();
  spawner->setPopulationCap(1);
  spawner->setTimeBetweenSpawns(2.f);

	TextureLoader tLoader;
	unsigned int texIDGrass = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/grass.png");
	unsigned int texIDDirt = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/dirt.png");
	unsigned int texIDLightBricks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/light_bricks.png");
	unsigned int texIDMosaicBricks = tLoader.load2DTexture("../AncientArcher/cpp/pckgs/primatives/mosaic_bricks.png");


	// grass + dirt layer
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				Entity e(
					ENTITYTYPE::CUBE,
					glm::vec3(i * 2, -3.f - .5f * k, j * 2),
					glm::vec3(2.f, .5f, 2.f),
					k < 1 ? texIDGrass : texIDDirt,
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

	// reverse other bridge thingy railings
	for (int i = 10; i < 24; i++)
	{
		for (int j = 5; j < 8; j++)
		{

			if (j == 5 || j == 7)  //secondary
			{
				Entity e(
					ENTITYTYPE::CUBE,
					glm::vec3(
						j * 2,
						-3.5f,
						i * 2),
					glm::vec3(2.0f, 1.0f, 2.0f),
					texIDLightBricks,
					true,
					false
				);
				prims->addToPrimativeEntities(e);
			}
		}
	}

	// MAZE
	for (int i = 24; i < 44; i++)
	{
    {// l/r walls
      Entity e(
        ENTITYTYPE::CUBE,
        glm::vec3(i * 2, -2.2f, -.43),
        glm::vec3(2.f, 3.f, 1.f),
        texIDMosaicBricks,
        true,
        false
      );
      prims->addToPrimativeEntities(e);
    }
    {
      Entity e2(
        ENTITYTYPE::CUBE,
        glm::vec3(i * 2, -2.2f, 38.5),
        glm::vec3(2.f, 3.f, 1.f),
        texIDMosaicBricks,
        true,
        false
      );
      prims->addToPrimativeEntities(e2);
    }

    //back wall
    {
      Entity e3(
        ENTITYTYPE::CUBE,
        glm::vec3(85, -2.2f, -48 + i*2),
        glm::vec3(1.f, 3.f, 2.f),
        texIDMosaicBricks,
        true,
        false
      );
      prims->addToPrimativeEntities(e3);
    }

    //MAZE IN SLIDERS
    {
      Entity eslider1(
        ENTITYTYPE::CUBE,
        glm::vec3(58, -2.2f, -35 + i * 2),
        glm::vec3(1.f, 3.f, 2.f),
        texIDMosaicBricks,
        true,
        false
      );
      prims->addToPrimativeEntities(eslider1);
    }

    {
      Entity eslider2(
        ENTITYTYPE::CUBE,
        glm::vec3(62, -2.2f, -72 + i * 2),
        glm::vec3(1.f, 3.f, 2.f),
        texIDMosaicBricks,
        true,
        false
      );
      prims->addToPrimativeEntities(eslider2);
    }

    {
      Entity eslider3(
        ENTITYTYPE::CUBE,
        glm::vec3(68, -2.2f, -45 + i * 2),
        glm::vec3(1.f, 3.f, 2.f),
        texIDMosaicBricks,
        true,
        false
      );
      prims->addToPrimativeEntities(eslider3);
    }

    {
      Entity eslider4(
        ENTITYTYPE::CUBE,
        glm::vec3(76, -2.2f, -52 + i * 2),
        glm::vec3(1.f, 3.f, 2.f),
        texIDDirt,
        true,
        false
      );
      prims->addToPrimativeEntities(eslider4);
    }

		for (int j = 0; j < 20; j++)
		{

			for (int k = 0; k < 2; k++)
			{
				// ground
				Entity e(
					ENTITYTYPE::CUBE,
					glm::vec3(i * 2, -4.f - .5f * k, j * 2),
					glm::vec3(2.f, .5f, 2.f),
					k < 1 ? texIDGrass : texIDDirt,
					true,
					false
				);
				prims->addToPrimativeEntities(e);
			}

      {
        // ceiling
        Entity eCeiling1(
          ENTITYTYPE::CUBE,
          glm::vec3(i * 2, 20.f, j * 2),
          glm::vec3(2.f, .5f, 2.f),
          texIDMosaicBricks,
          true,
          false
        );
        prims->addToPrimativeEntities(eCeiling1);

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
					ENTITYTYPE::SPHERE,
					glm::vec3(i * 2, -2.f, j * 2),
					glm::vec3(2.0f, 1.0f, .5f),
					texIDLightBricks,
					true,
					false
				);
				prims->addToPrimativeEntities(e);
			}
		}
	}

	// few random blocks off to the side
	for (int i = 24; i < 44; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				Entity e(
					ENTITYTYPE::CUBE,
					glm::vec3(i * 2, -4.f - .5f * k, j * 2),
					glm::vec3(2.f, .5f, 2.f),
					k < 1 ? texIDGrass : texIDDirt,
					true,
					false
				);
				prims->addToPrimativeEntities(e);
			}
		}
	}

	//Moving Blocks
	for (int i = 0; i < 4; i++)  //ground
	{
		for (int j = 0; j < 5; j++)
		{
			Entity e(
				ENTITYTYPE::CUBE,
				glm::vec3(47.5f, (1.f * j) - 3.25f, (1.f* i) + 9.f),
				glm::vec3(1.f, 1.f, 1.f),
				texIDMosaicBricks,
				true,
				false
			);
			prims->addToMovingEntities(e);

		}
	}


  // ---- LOAD SKYBOX ---- //
  std::vector<std::string> skyboxFiles =
  {
    "../AncientArcher/cpp/pckgs/skybox/stars/right.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/left.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/top.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/bottom.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/front.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/back.png"
  };

  sky = new SkyboxRenderer(skyboxFiles);

}
