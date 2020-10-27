#include <Loop.h>
//#include <glm/glm.hpp>
#include <Lights.h>
#include <rand/rand.h>
#include <CollisionHandler.h>
#include "../include/Astroid.h"
#include "../include/DestroyAstroid.h"
#include "../include/HitAstroid.h"
#include "../include/SplitAstroid.h"

int main()
{
	static int cam_1 = LOOP->addCamera();
	LOOP->getCamera(cam_1).shiftYawAndPitch(0.f, -90.f); // look down

	// fullscreen
	DISPLAY->setWindowSize(800, 800, true);

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

	static const float BOUNDRYSIZE = 10.9f;

	const float TURNSPEED = 210.f;
	static const float TURNSPEEDr = glm::radians(TURNSPEED);
	static const float MOVESPEED = 6.9f;
	static const float FIRELENGTH = .3187f;
	static const float BULLETSPEED = 23.f;

	static float xShipDir;
	static float zShipDir;
	static float xCamPos;
	static float zCamPos;
	static float xFireDir;
	static float zFireDir;
	static bool bulletOut(false);
	static float bulletTimer = 0.f;
	static bool bulletHitSomething(false);

	auto controlShip = [](float dt) {
		if (moveforward)
		{
			glm::vec3 dir = LOOP->getGameObject(go_ship).getRotation();
			xShipDir = sin(dir.y);
			zShipDir = cos(dir.y);
			LOOP->getGameObject(go_ship).advanceTranslate(glm::vec3(xShipDir, 0, zShipDir) * dt * MOVESPEED);

			// cam follow if move
//xCamPos = LOOP->getGameObject(go_ship).getLocation().x;
//zCamPos = LOOP->getGameObject(go_ship).getLocation().z;
//LOOP->getCamera(cam_1).setCurrentPosition(glm::vec3(xCamPos, 0, zCamPos));
//std::cout << "loc" << xCamPos << ',' << zCamPos << '\n';

			//positive bounds
			if (LOOP->getGameObject(go_ship).getLocation().x > BOUNDRYSIZE)
				LOOP->getGameObject(go_ship).advanceTranslate(glm::vec3(-BOUNDRYSIZE * 2, 0, 0));

			if (LOOP->getGameObject(go_ship).getLocation().z > BOUNDRYSIZE)
				LOOP->getGameObject(go_ship).advanceTranslate(glm::vec3(0, 0, -BOUNDRYSIZE * 2));

			//negative bounds
			if (LOOP->getGameObject(go_ship).getLocation().x < -BOUNDRYSIZE)
				LOOP->getGameObject(go_ship).advanceTranslate(glm::vec3(BOUNDRYSIZE * 2, 0, 0));

			if (LOOP->getGameObject(go_ship).getLocation().z < -BOUNDRYSIZE)
				LOOP->getGameObject(go_ship).advanceTranslate(glm::vec3(0, 0, BOUNDRYSIZE * 2));
		}

		if (turnleft)
		{
			LOOP->getGameObject(go_ship).advanceRotation(glm::vec3(0, TURNSPEEDr * dt, 0));
		}

		if (turnright)
		{
			LOOP->getGameObject(go_ship).advanceRotation(glm::vec3(0, -TURNSPEEDr * dt, 0));
		}

		if (fireweap && !bulletOut)
		{
			bulletOut = true;
			glm::vec3 dir = LOOP->getGameObject(go_ship).getRotation();
			xShipDir = sin(dir.y);
			zShipDir = cos(dir.y);
			xFireDir = xShipDir;
			zFireDir = zShipDir;
		}

		if (bulletOut)
		{
			bulletTimer += dt;

			if (bulletTimer >= FIRELENGTH)
			{
				bulletOut = false;
				bulletTimer = 0.f;
				bulletHitSomething = false;
			}

			if (!bulletHitSomething)
			{
				LOOP->getGameObject(go_lazer).advanceTranslate(glm::vec3(xFireDir, 0, zFireDir) * dt * BULLETSPEED);

				//positive bounds
				if (LOOP->getGameObject(go_lazer).getLocation().x > BOUNDRYSIZE)
					LOOP->getGameObject(go_lazer).advanceTranslate(glm::vec3(-BOUNDRYSIZE * 2, 0, 0));

				if (LOOP->getGameObject(go_lazer).getLocation().z > BOUNDRYSIZE)
					LOOP->getGameObject(go_lazer).advanceTranslate(glm::vec3(0, 0, -BOUNDRYSIZE * 2));

				//negative bounds
				if (LOOP->getGameObject(go_lazer).getLocation().x < -BOUNDRYSIZE)
					LOOP->getGameObject(go_lazer).advanceTranslate(glm::vec3(BOUNDRYSIZE * 2, 0, 0));

				if (LOOP->getGameObject(go_lazer).getLocation().z < -BOUNDRYSIZE)
					LOOP->getGameObject(go_lazer).advanceTranslate(glm::vec3(0, 0, BOUNDRYSIZE * 2));
			}
			else
			{
				//apply steady loc & rotation
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

	// ASTROIDS
	static int go_asteroid = LOOP->addObject("../assets/models/obj/asteroid.obj", cam_1, shader_unlit);
	LOOP->getGameObject(go_asteroid).translateTo(glm::vec3(0, -20, -5));
	LOOP->getGameObject(go_asteroid).setColliderSphere(glm::vec3(0, -20, -5), 1.f);

	static int go_asteroid2 = LOOP->addObject("../assets/models/obj/asteroid2.obj", cam_1, shader_unlit);
	LOOP->getGameObject(go_asteroid2).translateTo(glm::vec3(-7, -20, -7));
	LOOP->getGameObject(go_asteroid2).setColliderSphere(glm::vec3(-7, -20, -7), 1.f);

	static std::vector<Astroid> astroids;
	astroids.push_back(createAstroid(go_asteroid, 0, false));
	astroids.push_back(createAstroid(go_asteroid2, 0, false));

	auto checkCollide = []()
	{
		if (bulletOut && !bulletHitSomething) {
			for (auto& ast : astroids)
			{
				if (AA::CollisionHandler::getInstance()->sphere_vs_Sphere_3D
				(
					LOOP->getGameObject(go_lazer).getColliderSphere(),
					LOOP->getGameObject(ast.object_id).getColliderSphere(ast.instance_id)
				))
				{
					//std::cout << "hit! obj id: " << ast.object_id << ", inst id: " << ast.instance_id << '\n';
					bulletHitSomething = true;
					hitAstroid(ast, astroids);
					return;
				}
			}
		}
	};
	LOOP->addToUpdate(checkCollide);

	static const float STARTDELAY = 3.f;
	auto moveasteroids = [](float dt) {
		static float live_timer = 0.f;
		live_timer += dt;
		if (live_timer < STARTDELAY)
			return;
		for (const auto& ast : astroids)
		{
			//positive bounds
			if (LOOP->getGameObject(ast.object_id).getLocation(ast.instance_id).x > BOUNDRYSIZE)
				LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(-BOUNDRYSIZE * 2, 0, 0), ast.instance_id);

			if (LOOP->getGameObject(ast.object_id).getLocation(ast.instance_id).z > BOUNDRYSIZE)
				LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(0, 0, -BOUNDRYSIZE * 2), ast.instance_id);

			//negative bounds
			if (LOOP->getGameObject(ast.object_id).getLocation(ast.instance_id).x < -BOUNDRYSIZE)
				LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(BOUNDRYSIZE * 2, 0, 0), ast.instance_id);

			if (LOOP->getGameObject(ast.object_id).getLocation(ast.instance_id).z < -BOUNDRYSIZE)
				LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(0, 0, BOUNDRYSIZE * 2), ast.instance_id);

			// standard movement
			LOOP->getGameObject(ast.object_id).advanceTranslate(glm::vec3(dt * ast.direction.x * ast.speed, 0, dt * ast.direction.y * ast.speed), ast.instance_id);
			LOOP->getGameObject(ast.object_id).advanceRotation(glm::vec3(dt * ast.rotation.x, dt * ast.rotation.y, dt * ast.rotation.z), ast.instance_id);
		}
	};
	LOOP->addToDeltaUpdate(moveasteroids);

	return LOOP->runMainLoop();
}