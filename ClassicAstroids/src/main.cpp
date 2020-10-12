#include <Loop.h>
#include <glm/glm.hpp>
#include <Lights.h>
#include <rand/rand.h>
#include <CollisionHandler.h>
#include "../include/SplitAstroid.h"

int main()
{
	static int cam_1 = LOOP->addCamera();
	LOOP->getCamera(cam_1).shiftYawAndPitch(0.f, -90.f); // look down

	// fullscreen
//	DISPLAY->setWindowSize('f');

	static int shader_unlit = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");

	static int shader_lit = LOOP->addShader("../assets/shaders/combinedLight.vert", "../assets/shaders/combinedLight.frag");
	// directional light for lit shader
	static AA::DirectionalLight dLight;
	dLight.Direction = glm::vec3(-0.45f, -1.f, 0.f);
	dLight.Ambient = glm::vec3(0.05f);
	dLight.Diffuse = glm::vec3(0.3f);
	dLight.Specular = glm::vec3(1);
	setDirectionalLight(dLight, LOOP->getShader(shader_lit));

	// BACKDROP
	static int go_starplane = LOOP->addObject("../assets/models/obj/starplane.obj", cam_1, shader_lit);
	LOOP->getGameObject(go_starplane).translateTo(glm::vec3(0, -99, 0));
	LOOP->getGameObject(go_starplane).scaleTo(glm::vec3(10, 1, 10));

	// PLAYER SHIP
	static int go_ship = LOOP->addObject("../assets/models/obj/flyingV.obj", cam_1, shader_unlit);
	LOOP->getGameObject(go_ship).translateTo(glm::vec3(0, -20, 0));
	LOOP->getGameObject(go_ship).scaleTo(glm::vec3(.6667f));
	LOOP->getGameObject(go_ship).rotateTo(glm::vec3(0, glm::radians(180.f), 0));

	// LAZER BULLET
	static int go_lazer = LOOP->addObject("../assets/models/obj/lazer.obj", cam_1, shader_unlit);
	LOOP->getGameObject(go_lazer).translateTo(LOOP->getGameObject(go_ship).getLocation());
	LOOP->getGameObject(go_lazer).scaleTo(glm::vec3(.3333f));
	LOOP->getGameObject(go_lazer).rotateTo(glm::vec3(0, glm::radians(180.f), 0));
	LOOP->getGameObject(go_lazer).setColliderSphere(LOOP->getGameObject(go_ship).getLocation(), .02f);

	static bool turnleft(false), turnright(false), moveforward(false), fireweap(false);
	auto hotkeys = [](AA::KeyboardInput& keyin)
	{
		if (keyin.w || keyin.upArrow)  // move in pointing direction
		{
			moveforward = true;
		}
		else if (!(keyin.w || keyin.upArrow))
		{
			moveforward = false;
		}

		if (keyin.a || keyin.leftArrow)  // turn left
		{
			turnleft = true;
		}
		else if (!(keyin.a || keyin.leftArrow))
		{
			turnleft = false;
		}

		if (keyin.d || keyin.rightArrow)  // turn right
		{
			turnright = true;
		}
		else if (!(keyin.d || keyin.rightArrow))
		{
			turnright = false;
		}

		if (keyin.spacebar || keyin.mouseButton1)
		{
			fireweap = true;
		}
		else if (!(keyin.spacebar || keyin.mouseButton1))
		{
			fireweap = false;
		}
	};
	LOOP->addToKeyHandling(hotkeys);

	const float TURNSPEED = 220.f;
	static const float TURNSPEEDr = glm::radians(TURNSPEED);
	static const float MOVESPEED = 5.9f;
	static const float FIRELENGTH = .7187f;
	static const float BULLETSPEED = 24.f;

	static float xShipDir;
	static float zShipDir;
	static float xCamPos;
	static float zCamPos;
	static float xFireDir;
	static float zFireDir;
	static bool bulletLive(false);
	auto controlShip = [](float dt) {
		if (moveforward)
		{
			glm::vec3 dir = LOOP->getGameObject(go_ship).getRotation();
			xShipDir = sin(dir.y);
			zShipDir = cos(dir.y);
			LOOP->getGameObject(go_ship).advanceTranslate(glm::vec3(xShipDir, 0, zShipDir) * dt * MOVESPEED);

			// cam follow if move
			xCamPos = LOOP->getGameObject(go_ship).getLocation().x;
			zCamPos = LOOP->getGameObject(go_ship).getLocation().z;
			LOOP->getCamera(cam_1).setCurrentPosition(glm::vec3(xCamPos, 0, zCamPos));
		}

		if (turnleft)
		{
			LOOP->getGameObject(go_ship).advanceRotation(glm::vec3(0, TURNSPEEDr * dt, 0));
		}

		if (turnright)
		{
			LOOP->getGameObject(go_ship).advanceRotation(glm::vec3(0, -TURNSPEEDr * dt, 0));
		}

		if (fireweap && !bulletLive)
		{
			bulletLive = true;
			glm::vec3 dir = LOOP->getGameObject(go_ship).getRotation();
			xShipDir = sin(dir.y);
			zShipDir = cos(dir.y);
			xFireDir = xShipDir;
			zFireDir = zShipDir;
		}

		if (bulletLive)
		{
			static float bulletTimer = 0.f;
			LOOP->getGameObject(go_lazer).advanceTranslate(glm::vec3(xFireDir, 0, zFireDir) * dt * BULLETSPEED);
			bulletTimer += dt;
			if (bulletTimer >= FIRELENGTH)
			{
				bulletLive = false;
				bulletTimer = 0.f;
				LOOP->getGameObject(go_lazer).translateTo(LOOP->getGameObject(go_ship).getLocation());
				LOOP->getGameObject(go_lazer).translateTo(LOOP->getGameObject(go_ship).getLocation());
				LOOP->getGameObject(go_lazer).rotateTo(LOOP->getGameObject(go_ship).getRotation());
			}
		}
		else
		{
			//apply steady loc & rotation
			LOOP->getGameObject(go_lazer).translateTo(LOOP->getGameObject(go_ship).getLocation());
			LOOP->getGameObject(go_lazer).rotateTo(LOOP->getGameObject(go_ship).getRotation());
		}
	};
	LOOP->addToDeltaUpdate(controlShip);

	// ASTEROIDS
	static int go_asteroid = LOOP->addObject("../assets/models/obj/asteroid.obj", cam_1, shader_unlit);
	LOOP->getGameObject(go_asteroid).translateTo(glm::vec3(0, -20, -10));
	LOOP->getGameObject(go_asteroid).setColliderSphere(glm::vec3(0, -20, -10), 1.f);

	static int go_asteroid2 = LOOP->addObject("../assets/models/obj/asteroid2.obj", cam_1, shader_unlit);
	LOOP->getGameObject(go_asteroid2).translateTo(glm::vec3(-7, -20, -7));
	LOOP->getGameObject(go_asteroid2).setColliderSphere(glm::vec3(-7, -20, -7), 1.f);

	auto checkCollide = []() {
		if (AA::CollisionHandler::getInstance()->sphere_vs_Sphere_3D(
			LOOP->getGameObject(go_lazer).getColliderSphere(), LOOP->getGameObject(go_asteroid).getColliderSphere()
		)) {
			//std::cout << "first astroid hit\n";
			splitAstroid(go_asteroid);
		}

		if (AA::CollisionHandler::getInstance()->sphere_vs_Sphere_3D(
			LOOP->getGameObject(go_lazer).getColliderSphere(), LOOP->getGameObject(go_asteroid2).getColliderSphere()
		)) {
			//std::cout << "second astroid hit\n";
			splitAstroid(go_asteroid2);
		}
	};
	LOOP->addToUpdate(checkCollide);

	// get 6 random floats for rotations
	static float rand[12];
	for (int i = 0; i < 12; i++)
	{
		rand[i] = NTKR(0.f, 2.6f);
	}

	auto moveasteroids = [](float dt) {
		LOOP->getGameObject(go_asteroid).advanceRotation(glm::vec3(dt * rand[0], dt * rand[1], dt * rand[2]));
		LOOP->getGameObject(go_asteroid2).advanceRotation(glm::vec3(dt * rand[3], dt * rand[4], dt * rand[5]));
	};
	LOOP->addToDeltaUpdate(moveasteroids);

	return LOOP->runMainLoop();
}